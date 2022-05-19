#include"transforms.h"

int main() {
    vec3 O = {2, 5, 2};
    vec3 C = {0, 0, 0};
    vec3 Up = {0, 2, 0};
    mat4 View = lookAt(O, C, Up);
    print(View);
}