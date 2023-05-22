# GAME DESIGN

## Main Game Loop

- [ ] Check controller
- [ ] Move all objects (saving previous positions):
  - [ ] Enemies
  - [ ] Hero
  - [ ] Shots
- [ ] Activate new shots if needed
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
    - [ ] Scroll down offscreen
  - [ ] Draw objects (using current position, saving background in buffers):
    - [ ] Shots
    - [ ] Hero
    - [ ] Enemies
- [ ] Sync and transfer offscreen frame to real screen
- [ ] Check events and react to them:
  - [ ] Loss of lives
  - [ ] Sound effect playing
  - [ ] End of game/phase/etc.
  - [ ] Update score/completion/time/etc.
