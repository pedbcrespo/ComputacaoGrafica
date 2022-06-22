#version 330

uniform mat4 M;
uniform vec3 pmin, pmax;
uniform vec3 C[8];

layout (location = 0) in vec4 pos;

out vec3 v_color;

vec3 bilerp(float s, float t, vec3 A, vec3 B, vec3 C, vec3 D){
	vec3 E = mix(A, B, s);
	vec3 F = mix(C, D, s);
	return mix(E, F, t);
}

void main(){
	gl_Position = M*pos;

	float dx = (pos.x-pmin.x)/(pmax.x - pmin.x);
	float dy = (pos.y-pmin.y)/(pmax.y - pmin.y);
	float dz = (pos.z-pmin.z)/(pmax.z - pmin.z);
	vec3 C1 = bilerp(dx, dz, C[0], C[1], C[2], C[3]);
	vec3 C2 = bilerp(dx, dz, C[4], C[5], C[6], C[7]);
	v_color = mix(C1, C2, dy);
}
