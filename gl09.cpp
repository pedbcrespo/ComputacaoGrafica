#include <GL/glew.h>
#include <GL/freeglut.h>
#include "transforms.h"
#include "Color.h"
#include "GLutils.h"

VAO vao;
GLBuffer vbo, ebo;
ShaderProgram shaderProgram;
int n_verts;
float angle = 0;
GLTexture texture;

void init_surface(int m, int n){
	int N = m*n;

	float u0 = -5, u1 = 5, du = (u1-u0)/(m-1),
	      v0 = -5, v1 = 5, dv = (v1-v0)/(n-1);

	struct Vertex{
		vec3 position;
		vec2 texCoords;
	};

	std::vector<Vertex> V(N);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			float u = u0 + i*du;
			float v = v0 + j*dv;

			size_t ij = i + j*m;
			V[ij].position = {u, v, sin(u*v/4)};

			float s = i/(m-1.0);
			float t = j/(n-1.0);

			V[ij].texCoords = {s, t};
		}
	}

	std::vector<unsigned int> indices;
	for(int i = 0; i < m-1; i++){
		for(int j = 0; j < n-1; j++){	
			unsigned int ij = i + j*m;
			indices.insert(indices.end(), {
					ij, ij+1, ij+m,
					ij+m+1, ij+m, ij+1
					});
		}
	}

	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(V, GL_STATIC_DRAW);

	size_t stride = sizeof(Vertex);
	size_t offset_position = offsetof(Vertex, position);
	size_t offset_texCoords = offsetof(Vertex, texCoords);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,(void*)offset_texCoords);

	ebo = GLBuffer{GL_ELEMENT_ARRAY_BUFFER};
	ebo.data(indices, GL_STATIC_DRAW);

	n_verts = indices.size();
}

void init_texture(){
	texture = GLTexture{GL_TEXTURE_2D};
	texture.load("bob.jpg");
	
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(GLEW_EXT_texture_filter_anisotropic){
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}
}

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	shaderProgram = ShaderProgram{
		Shader{"TextureShader.vert", GL_VERTEX_SHADER},
		Shader{"TextureShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);

	init_surface(50, 50);
	
	init_texture();
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 Model = rotate_z(angle);
	mat4 View = lookAt({10, 10, 10}, {0, 0, 0}, {0, 0, 1});
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float a = w/(float)h;
	mat4 Projection = perspective(50, a, 0.1, 50);

	Uniform{"M"} = Projection*View*Model;

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, n_verts, GL_UNSIGNED_INT, 0);

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
