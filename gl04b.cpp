#include <GL/glew.h>
#include <GL/freeglut.h>
#include "transforms.h"
#include "Color.h"
#include "GLutils.h"

struct Vec3Col{
	vec3 position;
	Color color;
};

VAO vao;
GLBuffer vbo, ebo;
ShaderProgram shaderProgram;
float angle = 0;
	
void init(){
	glewInit();
	enable_debug();

	//glEnable(GL_DEPTH_TEST);
	
	shaderProgram = ShaderProgram{
		Shader{"ColorShader.vert", GL_VERTEX_SHADER},
		Shader{"ColorShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);
	
	std::vector<Vec3Col> V = {
		{{0, 0, 0}, red},
		{{1, 0, 0}, red},
		{{1, 1, 0}, green},
		{{0, 1, 0}, green},
		{{0, 0, 1}, blue},
		{{1, 0, 1}, blue},
		{{1, 1, 1}, yellow},
		{{0, 1, 1}, yellow}
	};

	std::vector<unsigned int> indices = {
		4, 5, 7,   6, 7, 5,
		5, 1, 6,   2, 6, 1,
		6, 2, 7,   3, 7, 2
	};

	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(V, GL_STATIC_DRAW);
	
	size_t stride = sizeof(Vec3Col);
	size_t offset_position = offsetof(Vec3Col, position);
	size_t offset_color = offsetof(Vec3Col, color);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, stride,(void*)offset_color);
	
	ebo = GLBuffer{GL_ELEMENT_ARRAY_BUFFER};
	ebo.data(indices, GL_STATIC_DRAW);
	
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 Model = rotate_y(angle)*translate(-0.5, -0.5, -0.5);
	mat4 View = lookAt({1.5, 1.5, 1.5}, {0, 0, 0}, {0, 1, 0});
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float a = w/(float)h;
	mat4 Projection = perspective(60, a, 0.1, 5);

	Uniform{"M"} = Projection*View*Model;

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

int last_x;

void mouse(int button, int state, int x, int y){
    last_x = x;
}

void mouseMotion(int x, int y){
    int dx = x - last_x;
    angle += dx*0.01;
    last_x = x;
    glutPostRedisplay();
}


int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);
	glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	init();
	glutMainLoop();
}
