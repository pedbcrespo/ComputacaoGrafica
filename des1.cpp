#include <GL/glew.h>
#include <GL/freeglut.h>
#include "transforms.h"
#include "Color.h"
#include "GLutils.h"
#include "VertexUtils.h"

std::vector<vec2> CP = loadCurve("borboleta.txt");
VAO vao;
GLBuffer vbo, ebo;
ShaderProgram shaderProgram;
int n_verts;
float angle = 0;
	
void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	shaderProgram = ShaderProgram{
		Shader{"ColorShader.vert", GL_VERTEX_SHADER},
		Shader{"ColorShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);
}

void etapaBaseDesenho(){
    mat4 Model = rotate_z(angle);
	mat4 View = lookAt({10, 10, 10}, {0, 0, 0}, {0, 0, 1});
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float a = (float)w/(float)h;
	mat4 Projection = scale(5,5,0)*perspective(50, a, 0.1, 50);

	Uniform{"M"} = Projection*View*Model;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(vao);
	glDrawElements(GL_LINES, n_verts, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<Vec2Col> V = {
        {0.21, 1.17},
        {0.42, 1.17},
        {0.63, 1.17},
        {0.84, 1.17},
        {1.05, 1.17},
        {1.26, 1.17},
        {1.47, 1.17},
        {0.21, 0.17},
        {0.42, 0.17},
        {0.63, 0.17},
        {0.84, 0.17},
        {1.05, 0.17},
        {1.26, 0.17},
        {1.47, 0.17},
    };

    int m=7, n=14;
	std::vector<unsigned int> indices;
	for(int i = 0; i <= 6; i++){
		for(int j = 7; j < 14; j++){	
            indices.push_back(i);
            indices.push_back(j);
		}
	}

	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(V, GL_STATIC_DRAW);
	
	size_t stride = sizeof(Vec2Col);
	size_t offset_position = offsetof(Vec2Col, position);
	size_t offset_color = offsetof(Vec2Col, color);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, stride,(void*)offset_color);
	ebo = GLBuffer{GL_ELEMENT_ARRAY_BUFFER};
	ebo.data(indices, GL_STATIC_DRAW);
	n_verts = indices.size();
    etapaBaseDesenho();
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
