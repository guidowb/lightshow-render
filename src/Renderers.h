//
//  Renderers.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Renderers_h
#define Renderers_h

#include "LightShow.h"
#include <vector>

class NullRenderer : public Renderer {
public:
    NullRenderer();
    virtual void render(Canvas *canvas);
};

class BlockRenderer : public Renderer {
public:
    BlockRenderer(std::vector<Renderer *> &renderers);
    virtual ~BlockRenderer();
    virtual void render(Canvas *canvas);

private:
    int nrenderers;
    Renderer **renderer;
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
    DotsRenderer(int spacing, std::vector<RGBA> &colors);
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

#endif /* Renderers_h */