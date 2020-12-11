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
    virtual void render(Canvas *canvas);

private:
    Renderer *thisLayer;
    Renderer *nextLayer;
};

class NullRenderer : public Renderer {
public:
    NullRenderer();
    virtual void render(Canvas *canvas);
};

class SolidRenderer : public Renderer {
public:
    SolidRenderer(RGBA color);
    virtual void render(Canvas *canvas);

private:
    RGBA color;
};

class DotsRenderer : public Renderer {
public:
    DotsRenderer(int spacing, Vector<RGBA> &colors);
    virtual ~DotsRenderer();
    virtual void render(Canvas *canvas);

private:
    int spacing;
    int ncolors;
    RGBA *color;
};

class TwinkleRenderer : public Renderer {
public:
    TwinkleRenderer(RGBA color, int tpm);
    virtual void render(Canvas *canvas);

private:
	long twinkles_per_minute;
	long cycle_brighten;
	long cycle_dim;
	RGBA color;
};

class SegmentRenderer : public Renderer {
public:
    SegmentRenderer(int from, int to, Renderer *renderer);
    virtual ~SegmentRenderer();
    virtual void render(Canvas *canvas);

private:
    int from;
    int to;
    Renderer *renderer;
};

class GradientRenderer : public Renderer {
public:
    GradientRenderer(Vector<RGBA> &colors);
    virtual ~GradientRenderer();
    virtual void render(Canvas *canvas);

private:
    int ncolors;
    RGBA *color;
};

class FadeRenderer : public Renderer {
public:
    FadeRenderer(Renderer *before, Renderer *after);
    virtual ~FadeRenderer();
    virtual void render(Canvas *canvas);

private:
    Renderer *before;
    Renderer *after;
};

#endif /* Renderers_h */