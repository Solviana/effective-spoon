#include "common_types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "polyline.h"

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
        scanf("%f %f", &l_curr_pst->p_st.x_f, &l_curr_pst->p_st.y_f);
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
    scanf("%f %f %f %f", 
        &f_line_pst->p_st.x_f, &f_line_pst->p_st.y_f, 
        &f_line_pst->next_pst->p_st.x_f, &f_line_pst->next_pst->p_st.y_f);
}