#ifndef POLYGEO_H
#define POLYGEO_H

type_float poly_area(const type_polyline_st* const f_shape_pst);
bool poly_cut_polyline(type_polyline_st* f_polyline_pst, const type_polyline_st* const f_line_pst, type_polyline_st** f_newpolylines_ppst);
#endif