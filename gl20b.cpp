#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLutils.h"
#include "GLMesh.h"
#include "Scene.h"
#include "Shaders.h"

struct ShadowMap{
	const int SHADOW_WIDTH = 2000, SHADOW_HEIGHT = 2000;
	ShaderProgram shadowShaderProgram;
	GLTexture depthMap;
	unsigned int depthMapFBO;
	mat4 lightSpaceMatrix;

	void init(){
		//////////////////////////////////////////////////////////////////////////////////////
		// Shader
		shadowShaderProgram = ShaderProgram{
			Shader{"shadow.vert", GL_VERTEX_SHADER},
		};

		//////////////////////////////////////////////////////////////////////////////////////
		// Textura
		depthMap = GLTexture{GL_TEXTURE_2D};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = {1, 1, 1, 1};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

		//////////////////////////////////////////////////////////////////////////////////////
		// Framebuffer
		glGenFramebuffers(1, &depthMapFBO);  
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		// Associa a textura ao framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

		// Volta ao framebuffer padrão
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void setup(vec4 light_position){
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);

		float a = SHADOW_WIDTH/(float)SHADOW_HEIGHT;
		mat4 Projection = perspective(90, a, 0.1, 50);
		mat4 View = lookAt(toVec3(light_position), {0, 0, 0}, {0, 1, 0});

		glUseProgram(shadowShaderProgram);
		Uniform{"Projection"} = Projection;
		Uniform{"View"} = View;

    	mat4 Transf = translate(0.5, 0.5, 0.5)*scale(0.5, 0.5, 0.5);
		lightSpaceMatrix = Transf*Projection*View;
	}
};

class PhongShaderShadow{
	ShaderProgram shaderProgram;

	public:
	void init(){
		shaderProgram = ShaderProgram{
			Shader{"PhongShaderShadow.vert", GL_VERTEX_SHADER},
			Shader{"PhongShaderShadow1.frag", GL_FRAGMENT_SHADER}
		};
	}

	void setup(mat4 Projection, mat4 View, Light light, const ShadowMap& shadow_map){
		glUseProgram(shaderProgram);

		Uniform{"Projection"} = Projection;
		Uniform{"View"} = View;

		Uniform{"light_position"} = light.position;
		Uniform{"Ia"} = light.Ia;
		Uniform{"Id"} = light.Id;
		Uniform{"Is"} = light.Is;
		
		Uniform{"LightSpaceMatrix"} = shadow_map.lightSpaceMatrix;
		
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, shadow_map.depthMap);
		Uniform{"shadow_map"} = 5;
	}
};

PhongShaderShadow shader;
Scene scene;
ShadowMap shadow_map;
Light light;

mat4 BaseView = lookAt({0, 7, 20}, {0, 7, 0}, {0, 1, 0});
float vangle = 0;

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	enable_debug();

	shader.init();

	scene.init();

	shadow_map.init();

	light.position = vec4{0, 15, 15, 1};
	light.Ia = vec3{ 0.2, 0.2, 0.2};
	light.Id = vec3{ 1.0, 1.0, 1.0};
	light.Is = vec3{ 1.0, 1.0, 1.0};
}
	
void draw_shadow_map(){
	shadow_map.setup(light.position);
	scene.draw();
}

void draw_color_buffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, w, h);

	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float a = w/(float)h;
	mat4 Projection = perspective(45, a, 0.1, 500);
	mat4 View = rotate_x(vangle)*BaseView;

	shader.setup(Projection, View, light, shadow_map);

	scene.draw();
}

void desenha(){
	// Primeira passada: renderiza o buffer de sombra
	draw_shadow_map();

	// Segunda passada: renderiza o buffer de cor 
	draw_color_buffer();
	
	glutSwapBuffers();
}

int last_x, last_y;
void mouse(int button, int state, int x, int y){
	last_x = x;
	last_y = y;
}

void mouseMotion(int x, int y){
	int dx = x - last_x;
	int dy = y - last_y;

	vangle += 0.01*dy;
	vangle = clamp(vangle, -1.5, 1.5);
	BaseView = rotate_y(dx*0.01)*BaseView;

	last_x = x;
	last_y = y;	
	glutPostRedisplay();
}

void special(int key, int x, int y){
	vec3 dir = {0, 0, 0};
	float zmove = 0;
	float xmove = 0;

	if(key == GLUT_KEY_UP)
		zmove += 0.2;

	if(key == GLUT_KEY_DOWN)
		zmove -= 0.2;

	if(key == GLUT_KEY_LEFT)
		xmove += 0.2;

	if(key == GLUT_KEY_RIGHT)
		xmove -= 0.2;

	BaseView = translate(xmove, 0, zmove)*BaseView;
	glutPostRedisplay();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);
	
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutSpecialFunc(special);
	
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	init();
	glutMainLoop();
}
