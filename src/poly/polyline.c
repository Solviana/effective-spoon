#include "common_types.h"

#include "polyline.h"
#include <stdlib.h>

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

void poly_insert(type_polyline_st* f_line_pst, type_point_st f_point_st, uint32_t f_dest_u32)
{
    type_polyline_st* l_curr_pst = f_line_pst;
    type_polyline_st* l_new_pst = (type_polyline_st*)calloc(1, sizeof(type_polyline_st));

    for(uint32_t i = 0; i < f_dest_u32 - 1 && l_curr_pst->next_pst != NULL; i++)
    {
        l_curr_pst = l_curr_pst->next_pst;
    }

    l_new_pst->next_pst = l_curr_pst->next_pst;
    l_curr_pst->next_pst = l_new_pst;
    l_curr_pst->p_st = f_point_st;
}