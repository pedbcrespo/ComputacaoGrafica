#version 330

uniform mat4 M;

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec3 in_color;

out vec3 v_color;

void main(){
	gl_Position = M*in_position;
	v_color = in_color;
}
