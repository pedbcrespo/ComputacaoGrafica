#include "acutest.h"
#include "rasterization.h"
#include "Primitives.h"
#include <iostream>

void test_line_strip(){
	std::vector<vec2> P = {
		{100, 300}, {200, 300},  {100, 200},
		{300, 200}, {400, 200},	{400, 300},
		{200, 400},{300, 300},{300, 450},
		{199.5,6.30645},{145,43},{199.5,31.5263},
		{150, 250}, {250, 200}, {350, 50}
	};

    LineStrip L0{0};
    TEST_CHECK(L0.size() == 0);    
    
    LineStrip L1{1};
    TEST_CHECK(L1.size() == 0);    
    
    LineStrip L2{2};
    TEST_CHECK(L2.size() == 1);    
    
    LineStrip L3{3};
    TEST_CHECK(L3.size() == 2);    
    
    LineStrip L4{3157};
    TEST_CHECK(L4.size() == 3156);    

    LineStrip L{15};
    TEST_CHECK(L.size() == 14);

    std::vector<Line<vec2>> lines = assemble(L, P); 

    std::vector<Line<vec2>> expected = {
        {P[0], P[1]},
        {P[1], P[2]},
        {P[2], P[3]},
        {P[3], P[4]},
        {P[4], P[5]},
        {P[5], P[6]},
        {P[6], P[7]},
        {P[7], P[8]},
        {P[8], P[9]},
        {P[9], P[10]},
        {P[10], P[11]},
        {P[11], P[12]},
        {P[12], P[13]},
        {P[13], P[14]},
    };

    TEST_ASSERT(lines.size() == expected.size());

    for(unsigned int i = 0; i < expected.size(); i++){
        TEST_CHECK(norm2(lines[i][0] - expected[i][0]) < 1e-5);
        TEST_CHECK(norm2(lines[i][1] - expected[i][1]) < 1e-5);
    }
}

void test_line_loop(){
	std::vector<vec2> P = {
		{100, 300}, {200, 300},  {100, 200},
		{300, 200}, {400, 200},	{400, 300},
		{200, 400},{300, 300},{300, 450},
		{199.5,6.30645},{145,43},{199.5,31.5263},
		{150, 250}, {250, 200}, {350, 50}
	};

    LineLoop L0{0};
    TEST_CHECK(L0.size() == 0);    
    
    LineLoop L1{1};
    TEST_CHECK(L1.size() == 0);    
    
    LineLoop L2{2};
    TEST_CHECK(L2.size() == 2);    
    
    LineLoop L3{3};
    TEST_CHECK(L3.size() == 3);    
    
    LineLoop L4{3157};
    TEST_CHECK(L4.size() == 3157);

    LineLoop L{15};
    TEST_CHECK(L.size() == 15);
    
    std::vector<Line<vec2>> lines = assemble(L, P); 

    std::vector<Line<vec2>> expected = {
        {P[0], P[1]},
        {P[1], P[2]},
        {P[2], P[3]},
        {P[3], P[4]},
        {P[4], P[5]},
        {P[5], P[6]},
        {P[6], P[7]},
        {P[7], P[8]},
        {P[8], P[9]},
        {P[9], P[10]},
        {P[10], P[11]},
        {P[11], P[12]},
        {P[12], P[13]},
        {P[13], P[14]},
        {P[14], P[0]},
    };

    TEST_ASSERT(lines.size() == expected.size());

    for(unsigned int i = 0; i < expected.size(); i++){
        TEST_CHECK(norm2(lines[i][0] - expected[i][0]) < 1e-5);
        TEST_CHECK(norm2(lines[i][1] - expected[i][1]) < 1e-5);
    }
}

void test_triangle_strip(){
	std::vector<vec2> P = {
		{100, 300}, {200, 300},  {100, 200},
		{300, 200}, {400, 200},	{400, 300},
		{200, 400},{300, 300},{300, 450},
		{199.5,6.30645},{145,43},{199.5,31.5263},
		{150, 250}, {250, 200}, {350, 50}
	};

    TriangleStrip T0{0};
    TEST_CHECK(T0.size() == 0);    
    
    TriangleStrip T1{1};
    TEST_CHECK(T1.size() == 0);    
    
    TriangleStrip T2{2};
    TEST_CHECK(T2.size() == 0);
    
    TriangleStrip T3{3};
    TEST_CHECK(T3.size() == 1);    
    
    TriangleStrip T4{4};
    TEST_CHECK(T4.size() == 2);    
    
    TriangleStrip T5{5};
    TEST_CHECK(T5.size() == 3);    
    
    TriangleStrip T6{6};
    TEST_CHECK(T6.size() == 4);    
    
    TriangleStrip T7{3157};
    TEST_CHECK(T7.size() == 3155);

    TriangleStrip T{15};
    TEST_CHECK(T.size() == 13);
    
    std::vector<Triangle<vec2>> tris = assemble(T, P); 

    std::vector<Triangle<vec2>> expected = {
        {P[0], P[1], P[2]},
        {P[1], P[2], P[3]},
        {P[2], P[3], P[4]},
        {P[3], P[4], P[5]},
        {P[4], P[5], P[6]},
        {P[5], P[6], P[7]},
        {P[6], P[7], P[8]},
        {P[7], P[8], P[9]},
        {P[8], P[9], P[10]},
        {P[9], P[10], P[11]},
        {P[10], P[11], P[12]},
        {P[11], P[12], P[13]},
        {P[12], P[13], P[14]},
    };

    TEST_ASSERT(tris.size() == expected.size());

    for(unsigned int i = 0; i < expected.size(); i++){
        TEST_CHECK(norm2(tris[i][0] - expected[i][0]) < 1e-5);
        TEST_CHECK(norm2(tris[i][1] - expected[i][1]) < 1e-5);
    }
}

void test_triangle_fan(){
	std::vector<vec2> P = {
		{100, 300}, {200, 300},  {100, 200},
		{300, 200}, {400, 200},	{400, 300},
		{200, 400},{300, 300},{300, 450},
		{199.5,6.30645},{145,43},{199.5,31.5263},
		{150, 250}, {250, 200}, {350, 50}
	};

    TriangleFan T0{0};
    TEST_CHECK(T0.size() == 0);    
    
    TriangleFan T1{1};
    TEST_CHECK(T1.size() == 0);    
    
    TriangleFan T2{2};
    TEST_CHECK(T2.size() == 0);
    
    TriangleFan T3{3};
    TEST_CHECK(T3.size() == 1);    
    
    TriangleFan T4{4};
    TEST_CHECK(T4.size() == 2);
    
    TriangleFan T5{5};
    TEST_CHECK(T5.size() == 3);    
    
    TriangleFan T6{6};
    TEST_CHECK(T6.size() == 4);    
    
    TriangleFan T7{3157};
    TEST_CHECK(T7.size() == 3155);

    TriangleFan T{15};
    TEST_CHECK(T.size() == 13);
    
    std::vector<Triangle<vec2>> tris = assemble(T, P); 

    std::vector<Triangle<vec2>> expected = {
        {P[0], P[1], P[2]},
        {P[0], P[2], P[3]},
        {P[0], P[3], P[4]},
        {P[0], P[4], P[5]},
        {P[0], P[5], P[6]},
        {P[0], P[6], P[7]},
        {P[0], P[7], P[8]},
        {P[0], P[8], P[9]},
        {P[0], P[9], P[10]},
        {P[0], P[10], P[11]},
        {P[0], P[11], P[12]},
        {P[0], P[12], P[13]},
        {P[0], P[13], P[14]},
    };

    TEST_ASSERT(tris.size() == expected.size());

    for(unsigned int i = 0; i < expected.size(); i++){
        TEST_CHECK(norm2(tris[i][0] - expected[i][0]) < 1e-5);
        TEST_CHECK(norm2(tris[i][1] - expected[i][1]) < 1e-5);
    }
}


TEST_LIST = {
    {"LineStrip", test_line_strip},
    {"LineLoop", test_line_loop},
    {"TriangleStrip", test_triangle_strip},
    {"TriangleFan", test_triangle_fan},
    {NULL, NULL}
};
