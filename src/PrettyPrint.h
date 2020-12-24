#ifndef _PRETTYPRINT_H_
#define _PRETTYPRINT_H_

#include "Serialize.h"

class PrettyPrint : public Serializer {
public:
    PrettyPrint();

public:
    virtual void startObject(const char *kind);
    virtual void addProperty(const char *name, Renderer *value);
    virtual void addProperty(const char *name, const char *value);
    virtual void addProperty(const char *name, const RGBA &color);
    virtual void addProperty(const char *name, uint32_t value);
    virtual void addProperty(const char *name, int ncolors, const RGBA *color);
    virtual void endObject();

private:
    int currentIndent;
};

#endif
