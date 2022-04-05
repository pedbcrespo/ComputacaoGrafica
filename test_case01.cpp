#include "acutest.h"
#include "geometry.h"
#include <iostream>

// valor pequeno para comparação de floats
const float EPS = 1E-5;

// Teste da função triArea
void test_area(){
    vec2 P[] = {{0.1, 1.78}, {4.24, 2.19}, {2.8, 4.2}};

    float area0 = triArea(P[0], P[1], P[2]);
    float area1 = triArea(P[2], P[1], P[0]);
    float area2 = triArea(P[0], P[1], P[1]);
    float area3 = triArea(P[1], P[1], P[1]);

    TEST_CHECK(fabs(area0+4.4559) < EPS);
    TEST_CHECK(area1 == -area0);
    TEST_CHECK(area2 == 0);
    TEST_CHECK(area3 == 0);
}

void test_barycentric(){
    // Triângulo definido em três formatos
    vec2 T[] = { {3.4, 2.54}, {8.46, 4.46}, {5.36, 6.6} };
    std::vector<vec2> T1 = {T[0], T[1], T[2]};
    std::array<vec2,3> T2 = {T[0], T[1], T[2]};

    // Calcula as coordenadas baricentricas de vários pontos
    vec2 ps[] = { {6.16, 4.78}, {2.52, 4.68}, {8.7, 3.16}, T[0], T[1], T[2]};
    for(vec2 p: ps){
        // Testa a função barycentric nos três formatos de triângulo
        vec3 b0 = barycentric(p, T);
        vec2 pt0 = b0[0]*T[0] + b0[1]*T[1] + b0[2]*T[2];
        
        vec3 b1 = barycentric(p, T1);
        vec2 pt1 = b1[0]*T1[0] + b1[1]*T1[1] + b1[2]*T1[2];
        
        vec3 b2 = barycentric(p, T2);
        vec2 pt2 = b2[0]*T2[0] + b2[1]*T2[1] + b2[2]*T2[2];

        // Verifica se os resultados são iguais
        TEST_CHECK(norm(pt0 - p) < EPS);
        TEST_CHECK(norm(pt1 - p) < EPS);
        TEST_CHECK(norm(pt2 - p) < EPS);
    }
}

TEST_LIST = {
    {"triArea", test_area},
    {"barycentric", test_barycentric},
    {NULL, NULL}
};