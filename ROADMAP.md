## Version 0.7 ()
  * UI:
    - [ ] improve enemy status view
  * game:
    - [x] fuel affecting total mass
    - [ ] fuel properties
    - [ ] ammunition
  * code:
    - [ ] shield decay as ship property
    - [ ] allow lambdas as systems in component mgr
    - [ ] improve fuel management code
    
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