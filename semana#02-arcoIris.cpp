#include "Image.h"
#include "math.h"
#include "vec.h"
#include<iostream>

int main() {
    int w = 600, h = 600;

    Image img(w, h);
    img.fill(white);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            img(x, y) = bilerp((float)x/w, (float)y/h, blue, green, orange, purple);
        }
    }

    img.savePNG("arcoIris.png");

    return 0;
}