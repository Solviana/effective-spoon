#include <gtest.h>

#include "polygeo.c"

TEST(geo, trianglearea)
{
    type_triangle_st tri = {{1.0, 0}, {0, 0}, {0,1.0}};
    type_float area = triangle_area(tri);

    EXPECT_FLOAT_EQ(area, 0.5);

}

TEST(geo, polyarea)
{
    type_polyline_st p1;
    type_polyline_st p2;
    type_polyline_st p3;
    type_polyline_st p4;
    p1.next_pst = &p2;
    p2.next_pst = &p3;
    p3.next_pst = &p4;
    p4.next_pst = nullptr;
    p1.p_st = {0, 0};
    p2.p_st = {0.5, 0};
    p3.p_st = {0.5, 0.5};
    p4.p_st = {0, 0.5};
    type_float area = poly_area(&p1);

    EXPECT_FLOAT_EQ(area, 0.25);

}
