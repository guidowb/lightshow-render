#ifndef Canvasses_h
#define Canvasses_h

#include "LightShow.h"

class MappedCanvas : public Canvas {
protected:
    MappedCanvas(Canvas *parent);

public:
    virtual uint16_t getSize();
    virtual uint32_t globalTime();
    virtual uint32_t sceneTime();
    virtual void setPixel(uint16_t pixel, RGBA color);

protected:
    Canvas *parent;
};

#endif