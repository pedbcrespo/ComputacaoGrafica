#ifndef TEXTURE_SHADER_H
#define TEXTURE_SHADER_H

#include "Sampler2D.h"

struct TextureShader{
	struct Varying{
		vec4 position;
		vec2 texCoords;
	};
	friend Varying operator*(float t, Varying V){
		return {t*V.position, t*V.texCoords};
	}
	friend Varying operator+(Varying U, Varying V){
		return {U.position + V.position, U.texCoords + V.texCoords};
	}

	mat4 M;
	Sampler2D texture;

	template<class Vertex>
	void vertexShader(Vertex in, Varying& out){
		out.position = M*getPosition(in);
		out.texCoords = in.texCoords;
	}

	void fragmentShader(Varying V, Color& FragColor){
		FragColor = texture.sample(V.texCoords);
	}
};

#endif
