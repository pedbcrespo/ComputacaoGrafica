#include "ObjMesh.h"

using Vertex = ObjMesh::Vertex;

inline GLTexture init_texture(std::string image){
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

inline MaterialInfo standard_material(std::string mat_Kd){
	MaterialInfo mat;

	mat.name = "standard";

	mat.Ka = {1, 1, 1};
	mat.Kd = {1, 1, 1};
	mat.Ks = {0, 0, 0};
	mat.Ns = 1;

	mat.map_Kd = mat_Kd;

	return mat;
}

struct SurfaceMesh{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

class GLMesh{
	VAO vao;
	GLBuffer vbo;
	GLBuffer vbo_tangents;
	GLBuffer ebo;
	std::vector<MaterialRange> materials;
	std::map<std::string, GLTexture> texture_map;
	public:
	mat4 Model;

	GLMesh() = default;

	GLMesh(std::string obj_file, mat4 _Model, MaterialInfo std_mat=standard_material("")){
		ObjMesh mesh{obj_file};
		std::vector<Vertex> tris = mesh.getTriangles();
		init_buffers(tris);

		materials = mesh.getMaterials(std_mat);

		for(MaterialRange range: materials){
			load_texture(mesh.path, range.mat.map_Ka);
			load_texture(mesh.path, range.mat.map_Kd);
			load_texture(mesh.path, range.mat.map_Ks);
			load_texture(mesh.path, range.mat.map_Bump);
			if(range.mat.map_Bump != "")
				calc_tangents(tris);
		}
		
		Model = _Model;
	}
	
	GLMesh(const SurfaceMesh& surface, mat4 _Model, MaterialInfo std_mat = standard_material("")){
		Model = _Model;
		init_buffers(surface.vertices);

		ebo = GLBuffer{GL_ELEMENT_ARRAY_BUFFER};
		ebo.data(surface.indices, GL_STATIC_DRAW);

		unsigned int size = surface.indices.size();

		materials = {
			{std_mat, 0, size}
		};
		load_texture("", std_mat.map_Kd);
	}

	void draw() const{
		Uniform{"Model"} = Model;
		for(MaterialRange range: materials)
			draw(range);
	}

	private:
	void init_buffers(const std::vector<Vertex>& vertices){
		vao = VAO{true};
		glBindVertexArray(vao);

		vbo = GLBuffer{GL_ARRAY_BUFFER};
		vbo.data(vertices, GL_STATIC_DRAW);

		size_t stride = sizeof(Vertex);
		size_t offset_position = offsetof(Vertex, position);
		size_t offset_texCoords = offsetof(Vertex, texCoords);
		size_t offset_normal = offsetof(Vertex, normal);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,(void*)offset_texCoords);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,(void*)offset_normal);
	}
	
	void load_texture(std::string path, std::string file){
		if(file != "" && texture_map.find(file) == texture_map.end()){
			std::string img = path + file;
			std::cout << "read image " << img << '\n';
			texture_map[file] = init_texture(img);
		}
	}

	void select_map(std::string map_name, std::string map_id, int texture_unit) const{
		auto it = texture_map.find(map_id);
		bool has_map = it != texture_map.end();
		Uniform{"has_" + map_name} = has_map;
		if(has_map){
			Uniform{map_name} = texture_unit;
			glActiveTexture(GL_TEXTURE0 + texture_unit);
			glBindTexture(GL_TEXTURE_2D, it->second);
		}
	}

	void draw(MaterialRange range) const{
		Uniform{"Ka"} = range.mat.Ka; 
		Uniform{"Kd"} = range.mat.Kd;
		Uniform{"Ks"} = range.mat.Ks;
		Uniform{"shininess"} = range.mat.Ns;

		select_map("map_Ka", range.mat.map_Ka, 0);
		select_map("map_Kd", range.mat.map_Kd, 1);
		select_map("map_Ks", range.mat.map_Ks, 2);
		select_map("normal_map", range.mat.map_Bump, 3);

		glBindVertexArray(vao);
		if(ebo == 0)
			glDrawArrays(GL_TRIANGLES, range.first, range.count);
		else
			glDrawElements(GL_TRIANGLES, range.count, GL_UNSIGNED_INT, (void*)(range.first*sizeof(int)));
	}

	void calc_tangents(const std::vector<Vertex>& vertices){
		std::vector<vec3> tangents;
		int N = vertices.size();
		for(int i = 0; i+2<N; i+=3){
			Vertex v0 = vertices[i];
			Vertex v1 = vertices[i+1];
			Vertex v2 = vertices[i+2];
			tangents.push_back(get_tangent(v0, v1, v2));
			tangents.push_back(get_tangent(v1, v2, v0));
			tangents.push_back(get_tangent(v2, v0, v1));
		}
		
		vbo_tangents = GLBuffer{GL_ARRAY_BUFFER};
		vbo_tangents.data(tangents, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	vec3 get_tangent(Vertex v0, Vertex v1, Vertex v2){
		vec3 d1 = v1.position - v0.position;
		vec3 d2 = v2.position - v0.position;

		float du1 = v1.texCoords[0] - v0.texCoords[0];
		float du2 = v2.texCoords[0] - v0.texCoords[0];
		float dv1 = v1.texCoords[1] - v0.texCoords[1];
		float dv2 = v2.texCoords[1] - v0.texCoords[1];

		mat2 A = {
			du1, dv1,
			du2, dv2
		};
		float det = A(0,0)*A(1,1) - A(1,0)*A(0,1);
		// primeira linha da inversa de A
		float a = (1/det)*A(1,1);
		float b = (1/det)*(-A(0,1));

		return a*d1 + b*d2;
	}

};

#endif
