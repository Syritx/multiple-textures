//
//  main.cpp
//  opengl-app
//
//  Created by Syritx on 2021-01-29.
//

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "screen.h"
#include "map.h"
#include "sprite.h"
#include <string>

using namespace std;
bool canMove = false;

float xGLPosition, yGLPosition;
float xLast, yLast;

char* spriteVertexShader =
"#version 330 core \n"
"layout (location = 0) in vec3 vp;"
"layout (location = 1) in vec2 aTexCoord;"
"out vec2 TexCoord;"
" "
"void main() {"
"  TexCoord = aTexCoord;"
"  gl_Position = vec4(vp, 1.0);"
"}";


char* spriteFragmentShader =
"#version 330 core \n"
"out vec4 FragColor;"
"in vec2 TexCoord;"
"uniform sampler2D ourTexture;"
" "
"void main() {"
"  FragColor = texture(ourTexture, TexCoord) * vec4(1.0, 1.0, 1.0, 1.0);"
"}";

char* projectPath;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        canMove = true;
        xLast = xGLPosition;
        yLast = yGLPosition;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) canMove = false;
}


void cursorPositionCallback(GLFWwindow* window, double x, double y) {
    
    float xpos = (float)x;
    float ypos = (float)y;
    xGLPosition = (xpos/1200.f-0.5f)*2;
    yGLPosition = -(ypos/800.f-0.5f)*2;
    
    cout << xGLPosition << " " << yGLPosition << endl;
    
    bool isInBounds = map_mouse_in_bounds(xGLPosition, yGLPosition);
    if (isInBounds && canMove) {
        float _x = xGLPosition-xLast;
        float _y = yGLPosition-yLast;
        
        xLast = xGLPosition;
        yLast = yGLPosition;
        set_map_position(map_p.x+_x, map_p.y+_y);
    }
    else if (!isInBounds) cout << "out bounds" << endl;
    else if (isInBounds) cout << "in bounds" << endl;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        FramePosition spPosition = initPosition(xGLPosition, yGLPosition);
        string p = string(projectPath);
        string path = p.substr(0,p.find("g/opengl-app"));
        string spr_path = path+"g/Resources/textures/My Post(6).jpg";
        map_add_sprite(Sprite(spriteVertexShader, spriteFragmentShader, spr_path, spPosition, RELATIVE_TO_PARENT));
    }
}



int main(int argc, char *argv[]) {
    
    if (!glfwInit()) glfwTerminate();
        
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    projectPath = argv[0];
    
    GLFWwindow* window = glfwCreateWindow(1200, 800, "2030", NULL, NULL);
    
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    
    if (!window) glfwTerminate();
    
    int panelWidth = 1100, panelHeight = 700;
    FrameSize f = pixelSizeToScreenSize(panelWidth, panelHeight, 1200, 800);
    cout << "w:" << f.w << " h:" << f.h << "\n";
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    
    glEnable(GL_DEPTH_TEST);
    string p = string(argv[0]);
    string path = p.substr(0,p.find("g/opengl-app"));
    start_map(path);
    FramePosition spPosition = initPosition(0.6f, 0.6f);
    string spr_path = path+"g/Resources/textures/My Post(6).jpg";
    map_add_sprite(Sprite(spriteVertexShader, spriteFragmentShader, spr_path, spPosition, RELATIVE_TO_PARENT));
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_map();
        
        ofstream myfile;
        myfile.open("output.txt");
        myfile << xGLPosition;
        myfile.close();
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}
