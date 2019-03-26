#ifndef SRC_COMMON_SHADER_H_
#define SRC_COMMON_SHADER_H_

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
 public:
    ~Shader();

    // generates the shader on the fly
    // ------------------------------------------------------------------------
    bool Compile(const std::string &vertex_file
                    , const std::string &fragment_file);

    // activate the shader
    // ------------------------------------------------------------------------
    void Use();

    // utility uniform functions
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec3(const std::string &name, const float *value) const;
    void SetVec3(const std::string &name, float v0, float v1, float v2) const;
    void SetMatrix4fv(const std::string &name, const float *value) const;

 private:
    GLuint id_;

    void deleteProgram();

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    bool hasCompileErrors(GLuint shader, const std::string &type);
};

#endif  // SRC_COMMON_SHADER_H_
