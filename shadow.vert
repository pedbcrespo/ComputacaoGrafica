#version 330

layout(location=0) in vec4 Vertex;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main(){
    gl_Position = Projection*View*Model*Vertex;
}  
