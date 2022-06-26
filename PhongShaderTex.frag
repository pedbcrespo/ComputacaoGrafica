#version 330

uniform bool has_map_Ka;
uniform sampler2D map_Ka;

uniform bool has_map_Kd;
uniform sampler2D map_Kd;

uniform bool has_map_Ks;
uniform sampler2D map_Ks;

uniform bool has_normal_map;
uniform sampler2D normal_map;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;

uniform vec4 light_position;
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform mat4 View;
uniform mat4 Model;

in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 FragColor;

vec3 get_normal(){
	if(has_normal_map){
		vec3 norm = 2*texture(normal_map, texCoords).rgb - 1;
		mat3 NormalMatrix = transpose(inverse(mat3(View*Model)));
		return normalize(NormalMatrix*norm);
	}
	return normalize(normal);
}

void main(){     
	vec3 ka = Ka;
	vec3 kd = Kd;
	vec3 ks = Ks;

	float alpha = 1;

	if(has_map_Ka){
		vec4 col = texture(map_Ka, texCoords);
		ka = ka*col.rgb;
	}
	if(has_map_Kd){
		vec4 col = texture(map_Kd, texCoords);
		kd = kd*col.rgb;
		ka = ka*col.rgb;
		alpha = col.a;
	}
	if(has_map_Ks){
		vec4 col = texture(map_Ks, texCoords);
		ks = ks*col.rgb;
	}
	if(alpha < 0.1)
		discard;

	// direção do observador
	vec3 wr = normalize(-position); 

	vec3 N = get_normal();
	
	// troca a direção da normal caso seja uma backface
	if(!gl_FrontFacing)
		N = -N;

	// Posição da luz no referencial do observador
	vec4 lightPos = View*light_position;

	// Direção da luz
	vec3 wi = (lightPos.w == 0)?
		normalize(lightPos.xyz): // luz direcional
		normalize(lightPos.xyz - position); // luz pontual

	// direção do raio refletido
	vec3 vs = normalize(reflect(-wi, N)); 

	vec3 ambient = ka*Ia;
	vec3 diffuse = kd*Id*max(0, dot(wi, N));
	vec3 specular = ks*Is*pow(max(0,dot(vs, wr)), shininess);

	FragColor = vec4(ambient + diffuse + specular, alpha);
}
