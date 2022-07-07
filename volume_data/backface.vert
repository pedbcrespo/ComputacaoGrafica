#version 330

uniform mat4 MVP;
layout (location = 0) in vec4 VerPos;
out vec4 Color;

void main(){
    Color = VerPos;
    gl_Position = MVP*VerPos;
}
