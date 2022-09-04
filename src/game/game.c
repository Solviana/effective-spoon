#include "common_types.h"

#include <stdio.h>
#include "polyline.h"
#include "polyqueue.h"
#include "polygeo.h"
#include "input.h"
#include "game.h"

void game_play(void)
{
    
    type_polyqueue_st l_polygonbuffer_st;
    type_polyline_st** l_linebuffer_ppst;
    uint8_t l_lines_u8;
    uint16_t l_polygons_u16 = 1;

    game_polyqueue_realloc(&l_polygonbuffer_st);
    l_lines_u8 = input_read(&l_linebuffer_ppst, l_polygonbuffer_st.polygonbuffer_ppst);

}

