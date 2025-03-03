#ifndef MODEL_H
#define MODEL_H

#include "assimp/cimport.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "shader.h"
#include <vector>

class Model
{
public:
    Model(char *path)
    {
        loadModel(path);
    }
    void draw(Shader& shader);
private:
    std::vector<Texture> loadedTextures;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, unsigned int typeLoaded);
};

#endif