## Version 0.6 (menu update)
  * UI:
    - [x] main menu
      + [x] start level
      + [ ] settings
      + [x] exit
    - [x] gfx for enemy status
  * game:
    - [ ] load levels from xml
    - [x] add cruiser ship type
  * code:
    - [x] restructure Irrlicht handling to allow for menus
    - [ ] ship component entities 
    - [x] get rid of all render code in tools
  * physics
    - [ ] runtime unit loading + tests

## Version 0.7 ()
  * game:
    - fuel affecting total mass
  * code:
    - shield decay as ship property
    
## Easy projects:
* Asteroids
  - passive objects
  - block shots and cannot be flown through
  - need some visualization

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