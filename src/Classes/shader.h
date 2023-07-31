#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Shader
{
    public:
        static Shader* singleton;
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath)
        {
            if(singleton == nullptr)
            {
                singleton = this;
            }
            // 1. retrieve the vertex/fragment source code from filePath
            string vertexCode;
            string fragmentCode;
            ifstream vShaderFile;
            ifstream fShaderFile;
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
            fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
            try 
            {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                // convert stream into string
                vertexCode   = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (ifstream::failure& e)
            {
                cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char * fShaderCode = fragmentCode.c_str();
            // 2. compile shaders
            unsigned int vertex, fragment;
            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");
            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");
            // shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");
            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use()
        {
            glUseProgram(ID);
        }



        void setBool(const char* nome, bool value)
        {
            glUniform1i(glGetUniformLocation(ID, nome), value);
        }

        void setInt(const char* nome, int value)
        {
            glUniform1i(glGetUniformLocation(ID, nome), value);
        }

        void setFloat(const char* nome, float value)
        {
            glUniform1f(glGetUniformLocation(ID, nome), value);
        }

        void setMatrix(const char* nome, mat4 value)
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, nome), 1, GL_FALSE, value_ptr(value));
        }

    private:

        void checkCompileErrors(unsigned int shader, string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
                }
            }
        }

};


Shader* Shader::singleton = nullptr;



#endif