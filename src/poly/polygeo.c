#include "common_types.h"

#include <stdbool.h>
#include <math.h>
#include "polygeo.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct tag_triangle_st
{
    type_point_st p1_st;
    type_point_st p2_st;
    type_point_st p3_st;
} type_triangle_st;

static type_float triangle_area(const type_triangle_st f_triangle_st);

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

// intersection of the lines defined by the first 2 points in the polyline (first line)
// rest of the points/lines are not used
bool poly_intersect(const type_polyline_st *const f_line1_pst, const type_polyline_st *const f_line2_pst, type_point_st *f_intersect_pst)
{
    // line equation ax + by = c
    bool l_ret_b = false;
    type_float a1 = f_line1_pst->p_st.y_f - f_line1_pst->next_pst->p_st.y_f;
    type_float b1 = f_line1_pst->p_st.x_f - f_line1_pst->next_pst->p_st.x_f;
    type_float c1 = a1 * f_line1_pst->p_st.x_f + b1 * f_line1_pst->p_st.y_f;

    type_float a2 = f_line2_pst->p_st.y_f - f_line2_pst->next_pst->p_st.y_f;
    type_float b2 = f_line2_pst->p_st.x_f - f_line2_pst->next_pst->p_st.x_f;
    type_float c2 = a2 * f_line2_pst->p_st.x_f + b2 * f_line2_pst->p_st.y_f;

    type_float det = a1*b2 - a2*b1;

    if (det != 0)
    {
        f_intersect_pst->x_f = (b2*c1 - b1*c2) / det;
        f_intersect_pst->y_f = (a1*c2 - a2*c1) / det;
    }
    
    // determine if intersect happens inside the section 
    if (MIN(f_line2_pst->p_st.x_f, f_line1_pst->p_st.x_f) < f_intersect_pst->x_f
        && MAX(f_line2_pst->p_st.x_f, f_line1_pst->p_st.x_f) > f_intersect_pst->x_f
        && MIN(f_line2_pst->p_st.y_f, f_line1_pst->p_st.y_f) < f_intersect_pst->y_f
        && MAX(f_line2_pst->p_st.y_f, f_line1_pst->p_st.y_f) > f_intersect_pst->y_f)
    {
        l_ret_b = true;
    }

    return l_ret_b;
}
