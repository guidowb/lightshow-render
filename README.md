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
time <from> <to> [<command>|<block>]
date <from> <to> [<command>|<block>]
brightness <percentage> [<command>|<block>]
gamma [<correction>] [<command>|<block>]
opacity [<level>] [<command>|<block>]
bounce <size> <speed> [<command>|<block>]
wrap <size> <speed> [<command>|<block>]
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
