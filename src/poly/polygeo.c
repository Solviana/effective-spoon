#include "common_types.h"

#include <math.h>
#include <stdlib.h>
#include "polygeo.h"
#include "polyline.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct tag_triangle_st
{
    type_point_st p1_st;
    type_point_st p2_st;
    type_point_st p3_st;
} type_triangle_st;

static type_float triangle_area(const type_triangle_st f_triangle_st);
static bool poly_intersect_line_line(const type_polyline_st *const f_line1_pst, const type_polyline_st *const f_line2_pst, type_point_st *f_intersect_pst);

type_float poly_area(const type_polyline_st *const f_shape_st)
{
    type_float l_ret_f = 0;
    type_triangle_st l_triangle_st;
    type_polyline_st* l_curr_pst;
    
    l_triangle_st.p1_st = f_shape_st->p_st;
    l_triangle_st.p2_st = f_shape_st->next_pst->p_st;
    l_triangle_st.p3_st = f_shape_st->next_pst->next_pst->p_st;
    l_curr_pst = f_shape_st->next_pst->next_pst->next_pst;
    l_ret_f += triangle_area(l_triangle_st);

    while (l_curr_pst != NULL)
    {
        l_triangle_st.p2_st = l_triangle_st.p3_st;
        l_triangle_st.p3_st = l_curr_pst->p_st;
        l_ret_f += triangle_area(l_triangle_st);
        l_curr_pst = l_curr_pst->next_pst;
    }

    return l_ret_f;
}

static type_float triangle_area(const type_triangle_st f_triangle_st)
{
    // Area = (1/2) * abs(x1(y2 − y3) + x2(y3 − y1) + x3(y1 − y2))
    type_float l_ret_f = 0;
    l_ret_f += f_triangle_st.p1_st.x_f * (f_triangle_st.p2_st.y_f - f_triangle_st.p3_st.y_f);
    l_ret_f += f_triangle_st.p2_st.x_f * (f_triangle_st.p3_st.y_f - f_triangle_st.p1_st.y_f);
    l_ret_f += f_triangle_st.p3_st.x_f * (f_triangle_st.p1_st.y_f - f_triangle_st.p2_st.y_f);
    l_ret_f = (type_float)fabs(l_ret_f);
    l_ret_f /= 2;
    return l_ret_f;
}

// cuts a convex(!) polyline into 2
bool poly_cut_polyline(type_polyline_st* f_polyline_pst, const type_polyline_st* const f_line_pst, type_polyline_st** f_newpolylines_ppst)
{
    bool l_ret_b = false;
    uint8_t l_nintersect_u8 = 0;
    type_polyline_st* l_intersectionlink_ppst[2];
    type_polyline_st* l_curr_pst = f_polyline_pst;

    /**
     * find intersection points if they exist
     * add them to the polygon and remember their location
     * */
    while(l_curr_pst->next_pst != NULL)
    {
        bool l_intersect_b = false;
        type_point_st l_intersection_st;
        l_intersect_b = poly_intersect_line_line(l_curr_pst, f_line_pst, &l_intersection_st);

        if(l_intersect_b)
        {
            l_intersectionlink_ppst[l_nintersect_u8 & 1] = poly_insert(l_curr_pst, l_intersection_st);
            l_curr_pst = l_intersectionlink_ppst[l_nintersect_u8 & 1]->next_pst;
            l_nintersect_u8++;
        }
        else
        {
            l_curr_pst = l_curr_pst->next_pst;
        }
    }
    // handle endpoint startpoint line
    {
        bool l_intersect_b = false;
        type_point_st l_intersection_st;
        type_polyline_st l_endline_st;
        l_endline_st = *l_curr_pst;
        l_endline_st.next_pst = f_polyline_pst;
        l_intersect_b = poly_intersect_line_line(&l_endline_st, f_line_pst, &l_intersection_st);
        if(l_intersect_b)
        {
            l_intersectionlink_ppst[l_nintersect_u8 & 1] = poly_insert(l_curr_pst, l_intersection_st);
            l_nintersect_u8++;
        }
    }
    // sanity check, something went wrong
    if (l_nintersect_u8 > 2)
    {
        exit(1); 
    }

    /**
     * Seperate the 2 shapes if the intersection happened
     * */
    if(l_nintersect_u8 == 2)
    {
        l_ret_b = true;
        // duplicate the intersection points
        type_polyline_st* l_intersectionlinknew_ppst[2];
        for(int i = 0; i < 2; i++)
        {
            l_intersectionlinknew_ppst[i] = poly_alloc(1);
            l_intersectionlinknew_ppst[i]->p_st = l_intersectionlink_ppst[i]->p_st;
        }

        // for the first shape: 
        // start from the starting point 
        // get to the first intersection link
        // replace it with duplicate
        // point duplicate to second intersection link

        // room for improvement: with this approach there is a special case, when the line directly intersects the starting point
        // there is probably a better approach
        l_curr_pst = f_polyline_pst;
        if(l_curr_pst == l_intersectionlink_ppst[0])
        {
            l_intersectionlinknew_ppst[0]->next_pst = l_intersectionlink_ppst[1];
            f_newpolylines_ppst[0] = l_intersectionlinknew_ppst[0];
        }
        else
        {
            while(l_curr_pst->next_pst != l_intersectionlink_ppst[0]) // in theory next will not be null
            {
                l_curr_pst = l_curr_pst->next_pst;
            }
            l_curr_pst->next_pst = l_intersectionlinknew_ppst[0];
            l_intersectionlinknew_ppst[0]->next_pst = l_intersectionlink_ppst[1];
            f_newpolylines_ppst[0] = f_polyline_pst;
        }

        // for the second shape:
        // start at the first intersection
        // find the second intersection 
        // replace the second intersection with it's duplicate (where ->next_pst is NULL)
        l_curr_pst = l_intersectionlink_ppst[0];
        while(l_curr_pst->next_pst != l_intersectionlink_ppst[1])
        {
            l_curr_pst = l_curr_pst->next_pst;
        }
        l_curr_pst->next_pst = l_intersectionlinknew_ppst[1];

        f_newpolylines_ppst[1] = l_intersectionlink_ppst[0];
    }

    return l_ret_b;
}

// intersection of the lines defined by the first 2 points in the polyline (first line)
// rest of the points/lines are not used
static bool poly_intersect_line_line(const type_polyline_st *const f_line1_pst, const type_polyline_st *const f_line2_pst, type_point_st *f_intersect_pst)
{
    // line equation ax + by = c
    bool l_ret_b = false;
    type_float a1 = f_line1_pst->next_pst->p_st.y_f - f_line1_pst->p_st.y_f;
    type_float b1 = f_line1_pst->p_st.x_f - f_line1_pst->next_pst->p_st.x_f;
    type_float c1 = a1 * f_line1_pst->p_st.x_f + b1 * f_line1_pst->p_st.y_f;

    type_float a2 = f_line2_pst->next_pst->p_st.y_f - f_line2_pst->p_st.y_f;
    type_float b2 = f_line2_pst->p_st.x_f - f_line2_pst->next_pst->p_st.x_f;
    type_float c2 = a2 * f_line2_pst->p_st.x_f + b2 * f_line2_pst->p_st.y_f;

    type_float det = a1*b2 - a2*b1;

    if (det != 0)
    {
        f_intersect_pst->x_f = (b2*c1 - b1*c2) / det;
        f_intersect_pst->y_f = (a1*c2 - a2*c1) / det;
        // determine if intersect happens on line 1 
        // starting point is isncluded, endpoint not!
        // float comparison room for improvement
        if (f_line1_pst->next_pst->p_st.x_f == f_intersect_pst->x_f
            && f_line1_pst->next_pst->p_st.y_f == f_intersect_pst->y_f)
        {
            l_ret_b = false;
        }
        else if (MIN(f_line1_pst->p_st.x_f, f_line1_pst->next_pst->p_st.x_f) <= f_intersect_pst->x_f
            && MAX(f_line1_pst->p_st.x_f, f_line1_pst->next_pst->p_st.x_f) >= f_intersect_pst->x_f
            && MIN(f_line1_pst->p_st.y_f, f_line1_pst->next_pst->p_st.y_f) <= f_intersect_pst->y_f
            && MAX(f_line1_pst->p_st.y_f, f_line1_pst->next_pst->p_st.y_f) >= f_intersect_pst->y_f)
        {
            l_ret_b = true;
        }
    }
    
    return l_ret_b;
}
