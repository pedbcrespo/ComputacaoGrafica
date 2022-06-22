#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLutils.h"
#include "ObjMesh.h"
#include "transforms.h"

VAO vao;
GLBuffer vbo;
std::vector<MaterialRange> materials;

ShaderProgram shaderProgram;

float angle = 0;

void init_shader(){
	shaderProgram = ShaderProgram{
		Shader{"PhongShader.vert", GL_VERTEX_SHADER},
		Shader{"PhongShader.frag", GL_FRAGMENT_SHADER}
		//Shader{"GouraudShader.vert", GL_VERTEX_SHADER},
		//Shader{"ColorShader.frag", GL_FRAGMENT_SHADER}
	};
}

MaterialInfo standard_material(){
	MaterialInfo mat;

	mat.name = "standard";

	mat.Ka = {1, 1, 1};
	mat.Kd = {1, 1, 1};
	mat.Ks = {0, 0, 0};
	mat.Ns = 1;

	return mat;
}

void init_surface(){
	ObjMesh mesh{"modelos/train-toy-cartoon/train-toy-cartoon.obj"};
	auto mesh_triangles = mesh.getTriangles();
	materials = mesh.getMaterials(standard_material());
	
	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(mesh_triangles, GL_STATIC_DRAW);

	size_t stride = sizeof(ObjMesh::Vertex);
	size_t offset_position = offsetof(ObjMesh::Vertex, position);
	size_t offset_normal = offsetof(ObjMesh::Vertex, normal);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,(void*)offset_normal);
}

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	init_shader();
	init_surface();
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float a = w/(float)h;
	
	mat4 Projection = perspective(45, a, 0.1, 50);
	mat4 View = lookAt({1, 1, 1}, {0, 0.3, 0}, {0, 1, 0});
	mat4 Model = rotate_y(angle)*scale(40, 40, 40);

	glUseProgram(shaderProgram);
	Uniform{"Projection"} = Projection;
	Uniform{"View"} = View;
	Uniform{"Model"} = Model;
	
	Uniform{"light_position"} = vec4{ 0, 13, 15, 1.0};
	Uniform{"Ia"} = vec3{ 0.2, 0.2, 0.2};
	Uniform{"Id"} = vec3{ 1.0, 1.0, 1.0};
	Uniform{"Is"} = vec3{ 1.0, 1.0, 1.0};
		
	for(MaterialRange range: materials){
		Uniform{"Ka"} = range.mat.Ka; 
		Uniform{"Kd"} = range.mat.Kd;
		Uniform{"Ks"} = range.mat.Ks;
		Uniform{"shininess"} = range.mat.Ns;

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, range.first, range.count);
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
	glutInitWindowSize(800, 600);
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
