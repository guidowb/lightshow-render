# LightShow

LightShow is a library that enables LED string light patterns to be
specified using a simple language syntax. It is implement in straight
C++ to allow it to be used on cheap Arduino based controllers (but there's
nothing Arduino-specific about it).

## Pattern Language

A pattern is described by a sequence of commands, that describe one or
multiple scenes that will be displayed on the string. Scenes are separated
by commands that describe transitions, like `fade`. Commands within a single
scene are treated as layers, with each subsequent command drawing over the
top of the previous command.

Built-in patterns and transitions are specified by a reserved name, and a
set of parameters. Commands are separated by line breaks.

```text
<color>
solid <color>
dots <spacing> <color>...
twinkle <color> [<twinkles-per-minute>]
segment <from> <to> <scene>
gradient <color> <color>...
fade <duration> <after-scene>
after <duration> <after-scene>
repeat <scene>
time <from> <to> <scene>
date <from> <to> <scene>
```

Commands that take a scene as argument can have that scene provided in
one of three ways:

- As a single command on the same line:

    ```text
    fade 1s solid black
    ```

- As a block following the initial command:

    ```text
    fade 1s
        solid black
        twinkle white
    ```

- By continuing the sequence of commands:

    ```text
    fade 1s
    solid black
    twinkle white
    ```

Indentation is significant and can be used to delineate blocks. But it
is optional, and in many common cases deep indentation can be avoided by
simply avoiding indenting blocks at all. If no indentation is used, the
block continues until the end of its containing block.

Tabs and spaces are both valid for indentation, but whichever is used,
indentation at each level must be consistent across any given block.

### Planned Patterns

```text
dashes <length> <color>...
brightness <percentage> <scene>
gamma [<correction>] <scene>
opacity [<level>] <scene>
bounce <size> <speed> <scene>
wrap <size> <speed> <scene>
```

## Code Structure

LED Strings are abstracted as a `Canvas`.
Patterns are drawn onto that canvas by `Renderer`s. Renderers can be
really simple, or they can themselves rely on other renderers to draw
more complex patterns.

Patterns are described by a simple language, and a `Compiler` translates
a string of that language into a tree of `Renderer`s that can be asked to
render the (complex) pattern onto a `Canvas` by invoking the `render()`
method on the root of the renderer tree.

The `Compiler` uses a `Parser`, and it uses a `Lexer`, to interpret the
pattern language.

### Animation

Renderers render one frame at a time. Non-animated, static renderers just
render the same pattern for every frame. But animating renderers may draw
a different pattern each time.

Frame rate is unpredictable, so animating renderers are expected to respect
the current time to determine the content of the frame to draw. For this
purpose, each `Canvas` presents two different clocks to renderers.

`globalTime()` reflects the system clock and is monotonically increasing
until it wraps (which only occurs once every 49 days or so).
That clock is the one to use for cycling animations,
and using it guarantees that if the same renderer appears in consecutive
scenes, the transition between the scenes is seamless.

The second clock is the `sceneTime()`, which is guaranteed to start at zero
when the scene that contains the renderer first appears. This is the clock
that works for one-shot animations, such as transitions between scenes.

Renderers that wrap scenes want to know when those scenes have complated.
Thus renderers that perform a one-shot animation should return `false`
from their `render()` methods until their scene completes. All other
renderers, including ones that animate cycles, should always return true
to allow wrapped sequences to proceed.

## Testing

### Unit testing

The lightshow-render library is test driven and comes with cppunit tests.
Unit tests cover everything except renderer output (as tests probing all pixels
for correct values would be really expensive to maintain).
Rendering is instead covered by visual testing - below.

### Visual testing

Lightshow comes with an OSX Swift app that reads a collection of lightshow pattern files,
and uses the lightshow library to render the resulting patterns in a Mac window.
This enables quick visual testing of rendering results.
Animations are rendered as a succession of frames,
similarly enabling quick visual validation of dynamic behavior.
