#ifndef POLYGEO_H
#define POLYGEO_H

type_float poly_area(const type_polyline_st *const f_shape_st);
bool poly_intersect(const type_polyline_st *const f_line1_st, const type_polyline_st *const f_line2_st, type_point_st *f_intersect_st);

#endif