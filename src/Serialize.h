#ifndef Serialize_h
#define Serialize_h

#include <iostream>
#include <string>

#include "Renderers.h"

class Serializer {
public:
    virtual void startObject(const char *kind) = 0;
    virtual void addProperty(const char *name, Renderer *value) = 0;
    virtual void addProperty(const char *name, const char *value) = 0;
    virtual void addProperty(const char *name, const RGBA &color) = 0;
    virtual void addProperty(const char *name, uint32_t value) = 0;
    virtual void addProperty(const char *name, int ncolors, const RGBA *color) = 0;
    virtual void endObject() = 0;
};

#endif /* Serialize.h */