#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <vector>

const char* source_vs =
	"#version 330\n                  "
	"layout (location=0) in vec4 v;  "
	"void main(){                    "
	"    gl_Position = v;            "
	"}                               ";

const char* source_fs =
	"#version 330\n                        "
	"out vec4 fragColor;                   "
	"void main(){ 	                       "
	"    fragColor = vec4(0, 0, 1, 1);     "
	"}                                     ";

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
}

void init_buffers(){
	struct vec2{ 
		float x, y; 
	};

	std::vector<vec2> V = {
		{-0.8, -0.8},
		{-0.4, -0.6},
		{ 0.4, -0.8},
		{ 0.8, -0.4},
		{-0.6,  0.6},
		{ 0.6,  0.0},
	};

	std::vector<unsigned int> indices = {
	    0, 4, 5, 3, 2, 1
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo_position;
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(vec2), V.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void init(){
	glewInit();

	init_shaders();
	
	init_buffers();
}

void desenha(){
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

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
