//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef LightShow_h
#define LightShow_h

#include <stdint.h>

typedef uint32_t RGBA;
const RGBA RGBA_NULL = 0;

class Canvas {
    virtual int getSize() = 0;
    virtual int getTimestamp() = 0;
    virtual void setPixel(int pixel, RGBA color) = 0;
};

class Renderer {
    virtual void render(Canvas *canvas) = 0;
};

#endif /* LightShow_h */