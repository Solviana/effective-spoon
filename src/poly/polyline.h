#ifndef POLYLINE_H
#define POLYLINE_H

type_polyline_st* poly_alloc(uint8_t f_len_u8);

void poly_free(type_polyline_st* f_line_pst);

void poly_insert(type_polyline_st* f_line_pst, type_point_st f_point_st, uint32_t f_dest_u32);

#endif