- optimize draw routines by using QuickDraw's "Offscreen Graphics Worlds", which (I believe) will solve the
  problem of us redrawing the screen on EVERY cell change, rather than only once when we're done updating the whole
  thing.
    - ugh this isn't supported before System 7 I think, so we'll have to try something else
    - unless the animation is desired, but even then some optimizations could be done i think
- maybe some math optimizations
