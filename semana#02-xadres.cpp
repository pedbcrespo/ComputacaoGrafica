#include <iostream>
#include "vec.h"
#include "Image.h"

using namespace std;

int main(){
  Image img(600, 600);
  img.fill(white);
  /*Para verificar linha e coluna do tabuleiro (EM GERAL, porque mais à frente estarei dividindo o tabuleiro em sub seções)*/
  int contY = 0;
  int contX = 0;
  /*Percorre tabuleiro na direção Y*/
  for (int linha = 0; linha < 600; linha += (600) / 8){
    /*Percorre tabuleiro na direção X*/
    for (int coluna = 0; coluna < 600; coluna += (600) / 8){
      if ((contY % 2 == 0 && contX % 2 == 0) || (contY % 2 != 0 && contX % 2 != 0)){
        /* Pinta quadrado de preto enquanto não chega na borda */
        for (int Ypixel = linha; Ypixel < (600 / 8) + linha && (Ypixel < 600); Ypixel++){
          for (int Xpixel = coluna; Xpixel < (coluna + (600 / 8)) && (Xpixel < 600); Xpixel++){
            img(Xpixel, Ypixel) = black;
          }
        }
      }
      contX++;
    }
    contY++;
  }
  img.savePNG("xadres.png");
  return 0; 
}