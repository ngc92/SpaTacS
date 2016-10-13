## Version 0.5 (player update)
  * UI:
    - [ ] waypoints for move command
    - [x] show currently active targets
    - [ ] pause game
    - [x] select target velocity
  * game:
    - [x] shield radius
  * physics:
    - [x] fixtures
  * AI:
    - [ ] switch ammo based on target status
    - [x] evasive/aggressive maneuvers for damaged ships
    
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
    - [ ] energy management
  * game:
    - [ ] load levels from xml
    - [ ] custom combat
    - [ ] add a few ship types
  * code:
    - [ ] units in xml files

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