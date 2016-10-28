* stability:
  - crash when trying to look up command for removed ship
* optimizations:
  - Spatial data structure to speed up hit tests
  - Reduce dynamic allocs
  - disable projectile/projectile collision tests in physics
  - limit render thread fps
* code
  - inheritance instead of variant for commands
  - collision handling: use current positions.
  - CEGUI?
  - unify random
* gameplay
  - ship - ship collisions
  - increase weapon accuracy when repeatedly firing on
    same target.
  - dump fuel action
  - Crew "component"
    + organisation
    + health
  - win game when no enemies present
* UI
  - rotatable camera
  - ship icons
  - panels
  - ingame menu
    +  main menu
    + continue
* AI    
* other
  - Ship Editor