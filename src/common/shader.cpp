#include "src/common/shader.h"

Shader::~Shader() {
    deleteProgram();
}

bool Shader::Compile(const std::string &vertex_file
                    , const std::string &fragment_file) {
    deleteProgram();

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_code, fragment_code;
    try {
        std::ifstream vertext_ifs, fragment_ifs;
        // ensure ifstream objects can throw exceptions:
        vertext_ifs.exceptions(std::ifstream::failbit
                                    | std::ifstream::badbit);
        fragment_ifs.exceptions(std::ifstream::failbit
                                    | std::ifstream::badbit);

        // open files
        vertext_ifs.open(vertex_file);
        fragment_ifs.open(fragment_file);

        std::stringstream s0, s1;
        s0 << vertext_ifs.rdbuf();
        s1 << fragment_ifs.rdbuf();

        // close file handlers
        vertext_ifs.close();
        fragment_ifs.close();

        // read file's buffer contents into streams
        // and convert stream into string
        vertex_code   = s0.str();
        fragment_code = s1.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }

    // 2. compile shaders
    GLuint vertex = 0, fragment = 0;
    bool ret = false;
    do {
        const char *p_vertex_code = vertex_code.c_str();
        const char *p_fragment_code = fragment_code.c_str();

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        // 传入代码
        glShaderSource(vertex, 1, &p_vertex_code, nullptr);
        // 编译
        glCompileShader(vertex);
        // 检查错误
        if (hasCompileErrors(vertex, "VERTEX")) {
            break;
        }

        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &p_fragment_code, nullptr);
        glCompileShader(fragment);
        if (hasCompileErrors(fragment, "FRAGMENT")) {
            break;
        }

        // shader Program
        id_ = glCreateProgram();
        glAttachShader(id_, vertex);
        glAttachShader(id_, fragment);
        // 链接
        glLinkProgram(id_);
        if (hasCompileErrors(id_, "PROGRAM")) {
            glDeleteProgram(id_);
            break;
        }

        ret = true;
    }while(false);

    // delete the shaders as they're linked into our program now
    // and no longer necessary
    if (vertex != 0) {
        glDeleteShader(vertex);
    }
    if (fragment != 0) {
        glDeleteShader(fragment);
    }

    return ret;
}


void Shader::Use() {
    glUseProgram(id_);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value ? 1 : 0);
}

void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, const float *value) const {
    glUniform3f(glGetUniformLocation(id_, name.c_str())
        , value[0], value[1], value[2]);
}
void Shader::SetVec3(const std::string &name
    , float v0, float v1, float v2) const {
    float tmp[]{v0, v1, v2};
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, tmp);
}

void Shader::SetMatrix4fv(const std::string &name, const float *value) const {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str())
        , 1, GL_FALSE, value);
}

bool Shader::hasCompileErrors(GLuint shader, const std::string &type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
            << "\n" << infoLog
            << "\n ------------------------------------------------------- "
            << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
            << "\n" << infoLog
            << "\n ------------------------------------------------------- "
            << std::endl;
        }
    }

    return (!success);
}

void Shader::deleteProgram() {
    if (id_ != 0) {
        glUseProgram(0);
        glDeleteProgram(id_);
    }
}
