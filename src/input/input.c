#include "common_types.h"
#include <stdbool.h>
#include <stdio.h>
#include "input.h"


// assumes correct allocation
static void read_line(type_polyline_st* f_line_pst)
{
    scanf("%f %f %f %f", 
        &f_line_pst->p_st.x_f, &f_line_pst->p_st.y_f, 
        &f_line_pst->next_pst->p_st.x_f, &f_line_pst->next_pst->p_st.y_f);
}

// assumes correct allocation
static void read_polyline(type_polyline_st* f_line_pst, const uint8_t f_npoints_u8)
{
    type_polyline_st* l_curr_pst = f_line_pst;
    for(int i = 0; i < f_npoints_u8; i++)
    {
        scanf("%f %f", &l_curr_pst->p_st.x_f, &l_curr_pst->p_st.y_f);
        l_curr_pst = l_curr_pst->next_pst;
    }
}