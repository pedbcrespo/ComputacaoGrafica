#include "vec.h"

int main(){
	vec2 u = {2, 1};
	vec2 v = {4, 3};

	vec2 w1 = u + v;
	vec2 w2 = 2*u - 3*v;
	vec2 w3 = u*v;
	vec2 w4 = u/v;

	vec3 m = {1, 2, 3};

	vec4 n = {1, 2, 3, 4};

	u[0] = 1;
	m[1]++;
	n[3] = n[2];

	print("u", u);
	print("v", v);
	print("w1", w1);
	print("w2", w2);
	print("m", m);
	print("n", n);

	printf("dot(u,v) = %g\n", dot(u, v));
	printf("norm(u) = %g\n", norm(u));
	print("u normalizado", normalize(u));
}