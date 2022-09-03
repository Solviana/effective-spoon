#ifndef POLYGEO_H
#define POLYGEO_H

type_float poly_area(const type_polyline_st* const f_shape_pst);
bool poly_intersect(const type_polyline_st* const f_line1_pst, const type_polyline_st* const f_line2_pst, type_point_st *f_intersect_pst);

#endif