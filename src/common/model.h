#ifndef SRC_COMMON_MODEL_H_
#define SRC_COMMON_MODEL_H_

#include <glad/glad.h>
#include <stb_image.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/common/mesh.h"
#include "src/common/shader.h"
#include "src/util/util.h"

class Model {
 public:
    /*  Model Data */
    // stores all the textures loaded so far,
    // optimization to make sure textures aren't loaded more than once.
    std::vector<mesh::Texture> textures_loaded;
    std::vector<mesh::Mesh> meshes;
    std::string directory;
    bool gamma_correction_;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    bool Load(std::string const &path, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(const Shader &shader);

 private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions
    // from file and stores the resulting meshes in the meshes vector.
    bool loadModel(std::string const &path);

    // processes a node in a recursive fashion.
    // Processes each individual mesh located at the node
    // and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    mesh::Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // checks all material textures of a given type
    // and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<mesh::Texture> loadMaterialTextures(
        aiMaterial *mat
        , const aiTextureType &type
        , const std::string &typeName);
};

#endif  // SRC_COMMON_MODEL_H_
