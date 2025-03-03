#include "mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
}

void Mesh::draw(Shader &shader)
{
    int diffuseNr = 0;
    int specularNr = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        Texture texture = textures[i];

        texture.use(GL_TEXTURE0 + i);
        
        std::string property = "material.";

        if (texture.textureType == TEXTURE_DIFFUSE)
        {
            property += "diffuse";
            property += std::to_string(diffuseNr);
            diffuseNr += 1;
        } else if (texture.textureType == TEXTURE_SPECULAR)
        {
            property += "specular";
            property += std::to_string(specularNr);
            specularNr += 1;
        }

        shader.setInt(property, i);
    }

    for (unsigned int i = 0; i < colors.size(); i++)
    {
        MatColor col = colors[i];
        
        std::string property = "material.";

        if (col.type == TEXTURE_DIFFUSE)
        {
            property += "diffuse_col";
            property += std::to_string(diffuseNr);
            diffuseNr += 1;
        } else if (col.type == TEXTURE_SPECULAR)
        {
            property += "specular_col";
            property += std::to_string(specularNr);
            specularNr += 1;
        }

        shader.setBool(property + "_for_col", true);
        shader.setVec4(property, col.col);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}