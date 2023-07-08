# SP1 MULTIDIRECCIONAL SCROLLING

This document explains the different experiments with doing multidirectional scrolling wth the SP1 sprite and background management library.

It is based on my previous vertical scrolling experiments which can also be found in this very same repository.

Ideas to test:

- [ ] Create basic scroll routines, establish framerate baselines for all 8 directions (UDLR and the diagonals)
- [ ] Create scroll mapping routine: big map, viewport, positioning and scrolling
- [ ] Try assembler optimizations first, compare with baselines:
    - [ ] Reduce instruction count per loop, then unroll loops
- [ ] Optimize SP1 updates
    - [ ] Apply Partial-2 technique to multiscroll (quad-linked list of tiles, etc.)
- [ ] Attribute scrolling (useful for 8-pixel scrolling)

## Basic scroll routines and framerate baselines

## Scroll map management

## Assembler optimizations for scroll routines

## Partial-2 optimization

## Attribute scrolling

