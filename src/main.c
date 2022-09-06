#include <stddef.h> //for NULL
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double type_float;

typedef struct tag_point
{
    type_float x_f;
    type_float y_f;
} type_point_st;

typedef struct tag_polyline
{
    type_point_st p_st;
    struct tag_polyline* next_pst;
} type_polyline_st;

typedef struct tag_polyqueue_st
{
    type_polyline_st** polygonbuffer_ppst;
    uint32_t alloc_size_u32;
    uint32_t head_u32;
    uint32_t tail_u32;
} type_polyqueue_st;

type_float game_play(void);
uint8_t input_read(type_polyline_st*** f_linebuffer_pppst, type_polyline_st** f_polygon_ppst);
type_polyline_st* poly_alloc(uint32_t f_len_u32);
void poly_free(type_polyline_st* f_line_pst);
type_polyline_st* poly_insert(type_polyline_st* f_line_pst, type_point_st f_point_st);
type_float poly_area(const type_polyline_st* const f_shape_pst);
bool poly_cut_polyline(type_polyline_st* f_polyline_pst, const type_polyline_st* const f_line_pst, type_polyline_st** f_newpolylines_ppst);
void game_polyqueue_realloc(type_polyqueue_st* f_q_st);
void game_polyqueue_dealloc(type_polyqueue_st* f_q_st);
void game_polyqueue_enqueue(type_polyqueue_st* f_q_pst, type_polyline_st* f_line_pst);
type_polyline_st* game_polyqueue_dequeue(type_polyqueue_st* f_q_st);
uint32_t game_polyqueue_used_size(const type_polyqueue_st* f_q_pst);

int main(void)
{
    (void)game_play();

    return 0;
}

static void game_draw_line(type_polyqueue_st* f_q_pst, const type_polyline_st* const f_line_pst);
static type_float game_find_largest(const type_polyqueue_st* f_q_pst);

type_float game_play(void)
{
    type_polyqueue_st l_polygonbuffer_st = {NULL, 0, 0, 0};
    type_polyline_st** l_linebuffer_ppst;
    uint8_t l_lines_u8;
    type_float l_maxarea_f;

    game_polyqueue_realloc(&l_polygonbuffer_st);
    l_lines_u8 = input_read(&l_linebuffer_ppst, l_polygonbuffer_st.polygonbuffer_ppst);

    for(uint32_t i = 0; i < l_lines_u8; i++)
    {
        game_draw_line(&l_polygonbuffer_st, l_linebuffer_ppst[i]);
        poly_free(l_linebuffer_ppst[i]); // line not needed anymore
    }

    l_maxarea_f = game_find_largest(&l_polygonbuffer_st);
    printf("%lf", l_maxarea_f);
    
    game_polyqueue_dealloc(&l_polygonbuffer_st);
    free(l_linebuffer_ppst);
    
    return l_maxarea_f;
}

static void game_draw_line(type_polyqueue_st* f_q_pst, const type_polyline_st* const f_line_pst)
{
    uint32_t l_npolygons_u32;
    
    l_npolygons_u32 = game_polyqueue_used_size(f_q_pst);
    // for each polygon check if the line cuts it in half
    // if no then queue the checked polygon
    // if yes then get the 2 seperate polygons and queue them
    for(uint32_t i = 0; i < l_npolygons_u32; i++)
    {
        type_polyline_st* l_curr_pst;
        type_polyline_st* l_new_ppst[2];
        bool l_cut_b;
        l_curr_pst = game_polyqueue_dequeue(f_q_pst);
        l_cut_b = poly_cut_polyline(l_curr_pst, f_line_pst, l_new_ppst);
        if(l_cut_b)
        {
            game_polyqueue_enqueue(f_q_pst, l_new_ppst[0]);
            game_polyqueue_enqueue(f_q_pst, l_new_ppst[1]);
        }
        else
        {
            game_polyqueue_enqueue(f_q_pst, l_curr_pst);
        }
    }
}

static type_float game_find_largest(const type_polyqueue_st* f_q_pst)
{
    type_float l_ret_f = 0.;
    const type_polyline_st* l_curr_pst;
    
    // direct array access, room for improvement
    for(uint32_t i = f_q_pst->head_u32; i < f_q_pst->tail_u32 + 1; i++)
    {
        type_float l_size_f;
        l_curr_pst = f_q_pst->polygonbuffer_ppst[i];
        l_size_f = poly_area(l_curr_pst);
        if(l_size_f > l_ret_f)
        {
            l_ret_f = l_size_f;
        }
    }

    return l_ret_f;
}

#define POLYGONBUFFER_REALLOC_SIZE 1000

type_polyline_st* game_polyqueue_dequeue(type_polyqueue_st* f_q_pst)
{
    type_polyline_st* l_ret_pst = NULL;
    if (f_q_pst->head_u32 <= f_q_pst->tail_u32) // if not empty
    {
        l_ret_pst = f_q_pst->polygonbuffer_ppst[f_q_pst->head_u32];
        f_q_pst->head_u32 += 1;
    }
    return l_ret_pst;
}

void game_polyqueue_enqueue(type_polyqueue_st* f_q_pst, type_polyline_st* f_line_pst)
{
    if (game_polyqueue_used_size(f_q_pst) == f_q_pst->alloc_size_u32)
    {
        game_polyqueue_realloc(f_q_pst);
    }
    f_q_pst->tail_u32 += 1;
    f_q_pst->polygonbuffer_ppst[f_q_pst->tail_u32] = f_line_pst;
}

uint32_t game_polyqueue_used_size(const type_polyqueue_st* f_q_pst)
{
    uint32_t l_ret_u32;
    l_ret_u32 = f_q_pst->tail_u32 - f_q_pst->head_u32 + 1;
    return l_ret_u32;
}

// room for improvement: reclaim everything before head
void game_polyqueue_realloc(type_polyqueue_st* f_q_pst) 
{
    f_q_pst->alloc_size_u32 += POLYGONBUFFER_REALLOC_SIZE;
    f_q_pst->polygonbuffer_ppst = (type_polyline_st**)realloc(f_q_pst->polygonbuffer_ppst, f_q_pst->alloc_size_u32);
}

// room for improvement deallocating polylines here is not the best solution (anything before head has to be handled by the user)
// we might end up being leaky
void game_polyqueue_dealloc(type_polyqueue_st* f_q_pst)
{
    for(int i = f_q_pst->head_u32; i <= f_q_pst->tail_u32; i++)
    {
        poly_free(f_q_pst->polygonbuffer_ppst[i]);
    }
    free(f_q_pst->polygonbuffer_ppst);
}

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

static type_float point_dist(type_point_st f_p1_st, type_point_st f_p2_st);

type_polyline_st* poly_alloc(uint32_t f_len_u32)
{
    type_polyline_st* l_ret_pst;
    type_polyline_st* l_curr_pst;

    l_ret_pst = (type_polyline_st*)calloc(1, sizeof(type_polyline_st)); // room for improvement, explicit value init
    l_curr_pst = l_ret_pst;

    for(uint32_t i = 0; i < f_len_u32 - 1; i++)
    {
        l_curr_pst->next_pst = (type_polyline_st*)calloc(1, sizeof(type_polyline_st));
        l_curr_pst = l_curr_pst->next_pst;
    }

    l_curr_pst->next_pst = NULL;

    return l_ret_pst;
}

void poly_free(type_polyline_st* f_line_pst)
{
    type_polyline_st* l_curr_pst = f_line_pst;
    type_polyline_st* l_next_pst;

    while(l_curr_pst->next_pst != NULL)
    {
        l_next_pst = l_curr_pst->next_pst;
        free(l_curr_pst);
        l_curr_pst = l_next_pst;
    }

    free(l_curr_pst);
}

type_polyline_st* poly_insert(type_polyline_st* f_line_pst, type_point_st f_point_st)
{
    type_polyline_st* l_ret_pst;
    type_polyline_st* l_new_pst;
    type_float l_dist_f = point_dist(f_point_st, f_line_pst->p_st);

    // room for improvement better fp arithmetic
    if(l_dist_f > 1.e-5)
    {
        l_new_pst = poly_alloc(1);
        l_new_pst->p_st = f_point_st;
        l_new_pst->next_pst = f_line_pst->next_pst;
        f_line_pst->next_pst = l_new_pst;
        l_ret_pst = l_new_pst;
    }
    else
    {
        l_ret_pst = f_line_pst;
    }

    return l_ret_pst;
}

static type_float point_dist(type_point_st f_p1_st, type_point_st f_p2_st)
{
    type_float l_sq_x_f = (f_p2_st.x_f - f_p1_st.x_f) * (f_p2_st.x_f - f_p1_st.x_f);
    type_float l_sq_y_f = (f_p2_st.y_f - f_p1_st.y_f) * (f_p2_st.y_f - f_p1_st.y_f);
    type_float l_ret_f = sqrt(l_sq_x_f + l_sq_y_f);
    return l_ret_f;
}

static void input_read_polyline(type_polyline_st* f_line_pst, const uint8_t f_npoints_u8);
static void input_read_line(type_polyline_st* f_line_pst); // for testing
static void input_read_lines(type_polyline_st** f_linebuffer_ppst, const uint8_t f_nlines_u8);
static void input_read_size(uint8_t* f_npoints_pu8, uint8_t* f_nlines_pu8);

uint8_t input_read(type_polyline_st*** f_linebuffer_pppst, type_polyline_st** f_polygon_ppst)
{
    uint8_t l_sides_u8;
    uint8_t l_lines_u8;
    type_polyline_st** l_linebuffer_ppst;

    input_read_size(&l_sides_u8, &l_lines_u8);
    *f_polygon_ppst = poly_alloc(l_sides_u8);
    l_linebuffer_ppst = (type_polyline_st**)calloc(l_lines_u8, sizeof(type_polyline_st*));
    for (int i = 0; i < l_lines_u8; i++)
    {
        l_linebuffer_ppst[i] = poly_alloc(2);
    }
    input_read_polyline(*f_polygon_ppst, l_sides_u8);
    input_read_lines(l_linebuffer_ppst, l_lines_u8);
    *f_linebuffer_pppst = l_linebuffer_ppst;

    return l_lines_u8;
}

static void input_read_size(uint8_t* f_npoints_pu8, uint8_t* f_nlines_pu8)
{
    scanf("%hhu %hhu", f_npoints_pu8, f_nlines_pu8);
}

static void input_read_polyline(type_polyline_st* f_line_pst, const uint8_t f_npoints_u8)
{
    type_polyline_st* l_curr_pst = f_line_pst;
    for(int i = 0; i < f_npoints_u8; i++)
    {
        scanf("%lf %lf", &l_curr_pst->p_st.x_f, &l_curr_pst->p_st.y_f);
        l_curr_pst = l_curr_pst->next_pst;
    }
}

static void input_read_lines(type_polyline_st** f_linebuffer_ppst, const uint8_t f_nlines_u8)
{
    for(int i = 0; i < f_nlines_u8; i++)
    {
        input_read_line(f_linebuffer_ppst[i]);
    }
}

static void input_read_line(type_polyline_st* f_line_pst)
{
    scanf("%lf %lf %lf %lf", 
        &f_line_pst->p_st.x_f, &f_line_pst->p_st.y_f, 
        &f_line_pst->next_pst->p_st.x_f, &f_line_pst->next_pst->p_st.y_f);
}