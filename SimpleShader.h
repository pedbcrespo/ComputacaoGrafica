#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include "matrix.h"
#include "Color.h"
#include "VertexUtils.h"

struct SimpleShader{
	using Varying = vec4;

	mat4 M;
	Color C;

	template<class Vertex>
	void vertexShader(Vertex in, Varying& out){
		out = M*getPosition(in);
	}

	void fragmentShader(Varying, Color& fragColor){
		fragColor = C;
	}
};

#endif
