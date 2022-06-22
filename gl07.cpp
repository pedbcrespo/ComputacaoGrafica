#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLutils.h"
#include "VertexUtils.h"
#include "transforms.h"

VAO vao;
GLBuffer vbo;
ShaderProgram shaderProgram;
	
void init(){
	glewInit();
	
	shaderProgram = ShaderProgram{
		Shader{"ColorShader.vert", GL_VERTEX_SHADER},
		Shader{"ColorShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);

	std::vector<Vec2Col> P = {
		{{36, 116}, red}, {{108, 90}, green}, {{103, 164}, blue},
		{{-34, -37}, red}, {{32, 78}, green}, {{127, 26}, blue},
		{{145, 43}, red}, {{278, 15}, green}, {{125, -82}, blue}, 
		{{256, 172}, red}, {{271, 69}, green}, {{149, 118}, blue}, 
		{{185, 213}, red}, {{247, 254}, green}, {{229, 188}, blue},
	};

	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(P, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
			sizeof(Vec2Col), (void*)offsetof(Vec2Col, position));
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE,
			sizeof(Vec2Col), (void*)offsetof(Vec2Col, color));
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	Uniform{"M"} = orthogonal(0, w, 0, h, -1, 1);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 15);

	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(200, 200);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);
	
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	init();
	glutMainLoop();
}
