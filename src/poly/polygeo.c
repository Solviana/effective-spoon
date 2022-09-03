#include "common_types.h"

#include <stdbool.h>
#include "polygeo.h"

typedef struct tag_triangle
{
    type_point_st p1;
    type_point_st p2;
    type_point_st p3;
} type_triangle;

type_float poly_area(const type_polyline_st *const f_shape_st)
{
    type_float l_ret_f;
    return l_ret_f;
}

static type_float triangle_area(const type_triangle f_triangle_st)
{
    type_float l_ret_f;
    return l_ret_f;
}


bool poly_intersect(const type_polyline_st *const f_line1_st, const type_polyline_st *const f_line2_st, type_point_st *f_intersect_st)
{
    bool l_ret_b;
    return l_ret_b;
}

