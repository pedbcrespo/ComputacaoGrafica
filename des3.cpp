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
	glDrawElements(GL_LINE_STRIP, n_verts, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	shaderProgram = ShaderProgram{
		Shader{"ColorShader.vert", GL_VERTEX_SHADER},
		Shader{"ColorShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);

        std::vector<Vec2Col> V = {
        {{1,0}},
        {{0,1}},
        {{-1,0}},
        {{0,-1}},
    };

    int m=7, n=14;
	std::vector<unsigned int> indices = {
      0,3,6,9,12,2,5,8,11,1,4,7,10,0  
    };

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


void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    double t = (2*M_PI/65);
    for(int i=0; i<65; i++){
        Uniform{"M"} = rotate_z(t*i) * translate(-0.5, -0.5,0) * scale(0.05,0.05,0.05)*rotate_z(M_PI/4);
        glBindVertexArray(vao);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    }
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
