#ifndef COLOR_SHADER_H
#define COLOR_SHADER_H

#include "matrix.h"
#include "Color.h"
#include "VertexUtils.h"

struct ColorShader{
	struct Varying{
		vec4 position;
		vec3 color;
	};
	friend Varying operator*(float t, Varying V){
		return {t*V.position, t*V.color};
	}
	friend Varying operator+(Varying U, Varying V){
		return {U.position + V.position, U.color + V.color};
	}

	mat4 M;

	template<typename Vertex>
	void vertexShader(Vertex in, Varying& out){
		out.position = M*getPosition(in);
		out.color = toVec3(in.color);
	}

	void fragmentShader(Varying V, Color& FragColor){
		FragColor = toColor(V.color);
	}
};

#endif
