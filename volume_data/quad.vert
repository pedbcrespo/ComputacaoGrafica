#version 330

layout (location = 0) in vec4 Position;
out float texCoord;

void main(){
	texCoord = .5*(Position.x+1);
	gl_Position = Position;
}
