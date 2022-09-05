#include <gtest.h>

extern "C"
{
#include "polygeo.c"
}

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

TEST(geo, polyintersect_middle)
{
    type_polyline_st p1 = {{0.5,0}, nullptr};
    type_polyline_st p2 = {{0.5,1}, nullptr};
    type_polyline_st p3 = {{0,0.5}, nullptr};
    type_polyline_st p4 = {{1,0.5}, nullptr};
    p1.next_pst = &p2;
    p3.next_pst = &p4;
    bool retval = false;
    type_point_st intersect;

    retval = poly_intersect_line_line(&p1, &p3, &intersect);

    EXPECT_TRUE(retval);
    EXPECT_FLOAT_EQ(intersect.x_f, 0.5);
    EXPECT_FLOAT_EQ(intersect.y_f, 0.5);
}

TEST(geo, polyintersect_lineend)
{
    type_polyline_st p1 = {{0.5,0}, nullptr};
    type_polyline_st p2 = {{0.5,1}, nullptr};
    type_polyline_st p3 = {{0,1}, nullptr};
    type_polyline_st p4 = {{1,1}, nullptr};
    p1.next_pst = &p2;
    p3.next_pst = &p4;
    bool retval = false;
    type_point_st intersect;

    retval = poly_intersect_line_line(&p1, &p3, &intersect);

    EXPECT_FALSE(retval);
    EXPECT_FLOAT_EQ(intersect.x_f, 0.5);
    EXPECT_FLOAT_EQ(intersect.y_f, 1);
}

TEST(geo, polyintersect_linestart)
{
    type_polyline_st p1 = {{0,0.5}, nullptr};
    type_polyline_st p2 = {{1,0.5}, nullptr};
    type_polyline_st p3 = {{0,0}, nullptr};
    type_polyline_st p4 = {{0,1}, nullptr};
    p1.next_pst = &p2;
    p3.next_pst = &p4;
    bool retval = false;
    type_point_st intersect;

    retval = poly_intersect_line_line(&p1, &p3, &intersect);

    EXPECT_TRUE(retval);
    EXPECT_FLOAT_EQ(intersect.x_f, 0);
    EXPECT_FLOAT_EQ(intersect.y_f, 0.5);
}

TEST(geo, polycut_diagonal)
{
    // construct a square
    type_polyline_st p1 = {{0,0}, nullptr};
    type_polyline_st p2 = {{1,0}, nullptr};
    type_polyline_st p3 = {{1,1}, nullptr};
    type_polyline_st p4 = {{0,1}, nullptr};
    p1.next_pst = &p2;
    p2.next_pst = &p3;
    p3.next_pst = &p4;
    // create a diagonal
    type_polyline_st l1 = {{0,0}, nullptr};
    type_polyline_st l2 = {{1,1}, nullptr};
    l1.next_pst = &l2;

    bool retval = false;
    type_polyline_st* newlines[2];
    retval = poly_cut_polyline(&p1, &l1, newlines);

    EXPECT_TRUE(retval);
    EXPECT_FLOAT_EQ(newlines[0]->p_st.x_f, 0);
    EXPECT_FLOAT_EQ(newlines[0]->p_st.y_f, 0);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->p_st.x_f, 1);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->p_st.y_f, 1);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->next_pst->p_st.x_f, 0);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->next_pst->p_st.y_f, 1);
    EXPECT_TRUE(newlines[0]->next_pst->next_pst->next_pst == nullptr);
    EXPECT_FLOAT_EQ(newlines[1]->p_st.x_f, 0);
    EXPECT_FLOAT_EQ(newlines[1]->p_st.y_f, 0);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->p_st.x_f, 1);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->p_st.y_f, 0);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->next_pst->p_st.x_f, 1);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->next_pst->p_st.y_f, 1);
    EXPECT_TRUE(newlines[1]->next_pst->next_pst->next_pst == nullptr);
}

TEST(geo, polycut_horizontal)
{
    // construct a square
    type_polyline_st p1 = {{0,0}, nullptr};
    type_polyline_st p2 = {{1,0}, nullptr};
    type_polyline_st p3 = {{1,1}, nullptr};
    type_polyline_st p4 = {{0,1}, nullptr};
    p1.next_pst = &p2;
    p2.next_pst = &p3;
    p3.next_pst = &p4;
    // create a diagonal
    type_polyline_st l1 = {{0,0.5}, nullptr};
    type_polyline_st l2 = {{1,0.5}, nullptr};
    l1.next_pst = &l2;

    bool retval = false;
    type_polyline_st* newlines[2];
    retval = poly_cut_polyline(&p1, &l1, newlines);

    EXPECT_TRUE(retval);
    EXPECT_FLOAT_EQ(newlines[0]->p_st.x_f, 0);
    EXPECT_FLOAT_EQ(newlines[0]->p_st.y_f, 0);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->p_st.x_f, 1);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->p_st.y_f, 0);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->next_pst->p_st.x_f, 1);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->next_pst->p_st.y_f, 0.5);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->next_pst->next_pst->p_st.x_f, 0);
    EXPECT_FLOAT_EQ(newlines[0]->next_pst->next_pst->next_pst->p_st.y_f, 0.5);
    EXPECT_TRUE(newlines[0]->next_pst->next_pst->next_pst->next_pst == nullptr);
    EXPECT_FLOAT_EQ(newlines[1]->p_st.x_f, 1);
    EXPECT_FLOAT_EQ(newlines[1]->p_st.y_f, 0.5);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->p_st.x_f, 1);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->p_st.y_f, 1);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->next_pst->p_st.x_f, 0);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->next_pst->p_st.y_f, 1);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->next_pst->next_pst->p_st.x_f, 0);
    EXPECT_FLOAT_EQ(newlines[1]->next_pst->next_pst->next_pst->p_st.y_f, 0.5);
    EXPECT_TRUE(newlines[1]->next_pst->next_pst->next_pst->next_pst == nullptr);
}