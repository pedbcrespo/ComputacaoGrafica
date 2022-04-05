#include "vec.h"
#include "geometry.h"

int main(){
	vec2 P[] = {{0, 1}, {4, 2}, {2, 4}};

	float area = triArea(P[0], P[1], P[2]);
	printf("area total = %g\n", area);
	float areaR = triArea(P[2], P[1], P[0]);
	printf("area reversa = %g\n", areaR);

	vec2 Q = {3,2};
	vec3 a = barycentric(Q, P);
	print("coordenadas baricentricas", a);
	vec2 vQ = a[0]*P[0] + a[1]*P[1] + a[2]*P[2];
	print("Q", Q);
	print("vQ", vQ);
	
	vec2 R = {13,22};
	vec3 b = barycentric(R, P);
	print("coordenadas baricentricas", b);
	vec2 vR = b[0]*P[0] + b[1]*P[1] + b[2]*P[2];
	print("R", R);
	print("vR", vR);
}
