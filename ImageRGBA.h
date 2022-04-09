#ifndef IMAGERGBA_H
#define IMAGERGBA_H

#include <vector>
#include <string>
#include "Color.h"

struct RGBA{
	Byte r, g, b, a;
};

inline vec4 toVec4(RGBA C){
	return {
		ByteToFloat(C.r),
		ByteToFloat(C.g),
		ByteToFloat(C.b),
		ByteToFloat(C.a),
	};
}

class ImageRGBA{
	int _width = 0, _height = 0;
	std::vector<RGBA> pixels;
	public:

	// Construtor padrão, imagem vazia
	ImageRGBA() = default;

	// Construtor para carregar um arquivo de imagem
	ImageRGBA(std::string filename);
	
	// Largura da imagem
	int width() const{ return _width; }

	// Altura da imagem
	int height() const{ return _height; }

	// Acesso a um pixel
	RGBA& operator()(int x, int y){
		return pixels[y*_width + x];
	}
};

#endif
