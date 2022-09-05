#include "common_types.h"

#include <stdio.h>
#include <stdlib.h>
#include "polyqueue.h"
#include "polygeo.h"
#include "input.h"
#include "game.h"

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
        free(l_linebuffer_ppst[i]); // line not needed anymore
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