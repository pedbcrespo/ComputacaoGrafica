#version 330

in float texCoord;
uniform sampler1D transferFunction;

out vec4 fragColor;

void main(){
	fragColor = texture(transferFunction, texCoord);	
	fragColor.a = 1;
}
