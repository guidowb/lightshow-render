#include "Renderers.h"
#include "Canvasses.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

//#define printf(fmt, ...) {}

Renderer::~Renderer() {}

// This, of course, isn't truly random. We're not doing security here.
// We need something that looks random but is perfectly predictable, so
// that, when we calculate consecutive frames, we get identical "random"
// values for each pixel.

static uint32_t random(uint32_t seed) {
	uint32_t r = seed * 179426323;
    return r;
}

NullRenderer::NullRenderer() {}

bool NullRenderer::render(Canvas *canvas) { return true; }

LayeredRenderer::LayeredRenderer(Renderer *thisLayer, Renderer *nextLayer) {
    this->thisLayer = thisLayer;
    this->nextLayer = nextLayer;
}

LayeredRenderer::~LayeredRenderer() {
    if (this->thisLayer) delete this->thisLayer;
    if (this->nextLayer) delete this->nextLayer;
}

bool LayeredRenderer::render(Canvas *canvas) {
    bool complete = true;
    if (this->thisLayer) complete &= this->thisLayer->render(canvas);
    if (this->nextLayer) complete &= this->nextLayer->render(canvas);
    return complete;
}

DuplicateRenderer::DuplicateRenderer(Renderer *layer) {
    this->layer = layer;
}

DuplicateRenderer::~DuplicateRenderer() {
    // Don't delete the layer, as it's a duplicate and being deleted elsewhere
}

bool DuplicateRenderer::render(Canvas *canvas) {
    return this->layer->render(canvas);
}

SolidRenderer::SolidRenderer(RGBA color) {
    this->color = color;
}

bool SolidRenderer::render(Canvas *canvas) {
    for (uint16_t p = 0; p < canvas->getSize(); p++) canvas->setPixel(p, color);
    return true;
}

DotsRenderer::DotsRenderer(uint16_t spacing, Vector<RGBA> &colors) {
    this->spacing = spacing;
    this->ncolors = colors.size();
    this->color = new RGBA[this->ncolors];
    for (uint16_t p = 0; p < this->ncolors; p++) this->color[p] = colors[p];
}

DotsRenderer::~DotsRenderer() {
    delete this->color;
}

bool DotsRenderer::render(Canvas *canvas) {
    int c = 0;
    for (uint16_t p = 0; p < canvas->getSize(); p += spacing) {
        canvas->setPixel(p, color[c]);
        c = (c + 1) % ncolors;
    }
    return true;
}

TwinkleRenderer::TwinkleRenderer(RGBA color, uint16_t tpm) {

    // Any given pixel twinkles every cycle_total (N) millis.
    // It takes cycle_brighten (K) time to brighten, and
    // cycle_dim (M) time to dim.
    //
    // Full bright is white (A).
    // Dim is 100% (B) of given color.
    //
    // Given a pixel and its start time
    // if T < K brightness is increasing
    // if K < T < M+K brightness is dimming
    // else brightness is dim
    //
    // Given pixel p, its start time is determined by multiplying
    // its position by some really big prime. That is the part that
    // spreads the twinkle across the whole pattern evenly but
    // seemingly randomly.
    //
    // The number of twinkles across the entire patterns is
    // controlled by the total cycle time for each pixel. It's
    // parameterized by specifying the desired number of twinkles
    // per minute across the entire pattern.

	this->twinkles_per_minute = tpm ? tpm : 300; // 5 per second
	this->cycle_brighten = 100; // 1/10th second
	this->cycle_dim = 1000; // 1 second
    this->color = color;
}

bool TwinkleRenderer::render(Canvas *canvas) {

    uint32_t cycle_total = (60000 * canvas->getSize()) / this->twinkles_per_minute;
    for (uint16_t p = 0; p < canvas->getSize(); p++) {
        uint32_t ptime = (canvas->globalTime() + random(p)) % cycle_total;
        if (ptime < cycle_brighten) {
            RGBA color = this->color;
            color.a = ((double) ptime / cycle_brighten) * (this->color.a);
            canvas->setPixel(p, color);
        }
        else if (ptime < (cycle_dim + cycle_brighten)) {
            ptime -= cycle_brighten;
            RGBA color = this->color;
            color.a = (1.0 - (double) ptime / cycle_dim) * (this->color.a);
            canvas->setPixel(p, color);
        }
    }
    return true;
}

SegmentRenderer::SegmentRenderer(uint16_t from, uint16_t to, Renderer *renderer) {
    this->from  = from;
    this->to = to;
    this->renderer = renderer;
}

SegmentRenderer::~SegmentRenderer() {
    delete this->renderer;
}

bool SegmentRenderer::render(Canvas *canvas) {

    class SegmentCanvas : public MappedCanvas {
    public:
        SegmentCanvas(Canvas *parent, uint16_t from, uint16_t to) : MappedCanvas(parent) { this->from = from; this->to = to; }
        virtual uint16_t getSize() { return to - from; }
        virtual void setPixel(uint16_t pixel, RGBA color) { parent->setPixel(pixel + from, color); }

    private:
        uint16_t from;
        uint16_t to;
    };

    printf("Segment from %d to %d\n", from, to);
    SegmentCanvas segment(canvas, from, to);
    return renderer->render(&segment);
}

GradientRenderer::GradientRenderer(Vector<RGBA> &colors) {
    this->ncolors = colors.size();
    this->color = new RGBA[this->ncolors];
    for (uint16_t p = 0; p < this->ncolors; p++) this->color[p] = colors[p];
}

GradientRenderer::~GradientRenderer() {
    delete this->color;
}

bool GradientRenderer::render(Canvas *canvas) {
    int segments = ncolors - 1;
    uint16_t interval = canvas->getSize() / segments;
    for (uint16_t p = 0; p < canvas->getSize(); p++) {
        int i1 = (p / interval) % ncolors;
        int i2 = (i1 + 1) % ncolors;
        int fraction = (p % interval) * 1000 / interval;
        canvas->setPixel(p, color[i1].blend(color[i2], fraction));
    }
    return true;
}

FadeRenderer::FadeRenderer(Renderer *before, Renderer *after, uint32_t duration) {
    this->before = before;
    this->after = after;
    this->duration = duration;
}

FadeRenderer::~FadeRenderer() {
    if (this->before) delete this->before;
    if (this->after)  delete this->after;
}

bool FadeRenderer::render(Canvas *canvas) {

    uint32_t time = canvas->sceneTime();

    if (time > duration) {
        return after->render(canvas);
    }

    // TODO: If the "after" renderer has any transparency, then we can't stop rendering the
    // "before" at the end of the fade. That's something we should consider detecting and
    // handling more properly.

    class FadeCanvas : public MappedCanvas {
    public:
        FadeCanvas(Canvas *parent, uint32_t ratio) : MappedCanvas(parent) { this->ratio = ratio; }
        virtual void setPixel(uint16_t pixel, RGBA color) {
            color.a = (color.a * ratio) / 1000;
            parent->setPixel(pixel, color);
        }

    private:
        uint32_t ratio;
    };

    before->render(canvas);
    uint32_t ratio = (time * 1000) / duration;
    FadeCanvas faded(canvas, ratio);
    after->render(&faded);
    return false;
}

WipeRenderer::WipeRenderer(Renderer *before, Renderer *after, uint32_t duration) {
    this->before = before;
    this->after = after;
    this->duration = duration;
}

WipeRenderer::~WipeRenderer() {
    if (this->before) delete this->before;
    if (this->after)  delete this->after;
}

bool WipeRenderer::render(Canvas *canvas) {

    uint32_t time = canvas->sceneTime();

    if (time > duration) {
        return after->render(canvas);
    }

    class WipeCanvas : public MappedCanvas {
    public:
        WipeCanvas(Canvas *parent, uint32_t ratio) : MappedCanvas(parent) { this->ratio = ratio; }
        virtual void setPixel(uint16_t pixel, RGBA color) {
            uint16_t cut = (getSize() * ratio) / 1000;
            if (pixel <= cut) parent->setPixel(pixel, color);
        }

    private:
        uint32_t ratio;
    };

    before->render(canvas);
    uint32_t ratio = (time * 1000) / duration;
    WipeCanvas faded(canvas, ratio);
    after->render(&faded);
    return false;
}

RotateRenderer::RotateRenderer(Renderer *block, uint32_t pps) {
    this->block = block;
    this->pps = pps;
}

RotateRenderer::~RotateRenderer() {
    if (this->block) delete this->block;
}

bool RotateRenderer::render(Canvas *canvas) {

    uint32_t time = canvas->sceneTime();

    class RotateCanvas : public MappedCanvas {
    public:
        RotateCanvas(Canvas *parent, uint16_t offset) : MappedCanvas(parent) { this->offset = offset; }
        virtual void setPixel(uint16_t pixel, RGBA color) {
            uint16_t npixel = (pixel + offset) % getSize();
            parent->setPixel(npixel, color);
        }

    private:
        uint16_t offset;
    };

    RotateCanvas rotated(canvas, time / pps);
    block->render(&rotated);
    return true;
}

AfterRenderer::AfterRenderer(Renderer *before, Renderer *after, uint32_t duration) {
    this->before = before;
    this->after = after;
    this->duration = duration;
}

AfterRenderer::~AfterRenderer() {
    // After renderers don't delete their before scene, because
    // that scene is also referenced inside of its after scene.
    // Thus the single delete will cause both to be cleaned up.
    if (this->after)  delete this->after;
}

bool AfterRenderer::render(Canvas *canvas) {

    class AfterCanvas : public MappedCanvas {
    public:
        AfterCanvas(Canvas *parent, uint32_t duration) : MappedCanvas(parent) { this->duration = duration; }
        virtual uint32_t sceneTime() { return parent->sceneTime() - duration; }

    private:
        uint32_t duration;
    };

    uint32_t time = canvas->sceneTime();

    before->render(canvas);
    if (time > duration) {
        AfterCanvas skewed(canvas, duration);
        return after->render(&skewed);
    }
    return false;
}

RepeatRenderer::RepeatRenderer(Renderer *block) {
    this->block = block;
    this->clockSkew = 0;
}

RepeatRenderer::~RepeatRenderer() {
    if (this->block) delete this->block;
}

bool RepeatRenderer::render(Canvas *canvas) {

    uint32_t sceneTime = canvas->sceneTime();

    class RepeatCanvas : public MappedCanvas {
    public:
        RepeatCanvas(Canvas *parent, uint32_t clockSkew) : MappedCanvas(parent) { this->clockSkew = clockSkew; }
        virtual uint32_t sceneTime() { return parent->sceneTime() - clockSkew; }

    private:
        uint32_t clockSkew;
    };

    RepeatCanvas repeat(canvas, this->clockSkew);
    if (block->render(&repeat)) {
        this->clockSkew = sceneTime;
    }
    return true;
}

TimeRenderer::TimeRenderer(Renderer *block, uint32_t from, uint32_t to) {
    this->block = block;
    this->from = from;
    this->to = to;
}

TimeRenderer::~TimeRenderer() {
    if (this->block) delete this->block;
}

bool TimeRenderer::render(Canvas *canvas) {
    const uint32_t epoch = canvas->epochTime();
    if (epoch == 0) return true;

    const time_t time = epoch;
    struct tm *timeinfo = localtime(&time);
    uint32_t now = timeinfo->tm_sec + 60 * timeinfo->tm_min + 3600 * timeinfo->tm_hour;

    bool inWindow = false;
    if (from < to) inWindow = (from <= now) && (now < to);
    else inWindow = (from <= now) || (now < to);

    if (inWindow) block->render(canvas);
    return true;
}

DateRenderer::DateRenderer(Renderer *block, uint16_t from, uint16_t to) {
    this->block = block;
    this->from = from;
    this->to = to;
}

DateRenderer::~DateRenderer() {
    if (this->block) delete this->block;
}

bool DateRenderer::render(Canvas *canvas) {
    const uint32_t epoch = canvas->epochTime();
    if (epoch == 0) return true;

    const time_t time = epoch;
    struct tm *timeinfo = localtime(&time);
    uint16_t today = ((timeinfo->tm_mon + 1) << 8) + timeinfo->tm_mday;

    bool inWindow = false;
    if (from < to) inWindow = (from <= today) && (today < to);
    else inWindow = (from <= today) || (today < to);

    printf("today %d/%d from %d/%d to %d/%d %d\n", today >> 8, today & 0x0ff, from >> 8, from & 0x0ff, to >> 8, to & 0x0ff, inWindow);

    if (inWindow) block->render(canvas);
    return true;
}
