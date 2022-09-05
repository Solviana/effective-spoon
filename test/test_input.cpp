#include <gtest.h>

FILE* fptr;

#define scanf(...) fscanf(fptr, __VA_ARGS__)

extern "C"
{
#include "input.c"
}

TEST(input, readline)
{
    fptr = fopen("../../test/data/lineread.txt", "r"); // relative to build folder. sorry!
    ASSERT_TRUE(fptr != nullptr) << "Test error";

    type_polyline_st p1;
    type_polyline_st p2;
    p1.next_pst = &p2;
    p2.next_pst = nullptr;
    
    input_read_line(&p1);
    EXPECT_FLOAT_EQ(p1.p_st.x_f, 0.5);
    EXPECT_FLOAT_EQ(p1.p_st.y_f, 0);
    EXPECT_FLOAT_EQ(p2.p_st.x_f, 0.252134);
    EXPECT_FLOAT_EQ(p2.p_st.y_f, 1);

    fclose(fptr);
}

TEST(input, readpolyline)
{
    fptr = fopen("../../test/data/polylineread.txt", "r");
    ASSERT_TRUE(fptr != nullptr) << "Test error";

    type_polyline_st p1;
    type_polyline_st p2;
    type_polyline_st p3;
    p1.next_pst = &p2;
    p2.next_pst = &p3;
    p3.next_pst = nullptr;
    input_read_polyline(&p1, 3);
    EXPECT_FLOAT_EQ(p1.p_st.x_f, 1);
    EXPECT_FLOAT_EQ(p1.p_st.y_f, 10.5);
    EXPECT_FLOAT_EQ(p2.p_st.x_f, 2);
    EXPECT_FLOAT_EQ(p2.p_st.y_f, 0);
    EXPECT_FLOAT_EQ(p3.p_st.x_f, 3.5);
    EXPECT_FLOAT_EQ(p3.p_st.y_f, 4.1);
    
    fclose(fptr);
}

TEST(input, readsize)
{
    fptr = fopen("../../test/data/input.txt", "r");
    ASSERT_TRUE(fptr != nullptr) << "Test error";

    uint8_t l_lines_u8;
    uint8_t l_sides_u8;

    input_read_size(&l_sides_u8, &l_lines_u8);
    EXPECT_EQ(l_sides_u8, 4);
    EXPECT_EQ(l_lines_u8, 2);

    fclose(fptr);
}

TEST(input, readinput)
{
    fptr = fopen("../../test/data/input.txt", "r");
    ASSERT_TRUE(fptr != nullptr) << "Test error";

    type_polyline_st* l_polygon_pst;
    type_polyline_st** l_linebuffer_ppst;
    uint8_t l_lines_u8;
    l_lines_u8 = input_read(&l_linebuffer_ppst, &l_polygon_pst);

    EXPECT_EQ(l_lines_u8, 2);

    EXPECT_EQ(l_linebuffer_ppst[0]->p_st.x_f, 0);
    EXPECT_EQ(l_linebuffer_ppst[0]->p_st.y_f, 0);
    EXPECT_EQ(l_linebuffer_ppst[0]->next_pst->p_st.x_f, 1);
    EXPECT_EQ(l_linebuffer_ppst[0]->next_pst->p_st.y_f, 1);
    EXPECT_TRUE(l_linebuffer_ppst[0]->next_pst->next_pst == nullptr);
    EXPECT_EQ(l_linebuffer_ppst[1]->p_st.x_f, 0.5);
    EXPECT_EQ(l_linebuffer_ppst[1]->p_st.y_f, 0);
    EXPECT_EQ(l_linebuffer_ppst[1]->next_pst->p_st.x_f, 0.5);
    EXPECT_EQ(l_linebuffer_ppst[1]->next_pst->p_st.y_f, 1);
    EXPECT_TRUE(l_linebuffer_ppst[1]->next_pst->next_pst == nullptr);

    EXPECT_EQ(l_polygon_pst->p_st.x_f, 0);
    EXPECT_EQ(l_polygon_pst->p_st.y_f, 0);
    EXPECT_EQ(l_polygon_pst->next_pst->p_st.x_f, 1);
    EXPECT_EQ(l_polygon_pst->next_pst->p_st.y_f, 0);
    EXPECT_EQ(l_polygon_pst->next_pst->next_pst->p_st.x_f, 1);
    EXPECT_EQ(l_polygon_pst->next_pst->next_pst->p_st.y_f, 1);
    EXPECT_EQ(l_polygon_pst->next_pst->next_pst->next_pst->p_st.x_f, 0);
    EXPECT_EQ(l_polygon_pst->next_pst->next_pst->next_pst->p_st.y_f, 1);
    EXPECT_TRUE(l_polygon_pst->next_pst->next_pst->next_pst->next_pst == nullptr);

    fclose(fptr);
}
