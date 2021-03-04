//
//  screen.h
//  opengl-app
//
//  Created by Syritx on 2021-03-03.
//

#ifndef screen_h
#define screen_h

struct FrameSize {
    float w;
    float h;
};

struct FramePosition {
    float x;
    float y;
};


struct FrameSize pixelSizeToScreenSize(int px, int py, int w, int h) {
    float width = (float)px/(float)w;
    float height = (float)py/(float)h;
    
    FrameSize frame;
    frame.w = width;
    frame.h = height;
    return frame;
}

struct FramePosition pixelPositionToScreenPosition(int px, int py, int w, int h) {
    float width = (float)px/(float)w;
    float height = (float)py/(float)h;
    
    FramePosition frame;
    frame.x = width;
    frame.y = height;
    return frame;
}

struct FramePosition initPosition(float x, float y) {
    FramePosition position;
    position.x = x;
    position.y = y;
    return position;
}


#endif /* screen_h */
