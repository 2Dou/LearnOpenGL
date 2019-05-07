#include "src/common/model.h"

void Model::Draw(const Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

bool Model::Load(std::string const &path, bool gamma /*= false*/) {
    gamma_correction_ = gamma;
    return loadModel(path);
}

bool Model::loadModel(std::string const &path) {
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path
        , aiProcess_Triangulate
            | aiProcess_FlipUVs
            | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
        || !scene->mRootNode) {  // if is Not Zero
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString()
                    << std::endl;
        return false;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    return true;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices
        // to index the actual objects in the scene.
        // the scene contains all the data,
        // node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes
    // (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}


mesh::Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<mesh::Texture> textures;

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        mesh::Vertex vertex;

        // positions
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        // normals
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;

        // texture coordinates
        // does the mesh contain texture coordinates?
        if (mesh->mTextureCoords[0]) {
            // a vertex can contain up to 8 different texture coordinates.
            // We thus make the assumption that we won't
            // use models where a vertex can have multiple
            // texture coordinates so we always take the first set (0).
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.TexCoords.x = vertex.TexCoords.y = 0.0f;
        }
        // tangent
        if (mesh->mTangents) {
            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;
        }
        // bitangent
        if (mesh->mBitangents) {
            vertex.Bitangent.x = mesh->mBitangents[i].x;
            vertex.Bitangent.y = mesh->mBitangents[i].y;
            vertex.Bitangent.z = mesh->mBitangents[i].z;
        }
        vertices.push_back(vertex);
    }

    // now walk through each of the mesh's faces
    // (a face is a mesh its triangle)
    // and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face
        // and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders.
    // Each diffuse texture should be named
    //
    // as 'texture_diffuseN'
    // where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    //
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<mesh::Texture> diffuseMaps
        = loadMaterialTextures(material
            , aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<mesh::Texture> specularMaps
        = loadMaterialTextures(material
            , aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. normal maps
    std::vector<mesh::Texture> normalMaps
        = loadMaterialTextures(material
            , aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. height maps
    std::vector<mesh::Texture> heightMaps
        = loadMaterialTextures(material
            , aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    std::vector<mesh::Texture> reflectionMap
        = loadMaterialTextures(material
            , aiTextureType_AMBIENT, "texture_ambient");
    textures.insert(textures.end(), reflectionMap.begin(), reflectionMap.end());

    // return a mesh object created from the extracted mesh data
    return mesh::Mesh(vertices, indices, textures);
}


std::vector<mesh::Texture> Model::loadMaterialTextures(
    aiMaterial *mat
    , const aiTextureType &type
    , const std::string &typeName) {
    std::vector<mesh::Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so,
        // continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);

                // a texture with the same filepath has already been loaded,
                // continue to next one. (optimization)
                skip = true;
                break;
            }
        }

        // if texture hasn't been loaded already, load it
        if (!skip) {
            mesh::Texture texture;

            auto path = this->directory+"/";
            texture.id = util::LoadTexture2D(path+str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
            // store it as texture loaded for entire model,
            // to ensure we won't unnecesery load duplicate textures.
        }
    }

    return textures;
}
