//
//  shader.h
//  opengl-app
//
//  Created by Syritx on 2021-01-29.
//

#ifndef shader_h
#define shader_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
private:
    int vertexShader;
    int fragmentShader;
    
public:
    int program;
    
    Shader(const char* vertexShaderSource = nullptr, const char* fragmentShaderSource = nullptr) {
        
        if (vertexShaderSource && fragmentShaderSource) {
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(vertexShader);
            glCompileShader(fragmentShader);
            
            program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            
            std::cout << program << std::endl;
        }
    }
    
    void use_shader() {
        glUseProgram(program);
    }
};


#endif /* shader_h */
