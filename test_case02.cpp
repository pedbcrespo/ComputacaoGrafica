#include "acutest.h"
#include "rasterization.h"
#include "Primitives.h"
#include <iostream>
#include <chrono>

bool operator==(Pixel a, Pixel b) {
    return a.x == b.x && a.y == b.y;
}

void test_general(){
    std::vector<Triangle<vec2>> Triangles = {
        {vec2{7.92, 9.54}, vec2{3.36, 6.32}, vec2{7.62, 4.92}},
        {vec2{9.4, 7.54}, vec2{14.36, 9.6}, vec2{15.82, 5.7}},
        {vec2{-7.5, 15.46}, vec2{-4.08}, vec2{20.67}},
        {vec2{-9.03, -5.04}, vec2{0.96, 3.42}, vec2{10.61, -9.52}},
    };

    for(auto T: Triangles){
        std::vector<Pixel> A = simple_rasterize_triangle(T);

        TEST_CHECK(A == scanline(T));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[2], T[0]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[0], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[0], T[2], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[0], T[2]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[1], T[0]}));
    }
}

void test_horizontal_edges(){
    std::vector<Triangle<vec2>> Triangles = {
        {vec2{1, 3}, vec2{4, 3}, vec2{2, 5}},
        {vec2{100, 300}, vec2{400, 300}, vec2{200, 500}},
        {vec2{117.53, 30.46}, vec2{20.3, 30.46}, vec2{73.3, 70.5}},
        {vec2{534.45, 300.46}, vec2{220.7, 300.46}, vec2{400.84, 205.5}},
    };

    for(auto T: Triangles){
        std::vector<Pixel> A = simple_rasterize_triangle(T);

        TEST_CHECK(A == scanline(T));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[2], T[0]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[0], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[0], T[2], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[0], T[2]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[1], T[0]}));
    }
}

void test_vertical_edges(){
    std::vector<Triangle<vec2>> Triangles = {
        {vec2{1, 5}, vec2{1, 1}, vec2{4, 3}},
        {vec2{100, 500}, vec2{100, 100}, vec2{400, 300}},
        {vec2{10, 6}, vec2{10, 14}, vec2{6, 8}},
        {vec2{14, 16}, vec2{14, 1.73}, vec2{20, 25}},
    };

    for(auto T: Triangles){
        std::vector<Pixel> A = simple_rasterize_triangle(T);

        TEST_CHECK(A == scanline(T));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[2], T[0]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[0], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[0], T[2], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[0], T[2]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[1], T[0]}));
    }
}

void test_slim(){
    std::vector<Triangle<vec2>> Triangles = {
        {vec2{2, 7}, vec2{9, 5}, vec2{7, 6}},
        {vec2{200, 700}, vec2{900, 500}, vec2{700, 600}},
        {vec2{194, 114}, vec2{400, 500}, vec2{352, 308}},
        {vec2{400.84, 100.24}, vec2{900.78, 400.21}, vec2{600.32, 300.98}},
    };

    for(auto T: Triangles){
        std::vector<Pixel> A = simple_rasterize_triangle(T);

        TEST_CHECK(A == scanline(T));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[2], T[0]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[0], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[0], T[2], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[0], T[2]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[1], T[0]}));
    }
}

void test_degenerated(){
    std::vector<Triangle<vec2>> Triangles = {
        {vec2{200, 300}, vec2{200, 300}, vec2{500, 300}},
        {vec2{300, 500}, vec2{300, 500}, vec2{300, 800}},
        {vec2{500, 500}, vec2{500, 500}, vec2{800, 800}},
        {vec2{280, 548}, vec2{280, 548}, vec2{280, 548}},
    };

    for(auto T: Triangles){
        std::vector<Pixel> A = simple_rasterize_triangle(T);

        TEST_CHECK(A == scanline(T));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[2], T[0]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[0], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[0], T[2], T[1]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[1], T[0], T[2]}));
        TEST_CHECK(A == scanline(Triangle<vec2>{T[2], T[1], T[0]}));
    }
}


void test_performance(){
    Triangle<vec2> T = {
        vec2{0, 0},
        vec2{10000, 10000},
        vec2{5000, 5500}
    };

    auto p0 = std::chrono::high_resolution_clock::now();

    std::vector<Pixel> A = simple_rasterize_triangle(T);

    auto p1 = std::chrono::high_resolution_clock::now();

    std::vector<Pixel> B = scanline(T);
    
    auto p2 = std::chrono::high_resolution_clock::now();

    TEST_CHECK((p1 - p0) > 30*(p2 - p1));
}

TEST_LIST = {
    {"casos gerais", test_general},
    {"arestas horizontais", test_horizontal_edges},
    {"arestas verticais", test_vertical_edges},
    {"triangulos finos", test_slim},
    {"triangulos degenerados", test_degenerated},
    {"performance", test_performance},
    {NULL, NULL}
};
