* stability:
  - physics problem when applying impulse 0: Add testcase, 
    and fix the problem
  - crash when trying to look up command for removed ship
* optimizations:
  - Spatial data structure to speed up hit tests
  - Reduce dynamic allocs
  - disable projectile/projectile collision tests in physics
* code
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
  - strong impacts on shield affect shield generator
  - afterburners
* UI
  - rotatable camera
  - ship icons
  - panels
  - ingame menu
    + main menu
    + continue
* AI    
* other
  - Ship Editor
  
  
  
REWORK:
  - [ ] Hit responses
  - [ ] Event Loop
  - [ ] Cleanup (remove dead Objects)
  - [ ] Despawn Event
  - [ ] Cleanup (remove physics Objects that are no longer needed)
  - [ ] Propulsion (Object Level)
  - [ ] Subcomponents