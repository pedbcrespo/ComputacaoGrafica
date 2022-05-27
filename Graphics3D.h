#ifndef GRAPHICS3D_H
#define GRAPHICS3D_H

#include <vector>
#include "geometry.h"
#include "Image.h"
#include "Primitives.h"
#include "rasterization.h"
#include "Clip3D.h"

class Graphics3D{
	Image img;
	std::vector<float> ZBuffer;

	public:
	Graphics3D(int w, int h) : img{w, h}{}

	void clear(Color col = white){
		img.fill(col);
	}
	
	void savePNG(std::string filename){
		img.savePNG(filename);	
	}
	void saveBMP(std::string filename){
		img.saveBMP(filename);	
	}

	void initZBuffer(){
		ZBuffer.assign(img.width()*img.height(), 1.0f);
	}

	template<class Vertex, class Prims, class Shader>
	void draw(const std::vector<Vertex>& V, const Prims& p, Shader& shader){
		// Pipeline de renderização
		draw(clip(assemble(p, transform(V, shader))), shader);
	}
		
	private:
	template<class Vertex, class Shader, class Varying=typename Shader::Varying>
	std::vector<Varying> transform(const std::vector<Vertex>& V, Shader& shader){
		std::vector<Varying> PV(V.size());
		for(unsigned int i = 0; i < V.size(); i++)
			shader.vertexShader(V[i], PV[i]);
		return PV;
	}

	template<class Primitive, class Shader>
	void draw(const std::vector<Primitive>& primitives, Shader& shader){
		for(auto primitive: primitives)
			drawPrimitive(primitive, shader);
	}

	vec2 toScreen(vec4 P){
		float x = P[0];
		float y = P[1];
		float w = P[3];
		return {
			((x/w + 1)*img.width() - 1)/2,
			((y/w + 1)*img.height() - 1)/2
		};
	}
	
	float& zbuffer(int x, int y){
		return ZBuffer[y*img.width() + x];
	}

	bool ZBufferTest(int x, int y, float z){
		if(ZBuffer.empty())
			return true;

		float& zb = zbuffer(x, y);
		if(z < zb){
			zb = z;
			return true;
		}

		return false;
	}
	template<class Varying, class Shader>
	void paint(Pixel p, Varying v, Shader& shader){
		if(p.x < 0 || p.y < 0 || p.x >= img.width() || p.y >= img.height())
			return;
		
		vec4 pos = getPosition(v);
		float z = pos[2]/pos[3];
		if(ZBufferTest(p.x, p.y, z))
			shader.fragmentShader(v, img(p.x,p.y));
	}
	
	template<class Varying, class Shader> 
	void drawPrimitive(Line<Varying> line, Shader& shader){
		vec4 P[] = { getPosition(line[0]), getPosition(line[1]) };
		vec2 L[] = { toScreen(P[0]), toScreen(P[1]) };
			
		for(Pixel p: rasterizeLine(L)){
			float t = find_mix_param(toVec2(p), L[0], L[1]);
			Varying vi = (1-t)*line[0] + t*line[1];
			paint(p, vi, shader);
		}
	}

	template<class Varying, class Shader>
	void drawPrimitive(Triangle<Varying> tri, Shader& shader){
		vec4 P[] = { getPosition(tri[0]), getPosition(tri[1]), getPosition(tri[2]) };
		vec2 T[] = { toScreen(P[0]), toScreen(P[1]), toScreen(P[2]) };

		for(Pixel p: rasterizeTriangle(T)){
			vec3 t = barycentric(toVec2(p), T);
			Varying vi = t[0]*tri[0] + t[1]*tri[1] + t[2]*tri[2];
			paint(p, vi, shader);
		}
	}
};

#endif
