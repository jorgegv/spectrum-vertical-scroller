# GAME DESIGN

## Main Game Loop

- [ ] Check controller
- [ ] Move all objects (saving previous positions):
  - [ ] Enemies
  - [ ] Hero
  - [ ] Shots
- [ ] Process hero shots
  - [ ] Activate new shots if needed (controller indicates "Fire" was pressed)
  - [ ] Delete shots that have gone off-screen
- [ ] Check collisions
  - [ ] Enemies with hero
  - [ ] Enemies with shots
- [ ] Redraw offscreen frame
  - [ ] Erase objects in order (using previous positions and saved buffers):
    - [ ] Enemies
    - [ ] Hero
    - [ ] Shots
  - [ ] Update background
    - [ ] Check and update top row (bring new bg from map)
    - [ ] Scroll down offscreen frame
  - [ ] Draw objects in reverse order as erasing (using current position, saving background in buffers):
    - [ ] Shots
    - [ ] Hero
    - [ ] Enemies
- [ ] Sync and transfer offscreen frame to real screen
- [ ] Check events and react to them:
  - [ ] Loss of lives
  - [ ] Sound effect playing
  - [ ] End of game/phase/etc.
  - [ ] Update score/completion/time/etc.

## Notes

- All sprites (enemies, hots, hero) should have 2 different status flags:

  - SPRITE_IS_ACTIVE: indicates wether the sprite record is active and should be processed in the Move/Erase/Draw parts of the main loop. This flag should be the same at the beginning and end of a given loop run, so that objects that "disappear" (be they enemies, shots or hero) are properly erased and the background is restored.

  - SPRITE_SHOULD_BE_DELETED: indicates that the game logic (collision checks, animations, shots going off-screen...) has determined that the object should disappear from the view. This means that it will be erased (i.e. the background restored) and will not be drawn again, and also that the SPRITE_IS_ACTIVE flag will be set to false (so that it is not processed in subsequent game loop runs)
