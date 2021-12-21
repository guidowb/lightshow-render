#include "Renderers.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

#define printf(fmt, ...) {}

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
    this->canvas = canvas;
    printf("Segment from %d to %d\n", from, to);
    return renderer->render(this);
}

void SegmentRenderer::setPixel(uint16_t pixel, RGBA color) {
    canvas->setPixel(pixel + from, color);
}

uint16_t SegmentRenderer::getSize() {
    return to - from;
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

    before->render(canvas);
    this->canvas = canvas;
    this->ratio = (time * 1000) / duration;
    after->render(this);
    return false;
}

void FadeRenderer::setPixel(uint16_t pixel, RGBA color) {
    color.a = (color.a * this->ratio) / 1000;
    canvas->setPixel(pixel, color);
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

    before->render(canvas);
    this->canvas = canvas;
    this->ratio = (time * 1000) / duration;
    after->render(this);
    return false;
}

void WipeRenderer::setPixel(uint16_t pixel, RGBA color) {
    uint16_t cut = (getSize() * this->ratio) / 1000;
    if (pixel <= cut) canvas->setPixel(pixel, color);
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

    this->offset = time / pps;
    this->canvas = canvas;
    block->render(this);
    return true;
}

void RotateRenderer::setPixel(uint16_t pixel, RGBA color) {
    uint16_t npixel = (pixel + offset) % getSize();
    canvas->setPixel(npixel, color);
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

    uint32_t time = canvas->sceneTime();

    before->render(canvas);
    if (time > duration) {
        this->canvas = canvas;
        return after->render(this);
    }
    return false;
}

uint32_t AfterRenderer::sceneTime() {
    return canvas->sceneTime() - duration;
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

    this->canvas = canvas;
    if (block->render(this)) {
        this->clockSkew = sceneTime;
    }
    return true;
}

uint32_t RepeatRenderer::sceneTime() {
    return canvas->sceneTime() - clockSkew;
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
