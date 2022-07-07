#ifndef SHADERS_H
#define SHADERS_H

#include "GLutils.h"

struct Light{
    vec4 position;
    vec3 Ia;
    vec3 Id;
    vec3 Is;
};

class PhongShaderTexNormal{
    ShaderProgram shaderProgram;

    public:
    void init(){
        shaderProgram = ShaderProgram{
            Shader{"PhongShaderTexNormal.vert", GL_VERTEX_SHADER},
            Shader{"PhongShaderTexNormal.frag", GL_FRAGMENT_SHADER}
        };
    }

    void setup(mat4 Projection, mat4 View, Light light) const{
        glUseProgram(shaderProgram);

        Uniform{"Projection"} = Projection;
        Uniform{"View"} = View;

        Uniform{"light_position"} = light.position;
        Uniform{"Ia"} = light.Ia;
        Uniform{"Id"} = light.Id;
        Uniform{"Is"} = light.Is;
        
    }
};

#endif
