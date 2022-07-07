#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GLMesh.h"
#include "transforms.h"

class Scene{
    public:
    std::vector<GLMesh> meshes;

    void init(){
        meshes.emplace_back(
            flag_mesh(50, 50), 
            translate(0, 8, -10)*scale(1.4282, 1, 1), 
            standard_material("brasil.png")
        );

        meshes.emplace_back(
            "modelos/bunny.obj", 
            translate(0, 5.2, 2), 
            standard_material("../blue.png")
        ); 

        meshes.emplace_back(
            "modelos/monkey.obj", 
            translate(0, 5.6, -2)*scale(1.4, 1.4, 1.4)*rotate_x(-0.7)
        );

        meshes.emplace_back(
            "modelos/teapot.obj", 
            translate(6,0,4)*scale(.14,.14,.14)*rotate_x(-M_PI/2), 
            standard_material("../bob.jpg")
        );

        MaterialInfo brickwall_material = standard_material("../brickwall.jpg");
        brickwall_material.map_Bump = "../brickwall_normal.jpg";

        meshes.emplace_back(
            "modelos/wall.obj", 
            scale(20, 20, 20), 
            brickwall_material
        );

        meshes.emplace_back(
            "modelos/Wood Table/Old Wood Table.obj", 
            translate(0,1.08,0)
        );

        meshes.emplace_back(
            "modelos/pose/pose.obj", 
            translate(-6, 0, 4)*rotate_y(1)*scale(.05, .05, .05)
        );

        meshes.emplace_back(
            "modelos/train-toy-cartoon/train-toy-cartoon.obj", 
            translate(0,0,6)*rotate_y(-2.3)*scale(120, 120, 120)
        );
    }

    void draw() const{
        for(const GLMesh& mesh: meshes)
            mesh.draw();
    }

    private:
    SurfaceMesh flag_mesh(int m, int n) const{
        int N = m*n;

        float u0 = -5, u1 = 5, du = (u1-u0)/(m-1),
            v0 = -5, v1 = 5, dv = (v1-v0)/(n-1);

        SurfaceMesh res;
        res.vertices.resize(N);
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                float u = u0 + i*du;
                float v = v0 + j*dv;

                Vertex& V = res.vertices[i + j*m];
                V.position = {u, v, sin(u*v/4)};

                float s = i/(m-1.0);
                float t = j/(n-1.0);

                V.texCoords = {s, t};

                vec3 du = {1, 0, cos(u*v/4)*v/4};
                vec3 dv = {0, 1, cos(u*v/4)*u/4};
                V.normal = normalize(cross(du, dv));
            }
        }

        for(int i = 0; i < m-1; i++){
            for(int j = 0; j < n-1; j++){	
                unsigned int ij = i + j*m;
                res.indices.insert(res.indices.end(), {
                        ij, ij+1, ij+m,
                        ij+m+1, ij+m, ij+1
                        });
            }
        }

        return res;
    }
};

#endif
