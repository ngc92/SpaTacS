## Version 0.7 (ammunition update)
  * UI:
    - [ ] improve enemy status view
  * game:
    - [x] fuel affecting total mass
    - [ ] fuel properties
    - [ ] ammunition
    - [ ] armour penetration
    - [ ] redefined damage model
    - [ ] multiple wpn per ship
  * code:
    - [x] shield decay as ship property
    - [x] allow lambdas as systems in component mgr
    - [x] improve fuel management code, unify with energy mgm
    - [ ] allow systems without Entity parameter
    

## Easy projects:
* Skybox:
  - just load and display a skybox
  - make it configurable by level.xml
  - find some textures
  
* Asteroids:
  - passive objects
  - block shots and cannot be flown through
  - need some visualization

* Ship view:
  - a menu page that shows the stats of different ships



## Unspecified
* optimizations:
  - Spatial data structure to speed up hit tests
  - Reduce dynamic allocs
  - disable projectile/projectile collision tests in physics
* code
  - inheritance instead of variant for commands
  - collision handling: use current positions.
  - CEGUI?
* gameplay
  - ship - ship collisions
  - increase weapon accuracy when repeatedly firing on
    same target.
  - dump fuel action
  - Crew "component"
    + organisation
    + health
* UI
  - rotatable camera
  - ship icons
* AI    
* other
  - Ship Editor