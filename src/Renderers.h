//
//  Renderers.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Renderers_h
#define Renderers_h

#include "Render.h"

class SolidRenderer : public Renderer {
public:
    SolidRenderer(RGBA color);
    virtual void render(Canvas *canvas);

private:
    RGBA color;
};

#endif /* Renderers_h */