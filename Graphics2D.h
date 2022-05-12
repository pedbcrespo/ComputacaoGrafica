#ifndef GRAPHICS2D_H
#define GRAPHICS2D_H

#include <vector>
#include "geometry.h"
#include "Image.h"
#include "VertexUtils.h"
#include "Primitives.h"
#include "rasterization.h"
#include "Clip2D.h"
	
class Graphics2D{
	Image img;

	public:
	Graphics2D(int w, int h) : img{w, h}{}

	void clear(Color col = white){
		img.fill(col);
	}
	
	void savePNG(std::string filename){
		img.savePNG(filename);	
	}
	
	template<class Prims>
	void draw(const std::vector<vec2>& V, const Prims& P, Color color){
		std::vector<Vec2Col> VC;
		for(vec2 v: V)
			VC.push_back({v, color});
		draw(VC, P);
	}

	template<class Prims>
	void draw(const std::vector<Vec2Col>& V, const Prims& P){
		ClipRectangle R = {-0.5f, -0.5f, img.width()-0.5f, img.height()-0.5f};
		for(auto primitive: clip(assemble(P, V), R))
			drawPrimitive(primitive);
	}

	void paint(Pixel p, Color c){
		if(p.x >= 0 && p.y >= 0 && p.x < img.width() && p.y < img.height())
			img(p.x,p.y) = c;
	}

	void drawPrimitive(Line<Vec2Col> line){
		vec2  L[] = { line[0].position, line[1].position };
		Color C[] = { line[0].color,    line[1].color    };

		for(Pixel p: rasterizeLine(L)){
			float t = find_mix_param(toVec2(p), L[0], L[1]);
			Color color = lerp(t, C[0], C[1]);
			paint(p, color);
		};
	}

	void drawPrimitive(Triangle<Vec2Col> tri){
		vec2 vec_0 = tri[0].position, vec_1 = tri[1].position, vec_2 = tri[2].position;
		Color cor_0 = tri[0].color, cor_1 = tri[1].color, cor_2 = tri[2].color;
        std::array<vec2,3> pontos={vec_0,vec_1,vec_2};
        for(Pixel p: rasterizeTriangle(pontos)){
            vec2 v = {(float)p.x, (float)p.y};
			vec3 pontosBaricentro = barycentric(v, pontos);
			std::array<float, 3> cobari{pontosBaricentro[0],
										pontosBaricentro[1],
										pontosBaricentro[2]};
			Color color = toColor((cobari[0]*toVec3(cor_0)) + (cobari[1]*toVec3(cor_1)) + (cobari[2]*toVec3(cor_2)));
			paint(p, color);
		}
	}
};

#endif
