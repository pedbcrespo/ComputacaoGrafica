#ifndef CLIP2D_H
#define CLIP2D_H

#include "Primitives.h"
#include "VertexUtils.h"

struct Semiplane{
	vec2 A;
	vec2 n;
	bool has(vec2 P) const{
		return dot(P-A, n) >= 0;
	}
	float intersect(vec2 P, vec2 Q) const{
		return dot(A-P, n)/dot(Q-P,n);
	}
};

struct ClipRectangle{
	float x0, y0, x1, y1;

	std::array<Semiplane, 4> sides() const{
		return {
			Semiplane{ {x0, y0}, { 1,  0} }, // left
			Semiplane{ {x1, y0}, {-1,  0} }, // right
			Semiplane{ {x0, y0}, { 0,  1} }, // down
			Semiplane{ {x1, y1}, { 0, -1} }, // up
		};
	}
};


bool clip(Line<Vec2Col>& line, ClipRectangle rect){
	float maxIn = 0;
	float minOut = 1;

 	vec2 A = line[0].position;
    vec2 B = line[1].position;

	for (Semiplane s: rect.sides()) {

		if(!s.has(A) && !s.has(B)) {
            return false;
        }

		if(s.has(A) && s.has(B)) {
            continue;
        }

		float t = s.intersect(A,B);

		if(s.has(A) && !s.has(B)) {
           minOut = std::min(minOut, t);
        }

		if(!s.has(A) && s.has(B)) {
           maxIn = std::max(maxIn, t);
        }

		if (maxIn > minOut) {
			return false;
		}

		// vec2 a = (1 - maxIn)*A + maxIn*B;
        // vec2 b = (1 - minOut)*A + minOut*B;

		vec2 a = lerp(maxIn, A, B);
		vec2 b = lerp(minOut, A, B);

		Color c1 = toColor((1 - maxIn)*toVec3(line[0].color) + maxIn*toVec3(line[1].color));
		Color c2 = toColor((1 - minOut)*toVec3(line[0].color) + minOut*toVec3(line[1].color));

		line[0] = {{a}, {c1}};
		line[1] = {{b}, {c2}};

		// Vec2Col a = lerp(maxIn, line[0], line[1]);
		// Vec2Col b = lerp(minOut, line[0], line[1]);

		// line[0] = a;
		// line[1] = b;
	}

	return true;
}

std::vector<Line<Vec2Col>> clip(const std::vector<Line<Vec2Col>>& lines, ClipRectangle rect){
	std::vector<Line<Vec2Col>> res;
	for(Line<Vec2Col> line: lines)
		if(clip(line, rect))
			res.push_back(line);
	return res;
}

std::vector<Vec2Col> clip(const std::vector<Vec2Col>& polygon, Semiplane S){
	std::vector<Vec2Col> R;

	for(unsigned int i = 0; i < polygon.size(); i++){
		Vec2Col P = polygon[i];
		Vec2Col Q = polygon[(i+1)%polygon.size()];
	
		bool Pin = S.has(P.position);
		bool Qin = S.has(Q.position);

		if(Pin != Qin)
			R.push_back(lerp(S.intersect(P.position, Q.position), P, Q));
		
		if(Qin)
			R.push_back(Q);
	}
	return R;
}


std::vector<Vec2Col> clip(const std::vector<Vec2Col>& polygon, ClipRectangle rect){
	std::vector<Vec2Col> R = polygon;

	for(Semiplane S: rect.sides())
		R = clip(R, S);

	return R;
}

std::vector<Triangle<Vec2Col>> clip(const std::vector<Triangle<Vec2Col>>& tris, ClipRectangle R){
	std::vector<Triangle<Vec2Col>> res;

	for(Triangle<Vec2Col> tri: tris){
		std::vector<Vec2Col> polygon = {tri[0], tri[1], tri[2]};
		polygon = clip(polygon, R);
		TriangleFan T{polygon.size()};
		for(Triangle<Vec2Col> tri: assemble(T, polygon))
			res.push_back(tri);
	}

	return res;
}

#endif
