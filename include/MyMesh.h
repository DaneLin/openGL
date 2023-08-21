#ifndef MYMESH_H
#define MYMESH_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "myShader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

class Mesh{
    public:
        //mesh data
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        //funtions
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader& shader);
    private:
        //渲染函数
        unsigned int VAO, VBO, EBO;
        //函数
        void setupMesh();
};


#endif