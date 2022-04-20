#include "Image.h"

float G(float x, float y, float cx, float cy, float s){
 float dx = x - cx;
 float dy = y - cy;
 return 100*exp(-(dx*dx + dy*dy)/(s*s));
}

float F(float x, float y){
 return 50 - G(x, y, 300, 500, 50) -        
             G(x, y, 300, 415, 30) -         
             G(x, y, 300, 350, 70) -        
                G(x, y, 250, 285, 20) -      
                G(x, y, 240, 250, 20) -        
                G(x, y, 350, 285, 20) -        
                G(x, y, 360, 250, 20) -       
                G(x, y, 235, 425, 20) -           
                G(x, y, 200, 425, 20) -        
                G(x, y, 160, 425, 20) -          
                G(x, y, 365, 425, 20) -           
                G(x, y, 400, 425, 20) -           
                G(x, y, 440, 425, 20);            



}

int main(){
 Image img(600, 600);
 img.fill(white);
 for(int y = 0; y < 600; y++)
  for(int x = 0; x < 600; x++)
   if(F(x,y) <= 0)
    img(x,y) = black;

 img.savePNG("boneco.png");
}