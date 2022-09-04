#ifndef POLYQUEUE_H
#define POLYQUEUE_H

typedef struct tag_polyqueue_st
{
    type_polyline_st** polygonbuffer_ppst;
    uint32_t size_u32;
    uint32_t head_u32;
    uint32_t tail_u32;
} type_polyqueue_st;

void game_polyqueue_realloc(type_polyqueue_st* f_q_st);
void game_polyqueue_dealloc(type_polyqueue_st* f_q_st);
void game_polyqueue_enqueue(type_polyqueue_st* f_q_pst, type_polyline_st* f_line_pst);
type_polyline_st* game_polyqueue_dequeue(type_polyqueue_st* f_q_st);

#endif