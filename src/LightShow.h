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
public:
    virtual uint16_t getSize() = 0;
    virtual uint32_t globalTime() = 0;
    virtual uint32_t sceneTime();
    virtual void setPixel(int pixel, RGBA color) = 0;
};

class Renderer {
public:
    virtual ~Renderer();
    virtual bool render(Canvas *canvas) = 0;
};

extern bool render(const char *sourceName, const char *pattern, Canvas *canvas);
extern Renderer *compile(const char *sourceName, const char *pattern);

#endif /* LightShow_h */