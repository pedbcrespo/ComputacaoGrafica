#ifndef SAMPLER2D_H
#define SAMPLER2D_H

#include "Image.h"

enum Filter{NEAREST, BILINEAR};
enum WrapMode{CLAMP, REPEAT, MIRRORED_REPEAT};

class Sampler2D{
	public:
	Image img;
	Filter filter;
	WrapMode wrapX, wrapY;

	Color sample(vec2 texCoords){
		float sx = texCoords[0]*img.width() - 0.5;
		float sy = texCoords[1]*img.height() - 0.5;

		if(filter == BILINEAR)
			return sampleBI(sx, sy);

		return sampleNN(sx, sy);
	}

	private:
	int limitValue(int v, int vmax, WrapMode wrap){
		if(wrap == CLAMP)
			return clamp(v, 0, vmax-1);

		int r = v%vmax;
		if(r < 0)
			r += vmax;

		if(wrap == REPEAT)
			return r;

		int n = v/vmax;
		if(v < 0)
			n--;
		
		return (n%2==0)? r: vmax-1 - r;
	}

	Color at(int x, int y){
		if(img.width() == 0 || img.height() == 0)
			return magenta;
		x = limitValue(x, img.width(), wrapX);
		y = limitValue(y, img.height(), wrapY);
		return img(x, y);
	}

	Color sampleNN(float sx, float sy){
		int x = round(sx);
		int y = round(sy);
		return at(x, y);
	}

	Color sampleBI(float sx, float sy){
		int x = floor(sx);
		int y = floor(sy);
		float u = sx - x;
		float v = sy - y;
		return bilerp(u, v, 
			at(x, y), at(x+1,y), 
			at(x, y+1), at(x+1, y+1)
		);
	}
};


#endif
