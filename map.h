//
//  map.h
//  opengl-app
//
//  Created by Syritx on 2021-03-03.
//

#ifndef map_h
#define map_h
#include "screen.h"
#include "sprite.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
using namespace std;

float aspect = 1.5714285f;
int width = 1400, height = (int)width/aspect;

FramePosition position = initPosition(0,0);

char* vertexShaderSource =
"#version 330 core \n"
"layout (location = 0) in vec3 vp;"
"layout (location = 1) in vec2 aTexCoord;"
"out vec2 TexCoord;"
" "
"void main() {"
"  TexCoord = aTexCoord;"
"  gl_Position = vec4(vp, 1.0);"
"}";

char* fragmentShaderSource =
"#version 330 core \n"
"out vec4 FragColor;"
"in vec2 TexCoord;"
"uniform sampler2D ourTexture;"
" "
"void main() {"
"  FragColor = texture(ourTexture, TexCoord) * vec4(1.0, 1.0, 1.0, 1.0);"
"}";

GLuint vao, vbo;
Asset assets[100];

int iwidth, iheight, nrChannels;
unsigned char *data;
unsigned int texture;

FramePosition map_p;
FrameSize map = pixelSizeToScreenSize(width, height, 1200, 800);
Shader shader;
int asset_id = 0;

float verts[] = {
    -map.w/2+position.x, -map.h/2+position.y, 0, 0.0f, 1.0f,
     map.w/2+position.x, -map.h/2+position.y, 0, 1.0f, 1.0f,
    -map.w/2+position.x,  map.h/2+position.y, 0, 0.0f, 0.0f,
    
     map.w/2+position.x,  map.h/2+position.y, 0, 1.0f, 0.0f,
     map.w/2+position.x, -map.h/2+position.y, 0, 1.0f, 1.0f,
    -map.w/2+position.x,  map.h/2+position.y, 0, 0.0f, 0.0f
};

void set_map_position(float x, float y) {
    map_p = initPosition(x, y);
    
    float v[] = {
        -map.w/2+map_p.x, -map.h/2+map_p.y, 0, 0.0f, 1.0f,
         map.w/2+map_p.x, -map.h/2+map_p.y, 0, 1.0f, 1.0f,
        -map.w/2+map_p.x,  map.h/2+map_p.y, 0, 0.0f, 0.0f,
        
         map.w/2+map_p.x,  map.h/2+map_p.y, 0, 1.0f, 0.0f,
         map.w/2+map_p.x, -map.h/2+map_p.y, 0, 1.0f, 1.0f,
        -map.w/2+map_p.x,  map.h/2+map_p.y, 0, 0.0f, 0.0f
    };
    for (int i = 0; i < sizeof(v)/sizeof(v[0]); i++) {
        verts[i] = v[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts)/sizeof(verts[0]) * sizeof(float), verts, GL_STATIC_DRAW);
}

bool map_mouse_in_bounds(float x, float y) {
    
    float xMax = verts[5],
          yMax = verts[11];
    
    float xMin = verts[0],
          yMin = verts[1];
    
    cout << -verts[0] << endl;
    
    if (x < xMax && x > xMin && y < yMax && y > yMin) return true;
    return false;
}

void start_map(string _path) {
    
    string full_path = _path + "g/Resources/textures/map.jpg";
    data = stbi_load(full_path.c_str(), &iwidth, &iheight, &nrChannels, 0);
    
    shader = Shader(vertexShaderSource, fragmentShaderSource);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "failed to load image\n";
    }
    stbi_image_free(data);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts)/sizeof(verts[0]) * sizeof(float), verts, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glBindVertexArray(0);
}

void map_add_sprite(Asset asset) {
    assets[asset_id] = asset;
    asset_id++;
}

void render_map() {
    
    for (Asset a : assets) {
        a.set_position(map_p);
        a.render();
    }
        
    shader.use_shader();
    cout << verts[0] << endl;
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts)/sizeof(verts[0]) * sizeof(float), verts, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



#endif /* map_h */
