## Version 0.6 (menu update)
  * UI:
    - [ ] main menu
      + [ ] start level
      + [ ] custom combat
      + [ ] settings
      + [ ] exit
    - [ ] pause menu
      + [ ] resume
      + [ ] end game
  * game:
    - [ ] load levels from xml
    - [ ] custom combat
    - [ ] add a few ship types
  * code:
    - [ ] restructure Irrlicht handling to allow for menus


## Version 0.7 (engine update)
  * UI
  * code
    - [ ] game entities 
  * game
  * physics

## Unspecified
* optimizations:
  - Spatial data structure to speed up hit tests
  - Reduce dynamic allocs
  - disable projectile/projectile collision tests in physics
* code
  - inheritance instead of variant for commands
  - collision handling: use current positions.
* gameplay
  - ship - ship collisions
  - increase weapon accuracy when repeatedly firing on
    same target.
  - Crew "component"
    + organisation
    + health
* UI
  - rotatable camera
  - ship icons
  - skybox
* AI    
* other
  - Ship Editor