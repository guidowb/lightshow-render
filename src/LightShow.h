//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef LightShow_h
#define LightShow_h

#include <stdint.h>

class RGBA {
public:
    RGBA();
    RGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0x0ff);

public:
    void operator+=(const RGBA other);
    RGBA operator+(const RGBA other) const;
    bool operator==(const RGBA other) const;
    operator uint32_t() const;

public:
    RGBA blend(const RGBA other, uint32_t fraction) const;

public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

const RGBA RGBA_NULL;

class Canvas {
public:
    virtual uint16_t getSize() = 0;
    virtual uint32_t globalTime() = 0;
    virtual uint32_t sceneTime();
    virtual uint32_t epochTime();
    virtual void setPixel(uint16_t pixel, RGBA color) = 0;
};

class Renderer {
public:
    virtual ~Renderer();
    virtual bool render(Canvas *canvas) = 0;
};

extern void render(Renderer *renderer, Canvas *canvas);
extern Renderer *compile(const char *sourceName, const char *pattern);

#endif /* LightShow_h */
