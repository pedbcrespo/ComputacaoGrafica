#include "Image.h"
#include "math.h"
#include "vec.h"
#include<iostream>

float norma(vec2 vetor){
    return sqrt(pow(vetor[0],2) + pow(vetor[1],2));
}

int tem_mesmo_raio(vec2 centro, float raio, int x, int y){
    vec2 p = {centro[0] - x, centro[1] - y};
    float norma_p = norma(p);
    return norma_p <= raio;
}

int main(){
	Image img(600, 600);
	img.fill(white);
    vec2 centro = {300, 300};
    float raio = 420;
    int num_circulo = 0;
    while(num_circulo < 15){
        for(int y = 0; y < 600 ; y++){
            for(int x = 0; x < 600; x++){
                if(tem_mesmo_raio(centro, raio, x, y)){
                    if(num_circulo%2 == 0)
                        img(x,y) = black;
                    else
                        img(x,y) = white;  
                }
            }
        }
        num_circulo += 1;
        raio -= 60;
    }
	img.savePNG("alvo.png");
}
