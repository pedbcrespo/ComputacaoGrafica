#ifndef VECCOL_H
#define VECCOL_H

#include "vec.h"
#include "Color.h"

template <class Vec>
struct PosCol{
	Vec position;
	Color color;

	friend PosCol lerp(float t, PosCol u, PosCol v){
		return {lerp(t, u.position, v.position), lerp(t, u.color, v.color)};
	}
};

using Vec2Col = PosCol<vec2>;
using Vec3Col = PosCol<vec3>;
using Vec4Col = PosCol<vec4>;

template<typename Vertex>
vec4 getPosition(Vertex in){
	return toVec4(in.position);
}

inline vec4 getPosition(vec2 v){
	return toVec4(v);
}

inline vec4 getPosition(vec3 v){
	return toVec4(v);
}

inline vec4 getPosition(vec4 v){
	return v;
}

	
#endif
