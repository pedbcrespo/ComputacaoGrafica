#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLutils.h"
#include "VertexUtils.h"
#include "transforms.h"
#include "bezier.h"

VAO vao;
GLBuffer vbo;
ShaderProgram shaderProgram;
std::vector<vec2> P = loadCurve("curvaC.txt");
std::vector<vec2> Q = sample_cubic_bezier_spline(P, 20);
int n_versts = Q.size();

void init(){
	glewInit();
	
	shaderProgram = ShaderProgram{
		Shader{"SimpleShader.vert", GL_VERTEX_SHADER},
		Shader{"SimpleShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);

	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(Q, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	Uniform{"M"} = orthogonal(0, w, 0, h, -1, 1);

	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, n_versts);

	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(400, 400);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("janela");
	glutDisplayFunc(desenha);
	
	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	init();
	glutMainLoop();
}
