# MULTIDIRECTIONAL SCROLLER DESIGN NOTES

## Tests to be done

- [ ] Double buffering horizontal scrolling demo
    - [ ] Basic VFB/RFB transfer
    - [ ] Scrolling routines with different pixel increments
    - [ ] FPS counter for performance comparison
- [ ] Adaptation of `technique-1` for comparison with horizontal scrolling
- [ ] Optimization 1: slow down Vscroll to match Hscroll speed while keeping a reasonable framerate for the game: different sync policies for Hscroll and Vscroll
- [ ] Optimization 2: do not scroll everything, but only the affected zones (either Hscroll or Vscroll).
    - [ ] Prepare some diagrams and video explanation of the Optimization 2 schema

## Initial performance analysis

How vertical scrolling with double buffering is done (via virtual framebuffer, VFB, and the real framebuffer, RFB):

- Everything happens on the VFB, which is transferred to the RFB on every frame
- The VFB is slightly larger than the RFB, one tile row at the top
- The RFB is drawn from the VFB by adjusting the initial line offset. This way, we can get the scrolling effect without having to scroll down the VFB on every frame.
- The VFB is scrolled down only when the top tile row is empty. That is, if the tiles are H pixels high, only once every H frames. It is scrolled in H pixel increments.
- The scroll down is done with a fast loop (LDDR) that can even be optimized more with unrolled LDD or stack based transfer (although it is less important, since it is only run sporadically). With a single LDI/LDD instruction per 8 pixels, we can scroll down an arbitrary offset
- We get the single-pixel scroll effect via the offset of the first line, but the real scroll is done much coarsely, and most important, _much less often_.
- As a result of the previous two considerations, vertical scrolling can be done very fast.

How horizontal scrolling wigh double buffering is done:

- Since the VFB and RFB are byte-aligned, we cannot use the offset trick for scrolling less than 8 pixels. This means that horizontal scrolling needs to be done for the whole VFB on every frame
- The redraw of the extra side bars of W-pixel wide tiles is done again ocassionally, every W frames (when the side bars are empty because of scrolling)
- Horizontal scrolling uses about 4 instructions per 8 scrolled pixels, just for scrolling 1 pixel (left or right)
- Specialized routines need to be done for scrolling in other increments (2, 4, etc. pixels) with similar or higher count of instructions/pixel
- As a result of the previous considerations, horizontal scrolling is at least W times slower when scrolling in 1 pixel increments, and even more if more than 1-pixel scrolling is wanted

Conclusions:

- The ZX screen memory layout is much more favorable to vertical than horizontal software scrolling
- Horizontal scrolling is much more demanding in terms of CPU usage than vertical
- Heavy optimizations will need to be done to make horizontal scrolling work at a speed comparable to vertical scrolling, so that multidirectional scroll does not look strange (i.e. fast when going up-down and much slower when left-right)

## Optimization notes

- Optimization 1: slow down Vscroll to match Hscroll speed while keeping a reasonable framerate for the game. Optimize horizontal scroll as much as possible, and use different sync policies for Hscroll and Vscroll.

- Optimization 2: do not scroll everything, but only the affected zones (either Hscroll or Vscroll). Similar to what was done in SP1 scrolling, `partial-inv-2` example. Have a list of all visible tiles and scroll only the areas covered by them.
    - We need two tile lists to know what tiles anter and exit the screen when scrolling, one for H scroll and other for V scroll
    - The lists are sorted, so all element addition/removal is done at the ends of the lists
    - Since we can move in either direction (up-down for vertical, left-right for horizontal) we need to add and remove tiles from the visible list either from the beginning or the end of the list (since the list is ordered).
    - This means that the lists have to be doubly-linked lists
    - We can merge both lists in a single quad-linked list, where each node has two H pointers for previous/next in H list, and two V pointers for vertical previous/next in V list.
    - WIth this schema, the insertion/deletion time is almost constant and quite fast, which is very desirable for smooth scrolling

