#version 330

uniform sampler1D TransferFunction;
uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform float     StepSize;
uniform vec2      ScreenSize;

uniform float vmin;
uniform float vmax;

in vec3 EntryPoint;
out vec4 FragColor;

vec4 transfer_function(float intensity){
	return texture(TransferFunction, intensity);
}

vec4 over(vec4 C1, vec4 C2){
	float a1 = C1.a;
	float a2 = C2.a;

	vec3 c = (1 - a2)*C1.rgb + C2.rgb;
	float a = a1 + a2 - a1*a2;

	return vec4(c, a);
}


void main(){
	vec3 exitPoint = texture(ExitPoints, gl_FragCoord.st/ScreenSize).xyz;
	vec3 dir = exitPoint - EntryPoint;
	float len = length(dir);
	if(len < StepSize)
		discard;

	vec3 deltaDir = normalize(dir)*StepSize;

	vec3 voxelCoord = EntryPoint;
	float lengthAcum = 0.0;

	FragColor = vec4(0);

	for(int i = 0; i < 1600; i++){
		float intensity =  texture(VolumeTex, voxelCoord).r;
		vec4 colorSample = transfer_function(intensity);
		colorSample.rgb *= colorSample.a;

		FragColor = over(FragColor, colorSample);

		voxelCoord += deltaDir;
		lengthAcum += StepSize;
		if (lengthAcum >= len || FragColor.a > 1)
			break;
	}
}
