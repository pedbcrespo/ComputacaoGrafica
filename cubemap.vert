#version 330
layout (location = 0) in vec4 Vertex;

out vec3 TexCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main(){
    vec4 pos = Model*Vertex;
    TexCoords = pos.xyz;
    gl_Position = Projection*View*pos;
} 
