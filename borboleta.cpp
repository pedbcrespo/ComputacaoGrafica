#include "matrix.h"
#include "color.h"
#include "Graphics2D_v1.h"
#include "bezier.h"
#include "math.h"
std::vector<vec2> CP = loadCurve("borboleta.txt");
std::vector<vec2> P = sample_cubic_bezier_spline(CP, 30);

int main(){
    vec2 centro = {400, 300};

    float pi=3.14159265359;
    Graphics2D G(800, 600);
    G.clear();
    LineStrip L{P.size()};
    float i=0.0;
    mat3 T = {
        1.0, 0.0, centro[0],
        0.0, 1.0, centro[1],
        0.0, 0.0, 1.0
    };

    mat3 Ti = {
        1.0, 0.0, - centro[0],
        0.0, 1.0, - centro[1],
        0.0, 0.0, 1.0
    };

    while( i < (2*pi-(pi/6))){
        Mat<3, 3> C = {
            cos(i), -sin(i), 0,
            sin(i), cos(i), 0,
            0, 0, 1
        };
        if(i==0){
            G.draw((T*C*Ti)*P, L, red);
        } 
        else{
            G.draw((T*C*Ti)*P, L, blue);
        }

        i+=pi/6;
    }

    G.savePNG("borboleta.png");
}