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
		Triangle<vec2> T = { tri[0].position, tri[1].position, tri[2].position };
		Triangle<Color> C = { tri[0].color, tri[1].color, tri[2].color };
		
		for (Pixel p: rasterizeTriangle(T)) {
			float t1 = find_mix_param(toVec2(p), T[0], T[1]);
			Vec2Col vecCol_1;
			vecCol_1.color = lerp(t1, C[0], C[1]);
			vecCol_1.position = lerp(t1, T[0], T[1]);

			float t2 = find_mix_param(toVec2(p), T[1], T[2]);
			Vec2Col vecCol_2;
			vecCol_2.color = lerp(t2, C[1], C[2]);
			vecCol_2.position = lerp(t2, T[1], T[2]);

			float t = find_mix_param(toVec2(p), vecCol_1.position, vecCol_2.position);
			Color color = lerp(t, vecCol_1.color, vecCol_2.color);
			paint(p, color);
		}
	}
};

#endif
