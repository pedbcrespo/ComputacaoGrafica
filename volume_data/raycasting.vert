#version 330

uniform mat4 MVP;
layout (location = 0) in vec4 VerPos;
out vec3 EntryPoint;

void main(){
    EntryPoint = VerPos.xyz;
    gl_Position = MVP*VerPos;
}
