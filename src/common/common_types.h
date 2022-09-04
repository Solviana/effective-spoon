#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stddef.h> //for NULL
#include <stdint.h>
#include <stdbool.h>

typedef float type_float;

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

#endif
