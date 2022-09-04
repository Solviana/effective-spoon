#include "common_types.h"

#include <stdlib.h>
#include "polyline.h"
#include "polyqueue.h"

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
    if (f_q_pst->tail_u32 == f_q_pst->size_u32 - 1)
    {
        game_polyqueue_realloc(f_q_pst);
    }
    f_q_pst->tail_u32 += 1;
    f_q_pst->polygonbuffer_ppst[f_q_pst->tail_u32] = f_line_pst;
}

// room for improvement: reclaim everything before head
void game_polyqueue_realloc(type_polyqueue_st* f_q_pst) 
{
    f_q_pst->size_u32 += POLYGONBUFFER_REALLOC_SIZE;
    f_q_pst->polygonbuffer_ppst = (type_polyline_st**)realloc(f_q_pst->polygonbuffer_ppst, f_q_pst->size_u32);
}

void game_polyqueue_dealloc(type_polyqueue_st* f_q_pst)
{
    for(int i = 0; i <= f_q_pst->tail_u32; i++)
    {
        poly_free(f_q_pst->polygonbuffer_ppst[i]);
    }
    free(f_q_pst->polygonbuffer_ppst);
}