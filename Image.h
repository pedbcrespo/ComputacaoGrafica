#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include "Color.h"

class Image{
	int _width = 0, _height = 0;
	std::vector<Color> pixels;
	public:

	// Construtor padrão, imagem vazia
	Image() = default;

	// Construtor com tamanho da imagem
	Image(int w, int h):
		_width(w), _height(h), pixels(w*h)
	{}

	// Construtor para carregar um arquivo de imagem
	Image(std::string filename);

	// Largura da imagem
	int width() const{ return _width; }

	// Altura da imagem
	int height() const{ return _height; }

	// Acesso a um pixel
	Color& operator()(int x, int y){
		return pixels[y*_width + x];
	}

	// Preenche a imagem com uma cor
	void fill(Color color){
		pixels.assign(pixels.size(), color);
	}

	// Salva a imagem num arquivo PNG
	void savePNG(std::string filename);

	// Salva a imagem num arquivo BMP
	void saveBMP(std::string filename);
};


#endif
