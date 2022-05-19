#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include "matrix.h"
#include "vec.h"

inline mat4 loadIdentity(){
    return Id<4>();
}

inline mat4 translate(vec3 u){
	return {
		1, 0, 0, u[0],
		0, 1, 0, u[1],
		0, 0, 1, u[2],
		0, 0, 0,    1
	};
}

inline mat4 translate(float x, float y, float z){
    return translate(vec3{x, y, z});
}

inline mat4 scale(float x, float y, float z){
	return {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	};
}

inline mat4 rotate_x(float angle){
	float c = cos(angle);
	float s = sin(angle);

	return {
		1, 0,  0, 0,
		0, c, -s, 0,
		0, s,  c, 0,
		0, 0,  0, 1
	};
}

inline mat4 rotate_y(float angle){
	float c = cos(angle);
	float s = sin(angle);

	return {
		 c, 0, s, 0,
		 0, 1, 0, 0,
		-s, 0, c, 0,
		 0, 0, 0, 1
	};
}

inline mat4 rotate_z(float angle){
	float c = cos(angle);
	float s = sin(angle);

	return {
		c, -s, 0, 0,
		s,  c, 0, 0,
		0,  0, 1, 0,
		0,  0, 0, 1
	};
}

inline mat4 rotate(vec3 n, float theta){
	vec3 vec_x={1,0,0};
    vec3 vec_y={0,1,0};
    vec3 vec_z={0,0,1};
    n=normalize(n);

    auto res_1 = (1-cos(theta))*(dot(n,vec_x)*n) +(cos(theta)*vec_x) + (sin(theta)*(cross(n,vec_x)));
    auto res_2 = (1-cos(theta))*(dot(n,vec_y)*n) +(cos(theta)*vec_y) + (sin(theta)*(cross(n,vec_y)));
    auto res_3 = (1-cos(theta))*(dot(n,vec_z)*n) +(cos(theta)*vec_z) + (sin(theta)*(cross(n,vec_z)));
	mat4 final = {
		res_1[0], res_2[0], res_3[0], 0,
        res_1[1], res_2[1], res_3[1], 0,
        res_1[2], res_2[2], res_3[2], 0,
        0,	   0,	  0,     1
    };
    return final;
}

inline mat4 lookAt(vec3 eye, vec3 center, vec3 up){
	vec3 vetorUpNormalizado = normalize(up);
	vec3 direcaoDoPontoObservado = normalize(center - eye);
    vec3 vecSperpendicular = normalize(cross(direcaoDoPontoObservado, vetorUpNormalizado));
    vec3 vecUperpendicular = cross(vecSperpendicular, direcaoDoPontoObservado);
    mat4 M1 = {
        vecSperpendicular[0], vecSperpendicular[1], vecSperpendicular[2], 0.0,
        vecUperpendicular[0], vecUperpendicular[1], vecUperpendicular[2], 0.0,
        -direcaoDoPontoObservado[0], -direcaoDoPontoObservado[1], -direcaoDoPontoObservado[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    mat4 M2 = {
        1.0,  0.0,  0.0,  -eye[0],
        0.0,  1.0,  0.0,  -eye[1],
        0.0,  0.0,  1.0,  -eye[2],
        0.0,  0.0,  0.0,   1.0
    };
	return M1*M2;
}


inline mat4 orthogonal(float l, float r, float b, float t, float n, float f){
	return {
		2/(r-l),      0,     0,      (l+r)/(l-r),
			0,  2/(t-b),     0,      (b+t)/(b-t),
			0,        0, -2/(f-n),   (n+f)/(n-f),
			0,        0,      0,               1
	};	
}

inline mat4 frustum(float l, float r, float b, float t, float n, float f){
	mat4 Persp = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0,-(n+f)/n, -f,
		0, 0, -1/n, 0
	};
	mat4 Volume = {
		2/(r-l),      0,     0,      (l+r)/(l-r),
			0,  2/(t-b),     0,      (b+t)/(b-t),
			0,        0, 2/(f-n),    (n+f)/(n-f),
			0,        0,      0,               1
	};	
	return Volume*Persp;
}

inline mat4 perspective(float fovy, float aspect, float Near, float Far){
	float rad = (M_PI*fovy)/180;
	float resultNear = Near*tan(rad/2);
	float resultTop = -resultNear;
	float resultTopAspect = resultNear*aspect;
	float resultLeft = -resultTopAspect;
	return frustum(resultLeft, resultTopAspect, resultTop, resultNear, -Near, -Far);
}

#endif