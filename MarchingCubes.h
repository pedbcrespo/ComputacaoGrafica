#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include "vec.h"
#include <functional>

struct CubeData{
	vec3 P[8];
	float V[8];
	
	CubeData(std::function<float(float, float, float)> F, vec3 p0, const Mat<3,8>& D){
		for(int vi = 0; vi < 8; vi++){
			vec3 p = p0 + D.get_column(vi);
			P[vi] = p;
			V[vi] = F(p[0], p[1], p[2]);
		}
	}

	void process(std::vector<vec3>& tri_vertices) const{
		#include "MarchingCubesTables.h"

		vec3 edge_vertex[12];
		for (int e = 0; e < 12; e++){
			int ia = indicesTable[e][0];
			int ib = indicesTable[e][1];

			float va = V[ia];
			float vb = V[ib];

			if ((va < 0) != (vb < 0))
				edge_vertex[e] = lerp(-va/(vb - va), P[ia], P[ib]);
		}

		for (int *t = triTable[cube_index()]; *t != -1; t++)
			tri_vertices.push_back(edge_vertex[*t]);
	}

	int cube_index() const{
		int r = 0;
		for (int i = 0; i < 8; i++)
			r |= (V[i] < 0) << i;
		return r;
	}
};

inline Mat<3, 8> displacements(vec3 d){
	float x = d[0], y = d[1], z = d[2];
	return {
		0, x, x, 0, 0, x, x, 0,
		0, 0, 0, 0, y, y, y, y,
		0, 0, z, z, 0, 0, z, z
	};
}

inline std::vector<vec3> marchingCubes(
	std::function<float(float, float, float)> F, 
	int nx, int ny, int nz, 
	vec3 pmin, vec3 pmax
){
	std::vector<vec3> tri_vertices;
	
	vec3 d = (pmax - pmin)/vec3{(float)nx, (float)ny, (float)nz};
	Mat<3,8> D = displacements(d);

	for(float i = 0; i < nx; i++)
	for(float j = 0; j < ny; j++)
	for(float k = 0; k < nz; k++){
		vec3 p0 = pmin + d*vec3{i, j, k};
		CubeData cube{F, p0, D};
		cube.process(tri_vertices);
	}
	
	return tri_vertices;
}

#endif
