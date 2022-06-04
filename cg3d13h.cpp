#include "Graphics3D.h"
#include "TextureShader.h"
#include "ObjMesh.h"
#include "transforms.h"

class Mesh{
	std::vector<ObjMesh::Vertex> tris;
	std::vector<MaterialRange> materials;
	std::map<std::string, Image> texture_map;
	public:
	mat4 Model;

	Mesh(std::string obj_file, mat4 _Model, std::string default_texture = ""){
		ObjMesh mesh{obj_file};
		tris = mesh.getTriangles();

		MaterialInfo std_mat;
		std_mat.map_Kd = default_texture;

		materials = mesh.getMaterials(std_mat);

		for(MaterialRange range: materials)
			load_texture(mesh.path, range.mat.map_Kd);

		Model = _Model;
	}
	
	void load_texture(std::string path, std::string file){			
		if(file != "" && texture_map.find(file) == texture_map.end()){
			std::string img = path + file;
			std::cout << "read image " << img << '\n';
			texture_map[file] = Image{img};
		}
	}

	void get_texture(std::string file, Image& img) const{
		auto it = texture_map.find(file);
		img = (it != texture_map.end())? it->second: Image{};
	}

	void draw(Graphics3D& G, TextureShader& shader) const{
		for(MaterialRange range: materials){
			get_texture(range.mat.map_Kd, shader.texture.img);
			TrianglesRange T{range.first, range.count};
			G.draw(tris, T, shader);
		}
	}
};

int main(){
	std::vector<Mesh> meshes;

	meshes.emplace_back(
		"models/floor/floor.obj", 
		scale(35, 35, 35), 
		"../../textures/Wood.jpg");

	// meshes.emplace_back(
	// 	"modelos/box.obj", 
	// 	translate(1, 1, -6)*rotate_y(-0.4),
	// 	"../bob.jpg");

	// meshes.emplace_back(
	// 	"modelos/metroid/DolBarriersuit.obj", 
	// 	translate(-1, 0, -1)*rotate_y(0.2)*scale(.1, .1, .1));

	// meshes.emplace_back(
	// 	"modelos/pose/pose.obj", 
	// 	translate(1, 0, 0)*scale(0.009, 0.009, 0.009));

	// meshes.emplace_back(
	// 	"modelos/House Complex/House Complex.obj", 
	// 	translate(4, 0, 0)*rotate_y(0.5*M_PI)*scale(.15, .15, .15));

	// meshes.emplace_back(
	// 	"modelos/pony-cartoon/Pony_cartoon.obj", 
	// 	translate(-2, -0.0001, -3)*scale(0.005, 0.005, 0.005),
	// 	"../../blue.png");

	TextureShader shader;
	shader.texture.filter = BILINEAR;
	shader.texture.wrapX = REPEAT;
	shader.texture.wrapY = REPEAT;

	int w = 800, h = 600;
	Graphics3D G{w, h};

	float a = w/(float)h;
	mat4 Projection = perspective(45, a, 0.1, 1000);

	vec3 p0 = {2, 1.7, 10};
	vec3 p1 = {-1, 2.7, -18};

	int nframes = 20;
	for(int k = 0; k < nframes; k++){
		G.clear({0, 165, 220});
		G.initZBuffer();

		float t = k/(nframes-1.0);
		vec3 pos = lerp(t, p0, p1);
		mat4 View = lookAt(pos, vec3{0, 1.5, 0}, {0, 1, 0});

		for(const Mesh& mesh: meshes){
			shader.M = Projection*View*mesh.Model;
			mesh.draw(G, shader);
		}

		char filename[30];
		sprintf(filename, "anim/output%03d.png", k);
		puts(filename);
		G.savePNG(filename);
	}
}
