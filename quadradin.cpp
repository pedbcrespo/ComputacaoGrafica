#include "matrix.h"
#include "color.h"
#include "Graphics2D_v1.h"
#include "bezier.h"
#include "math.h"

int main(){
    std::vector<vec2> P = {
        {1,0},
        {0,1},
        {-1,0},
        {0,-1}
    };

    vec2 centro = {300, 300};

    float pi=3.14159265359;
    Graphics2D G(600, 600);
    G.clear();
    LineLoop L{P.size()};

    Mat<3, 3> A = {
        20, 0,  0,
        0,  20, 0,
        0,  0,  1,
    };
    Mat<3, 3> B = {
        1,  0,  300,
        0,  1,  300,
        0,  0,  1,
    };
    Mat<3,3> R = {
        cos(pi/8), -sin(pi/8), 0.0,
        sin(pi/8), cos(pi/8), 0.0,
        0.0, 0.0, 1.0
    };

    float i = 0.0;

    while( i <= 2.0 * pi){
        mat3 T = {
            1.0,  0.0,  centro[0],
            0.0,  1.0,  centro[1],
            0.0,  0.0,  1.0
        };
        mat3 Ti = {
            0.0,  1.0,  -centro[1],
            1.0,  0.0,  -centro[0],
            0.0,  0.0,  1.0
        };
        Mat<3, 3> C = {
            cos(i),  -sin(i),  0,
            sin(i),  cos(i),   0,
            0,       0,        1
        };

        G.draw((T*C*Ti*R*B*A)*P, L, blue);
        i+=pi/24;
    }

    G.savePNG("quadradin.png");
}