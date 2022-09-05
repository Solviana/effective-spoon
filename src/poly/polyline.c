#include "common_types.h"

#include "polyline.h"
#include <stdlib.h>
#include <math.h>

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