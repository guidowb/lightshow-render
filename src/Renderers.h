//
//  Renderers.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Renderers_h
#define Renderers_h

#include <stdlib.h>
#include "LightShow.h"
#include "Canvasses.h"

template<typename T> class Vector {
public:
    Vector(int capacity) {
        this->capacity = capacity;
        this->count = 0;
        this->elements = new T[capacity+1];
    }
    ~Vector() { delete elements; }

    bool append(T element) { if (count >= capacity) return false; elements[count++] = element; return true; }
    T operator[](int index) { if (index < count) return elements[index]; return elements[capacity]; }
    int size() { return count; }

private:
    int count;
    int capacity;
    T *elements;
};

class LayeredRenderer : public Renderer {
public:
    LayeredRenderer(Renderer *thisLayer, Renderer *nextLayer);
    virtual ~LayeredRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
    Renderer *thisLayer;
    Renderer *nextLayer;
};

class DuplicateRenderer : public Renderer {
public:
    DuplicateRenderer(Renderer *layer);
    virtual ~DuplicateRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
    Renderer *layer;
};

class NullRenderer : public Renderer {
public:
    NullRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);
};

class SolidRenderer : public Renderer {
public:
    SolidRenderer(RGBA color);
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
    RGBA color;
};

class DotsRenderer : public Renderer {
public:
    DotsRenderer(uint16_t spacing, Vector<RGBA> &colors);
    virtual ~DotsRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
    uint16_t spacing;
    int ncolors;
    RGBA *color;
};

class TwinkleRenderer : public Renderer {
public:
    TwinkleRenderer(RGBA color, uint16_t tpm);
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
	uint32_t twinkles_per_minute;
	uint32_t cycle_brighten;
	uint32_t cycle_dim;
	RGBA color;
};

class SegmentRenderer : public Renderer, MappedCanvas {
public:
    SegmentRenderer(uint16_t from, uint16_t to, Renderer *renderer);
    virtual ~SegmentRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);
    virtual void setPixel(uint16_t pixel, RGBA color);
    virtual uint16_t getSize();

private:
    uint16_t from;
    uint16_t to;
    Renderer *renderer;
};

class GradientRenderer : public Renderer {
public:
    GradientRenderer(Vector<RGBA> &colors);
    virtual ~GradientRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
    int ncolors;
    RGBA *color;
};

class FadeRenderer : public Renderer, MappedCanvas {
public:
    FadeRenderer(Renderer *before, Renderer *after, uint32_t duration);
    virtual ~FadeRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);
    virtual void setPixel(uint16_t pixel, RGBA color);

private:
    Renderer *before;
    Renderer *after;
    uint32_t duration;
    uint32_t ratio;
};

class WipeRenderer : public Renderer, MappedCanvas {
public:
    WipeRenderer(Renderer *before, Renderer *after, uint32_t duration);
    virtual ~WipeRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);
    virtual void setPixel(uint16_t pixel, RGBA color);

private:
    Renderer *before;
    Renderer *after;
    uint32_t duration;
    uint32_t ratio;
};

class RotateRenderer : public Renderer, MappedCanvas {
public:
    RotateRenderer(Renderer *block, uint32_t pps);
    virtual ~RotateRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);
    virtual void setPixel(uint16_t pixel, RGBA color);

private:
    Renderer *block;
    uint32_t pps;
    uint16_t offset;
};

class AfterRenderer : public Renderer , MappedCanvas {
public:
    AfterRenderer(Renderer *before, Renderer *after, uint32_t duration);
    virtual ~AfterRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);
    virtual uint32_t sceneTime();

private:
    Renderer *before;
    Renderer *after;
    uint32_t duration;
};

class RepeatRenderer : public Renderer, MappedCanvas {
public:
    RepeatRenderer(Renderer *block);
    virtual ~RepeatRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);
    virtual uint32_t sceneTime();

private:
    Renderer *block;
    uint32_t clockSkew;
};

class TimeRenderer : public Renderer {
public:
    TimeRenderer(Renderer *block, uint32_t from, uint32_t to);
    virtual ~TimeRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
    Renderer *block;
    uint32_t from;
    uint32_t to;
};

class DateRenderer : public Renderer {
public:
    DateRenderer(Renderer *block, uint16_t from, uint16_t to);
    virtual ~DateRenderer();
    virtual bool render(Canvas *canvas);
    virtual void serialize(Serializer &serializer);

private:
    Renderer *block;
    uint16_t from;
    uint16_t to;
};

#endif /* Renderers_h */