#include <GL/glew.h>
#include <GL/freeglut.h>
#include "transforms.h"
#include "Color.h"
#include "GLutils.h"

VAO vao;
GLBuffer vbo, ebo;
ShaderProgram shaderProgram;

void init(){
	glewInit();
	enable_debug(); 

	shaderProgram = ShaderProgram{
		Shader{"SimpleShader.vert", GL_VERTEX_SHADER},
		Shader{"SimpleShader.frag", GL_FRAGMENT_SHADER}
	};
	
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

	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(V, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	ebo = GLBuffer{GL_ELEMENT_ARRAY_BUFFER};
	ebo.data(indices, GL_STATIC_DRAW);
}
	
void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	mat4 Model = rotate_y(0.2)*rotate_x(0.1)*translate(-0.5, -0.5, -0.5);
	mat4 View = lookAt({0, 0, 2}, {0, 0, 0}, {0, 1, 0});
	mat4 Projection = frustum(-1, 1, -1, 1, 1, 10);
		
	Uniform{"M"} = Projection*View*Model;
	Uniform{"C"} = toVec3(blue);

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
