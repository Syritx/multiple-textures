//
//  sprite.h
//  opengl-app
//
//  Created by Syritx on 2021-03-03.
//

#ifndef sprite_h
#define sprite_h

#include "shader.h"
#include <iostream>
#include <string>

int RELATIVE_TO_PARENT = 1;
int INDEPENDENT = 0;

using namespace std;

class Asset {
public:
    
    int spr_width, spr_height, channels;
    unsigned int texture;
    float vertices[30];
    GLuint _vao, _vbo;
    Shader aShader;
    FramePosition position;
    FrameSize glsize;
    
    virtual void render() {
        
        aShader.use_shader();
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)/sizeof(vertices[0]) * sizeof(float), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    
    virtual void set_position(FramePosition relativePosition) {
        float _v[] = {
            -glsize.w/2+relativePosition.x+this->position.x, -glsize.h/2+relativePosition.y+this->position.y, 0, 0.f, 1.f,
             glsize.w/2+relativePosition.x+this->position.x,  glsize.h/2+relativePosition.y+this->position.y, 0, 1.f, 0.f,
            -glsize.w/2+relativePosition.x+this->position.x,  glsize.h/2+relativePosition.y+this->position.y, 0, 0.f, 0.f,
            
            -glsize.w/2+relativePosition.x+this->position.x, -glsize.h/2+relativePosition.y+this->position.y, 0, 0.f, 1.f,
             glsize.w/2+relativePosition.x+this->position.x, -glsize.h/2+relativePosition.y+this->position.y, 0, 1.f, 1.f,
             glsize.w/2+relativePosition.x+this->position.x,  glsize.h/2+relativePosition.y+this->position.y, 0, 1.f, 0.f,
        };
        
        for (int i = 0; i < 30; i++) {
            vertices[i] = _v[i];
        }
    }
};


class Sprite: public Asset {
    
public:
    
    Sprite(char* vertexShaderSource, char* fragmentShaderSource, string path, FramePosition position, int relativity) {
        
        this->position = position;
        unsigned char* data = stbi_load(path.c_str(), &spr_width, &spr_height, &channels, 0);
        aShader = Shader(vertexShaderSource, fragmentShaderSource);
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glsize = pixelSizeToScreenSize((int)spr_width*.6f, (int)spr_height*.6f, 1200, 800);
        
        /*
         TEXCOORDS:
         
            TEXCOORD X:
                -X = 0
                +X = 1
            TEXCOORD Y:
                -Y = 1
                +Y = 0
         */
        
        
        float _v[] = {
            -glsize.w/2+position.x, -glsize.h/2+position.y, 0, 0.f, 1.f,
             glsize.w/2+position.x,  glsize.h/2+position.y, 0, 1.f, 0.f,
            -glsize.w/2+position.x,  glsize.h/2+position.y, 0, 0.f, 0.f,
            
            -glsize.w/2+position.x, -glsize.h/2+position.y, 0, 0.f, 1.f,
             glsize.w/2+position.x, -glsize.h/2+position.y, 0, 1.f, 1.f,
             glsize.w/2+position.x,  glsize.h/2+position.y, 0, 1.f, 0.f,
        };
        
        for (int i = 0; i < sizeof(_v)/sizeof(_v[0]); i++) {
            vertices[i] = _v[i];
        }
        
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spr_width, spr_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            cout << "failed to load image\n";
        }
        stbi_image_free(data);
        
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)/sizeof(vertices[0]) * sizeof(float), vertices, GL_STATIC_DRAW);
        
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glBindVertexArray(0);
    }
};


#endif /* sprite_h */
