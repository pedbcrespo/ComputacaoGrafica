#include <GL/glew.h>
#include <GL/freeglut.h>

void init(){
	glewInit();
}

void desenha(){
	glClearColor(0.8, 0.6, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Janela");
	glutDisplayFunc(desenha);
	init();
	glutMainLoop();
}
