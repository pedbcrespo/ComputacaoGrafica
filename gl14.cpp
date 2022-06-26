#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLutils.h"
#include "transforms.h"

VAO vao;
GLBuffer vbo;

GLTexture diffuse_map;
GLTexture specular_map;
ShaderProgram shaderProgram;
mat4 Model = loadIdentity();

mat4 R;

GLTexture init_texture(std::string image){
	GLTexture texture{GL_TEXTURE_2D};
	texture.load(image);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(GLEW_EXT_texture_filter_anisotropic){
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}
	return texture;
}

void init(){
	glewInit();
	enable_debug();
	glEnable(GL_DEPTH_TEST);
	
	shaderProgram = ShaderProgram{
		Shader{"PhongShaderTex.vert", GL_VERTEX_SHADER},
		Shader{"PhongShaderTex.frag", GL_FRAGMENT_SHADER}
	};
	struct Vertex{
		vec2 position;
		vec2 texCoords;
		vec3 normal;
	};

	std::vector<Vertex> V{
		{{-5, -5}, {0, 0}, {0, 0, 1}},
		{{ 5, -5}, {1, 0}, {0, 0, 1}},
		{{ 5,  5}, {1, 1}, {0, 0, 1}},
		{{-5,  5}, {0, 1}, {0, 0, 1}}
	};
	
	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(V, GL_STATIC_DRAW);
	
	size_t stride = sizeof(Vertex);
	size_t offset_position = offsetof(Vertex, position);
	size_t offset_texCoords = offsetof(Vertex, texCoords);
	size_t offset_normal = offsetof(Vertex, normal);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset_texCoords);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_normal);
	
	diffuse_map = init_texture("diffuse.png");
	specular_map = init_texture("specular.png");

}

void draw(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 View = lookAt({0, 0, 15}, {0, 0, 0}, {0, 1, 0});
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float a = w/(float)h;
	mat4 Projection = perspective(50, a, 0.1, 50);

	glUseProgram(shaderProgram);
	
	Uniform{"Ka"} = vec3{1, 1, 1};
	Uniform{"Kd"} = vec3{1, 1, 1};
	Uniform{"Ks"} = vec3{1, 1, 1};
	Uniform{"shininess"} = 40.0f;

	Uniform{"light_position"} = vec4{ 0.0, 0.0, 10.0, 1.0 };
	Uniform{"Ia"} = vec3{ 0.1, 0.1, 0.1};
	Uniform{"Id"} = vec3{ 0.8, 0.8, 0.8};
	Uniform{"Is"} = vec3{ 1.0, 1.0, 1.0};

	Uniform{"Projection"} = Projection;
	Uniform{"View"} = View;
	Uniform{"Model"} = Model;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_map);
	Uniform{"map_Kd"} = 0;
	Uniform{"has_map_Kd"} = true;
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_map);
	Uniform{"map_Ks"} = 1;
	Uniform{"has_map_Ks"} = true;

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glutSwapBuffers();
}

int last_x, last_y;

void mouse(int button, int state, int x, int y){
	last_x = x;
	last_y = y;
}

void mouseMotion(int x, int y){
	int dx = x - last_x;
	int dy = y - last_y;
	
	Model = rotate_y(dx*0.01)*rotate_x(dy*0.01)*Model;

	last_x = x;
	last_y = y;
	glutPostRedisplay();
}


int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("janela");
	glutDisplayFunc(draw);
	
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	printf("GL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	init();
	glutMainLoop();
}
