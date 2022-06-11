#include <GL/glew.h>
#include <GL/freeglut.h>
#include "matrix.h"
#include "transforms.h"

const char* source_vs =
	"#version 330\n                 "
	"uniform mat4 M;                "
	"layout (location=0) in vec4 v; "
	"void main(){                   "
	"    gl_Position = M*v;         "
	"}                              ";

const char* source_fs =
	"#version 330\n                  "
	"uniform vec3 C;                 "
	"out vec4 fragColor;             "
	"void main(){ 	                 "
	"    fragColor = vec4(C, 1);     "
	"}                               ";

unsigned int shaderProgram;
unsigned int vao;

void init_shaders(){
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source_vs, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &source_fs, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
}

void init_buffers(){
	std::vector<vec3> V = {
		{0, 0, 0},
		{1, 0, 0},
		{1, 1, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 1},
		{1, 1, 1},
		{0, 1, 1},
	};
	std::vector<unsigned int> indices = {
		0, 1,	1, 2,	2, 3,	3, 0,	4, 5,	5, 6,
		6, 7,	7, 4,	0, 4,	1, 5,	2, 6,	3, 7
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo_position;
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, V.size()*sizeof(vec3), V.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_STATIC_DRAW);
}

void init(){
	glewInit();

	init_shaders();
	
	init_buffers();
}

void setup_uniforms(){
	mat4 Model = rotate_y(0.2)*rotate_x(0.1)*translate(-0.5, -0.5, -0.5);
	mat4 View = lookAt({0, 0, 2}, {0, 0, 0}, {0, 1, 0});
	mat4 Projection = frustum(-1, 1, -1, 1, 1, 10);

	mat4 M = Projection*View*Model;
	int loc = glGetUniformLocation(shaderProgram, "M");
	glUniformMatrix4fv(loc, 1, true, M.v);

	vec3 C = {0, 0, 1};
	loc = glGetUniformLocation(shaderProgram, "C");
	glUniform3fv(loc, 1, C.v);
}

void desenha(){
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	setup_uniforms();

	glBindVertexArray(vao);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(500, 500);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);

	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	init();
	glutMainLoop();
}
