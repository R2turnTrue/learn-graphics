#include "model.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
}

void Model::loadModel(std::string path)
{
    std::cout << "Loading Model: " << path << std::endl;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

    std::cout << "Final Meshes: " << meshes.size() << std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    std::vector<MatColor> colors;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = vec;
        } else 
        {
            vertex.uv = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        if (diffuseMaps.size() < 1)
        {
            C_STRUCT aiColor4D diffuse;
            if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
                colors.push_back(MatColor { TEXTURE_DIFFUSE, glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a) });
            else
                colors.push_back(MatColor { TEXTURE_DIFFUSE, glm::vec4(1.0) });
            
            std::cout << "Diffuse_Color - " << diffuse.r << ":" << diffuse.g << ":" << diffuse.b << ":" << diffuse.a << std::endl;
        }

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        if (specularMaps.size() < 1)
        {
            C_STRUCT aiColor4D specular;
            if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
                colors.push_back(MatColor { TEXTURE_SPECULAR, glm::vec4(specular.r, specular.g, specular.b, specular.a) });
            else
                colors.push_back(MatColor { TEXTURE_SPECULAR, glm::vec4(1.0) });
            
            std::cout << "Specular_Color - " << specular.r << ":" << specular.g << ":" << specular.b << ":" << specular.a << std::endl;
        }
    }

    std::cout << "Loaded Vertices: " << mesh->mNumVertices << std::endl;
    std::cout << "Final Vertices: " << vertices.size() << std::endl;
    
    Mesh result(vertices, indices, textures);
    result.colors = colors;
    return result;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, unsigned int typeLoaded)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;

        for (unsigned int j = 0; j < this->loadedTextures.size(); j++)
        {
            if (std::strcmp(loadedTextures[j].path.c_str(), str.C_Str()))
            {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }
        
        if (skip)
            break;

        Texture texture(str.C_Str());
        texture.textureType = typeLoaded;
        textures.push_back(texture);
    }
    return textures;
}