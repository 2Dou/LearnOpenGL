#ifndef SRC_COMMON_MESH_H_
#define SRC_COMMON_MESH_H_

#include <glad/glad.h>  // holds all OpenGL type declarations

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/common/shader.h"

namespace mesh {

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
 public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(const std::vector<Vertex> &vertices
        , const std::vector<unsigned int> &indices
        , const std::vector<Texture> &textures);

    // render the mesh
    void Draw(const Shader &shader);

 private:
    /*  Render data  */
    unsigned int VBO, EBO;

    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh();
};

}  // namespace mesh

#endif  // SRC_COMMON_MESH_H_
