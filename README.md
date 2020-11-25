# LightShow

LightShow is a library that enables LED string light patterns to be
specified using a simple language syntax. It is implement in straight
C++ to allow it to be used on cheap Arduino based controllers (but there's
nothing Arduino-specific about it).

## Pattern Language

A pattern is a sequence of commands, that are executed in order to draw each
frame of the LED string.
Built-in patterns are specified by a reserved name, and a set of parameters.
Commands are separated by line breaks.

```text
<color>
solid <color>
dots <spacing> <color>...
```

### Planned Enhancements

To reduce the need for deep nesting in many common cases,
commands that affect settings may take an optional block as argument.
If such a block is provided, the setting will apply only to that block
and revert to its original value after that. If no block is provided,
the setting affects subsequent commands in the same block.

```text
brightness 80%
solid white

brightness 80%
    solid white
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

The lightdow-render library is test driven and comes with cppunit tests.
Unit tests cover everything except renderer output (as tests probing all pixels
for correct values would be really expensive to maintain).
Rendering is instead covered by visual testing - below.

### Visual testing

Lightshow comes with an OSX Swift app that reads a collection of lightshow pattern files,
and uses the lightshow library to render the resulting patterns in a Mac window.
This enables quick visual testing of rendering results.
Animations are rendered as a succession of frames,
similarly enabling quick visual validation of dynamic behavior.
