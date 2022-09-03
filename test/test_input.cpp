#include <gtest.h>

FILE* fptr;

#define scanf(...) fscanf(fptr, __VA_ARGS__)

#include "input.c"

TEST(input, lineread)
{
    fptr = fopen("../test/data/lineread.txt", "r"); // relative to build folder. sorry!
    ASSERT_TRUE(fptr != nullptr) << "Test error";

    type_polyline_st p1;
    type_polyline_st p2;
    p1.next_pst = &p2;
    p2.next_pst = nullptr;
    
    read_line(&p1);
    EXPECT_FLOAT_EQ(p1.p_st.x_f, 0.5);
    EXPECT_FLOAT_EQ(p1.p_st.y_f, 0);
    EXPECT_FLOAT_EQ(p2.p_st.x_f, 0.252);
    EXPECT_FLOAT_EQ(p2.p_st.y_f, 1);

    fclose(fptr);
}

TEST(input, polylineread)
{
    fptr = fopen("../test/data/polylineread.txt", "r");
    ASSERT_TRUE(fptr != nullptr) << "Test error";

    type_polyline_st p1;
    type_polyline_st p2;
    type_polyline_st p3;
    p1.next_pst = &p2;
    p2.next_pst = &p3;
    p3.next_pst = nullptr;
    read_polyline(&p1, 3);
    EXPECT_FLOAT_EQ(p1.p_st.x_f, 1);
    EXPECT_FLOAT_EQ(p1.p_st.y_f, 10.5);
    EXPECT_FLOAT_EQ(p2.p_st.x_f, 2);
    EXPECT_FLOAT_EQ(p2.p_st.y_f, 0);
    EXPECT_FLOAT_EQ(p3.p_st.x_f, 3.5);
    EXPECT_FLOAT_EQ(p3.p_st.y_f, 4.1);
    
    fclose(fptr);
}