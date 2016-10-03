## Version 0.4 (systems update)
  * UI:
    - [ ] system status view
    - [ ] shield inidcator
    - [ ] energy management
  * game:
    - [ ] FuelTank component
    - [ ] PowerBank component
    - [ ] LifeSupport component
    - [ ] fuel and power consumption curves
    - [ ] overheating of systems
  * code:
    - [ ] allow component interaction (for crew <-> life support)
    - [ ] enable ship types in SpawnShip Event
    - [ ] separate shield and shield generator, hull not a system
    - [ ] sort energy management code
    - [ ] unify random

## Version 0.5 (combat update)
  * UI:
    - [ ] waypoints for move command
    - [ ] show currently active targets
    - [ ] smoother scrolling
    - [ ] pause game
  * game:
    - [ ] change weapon modes into actual ammo types
    - [ ] rework damage model
  * code:
    - [ ] do not associate a physics::Object with a game object.
  * AI:
    - [ ] switch ammo based on target status
    - [ ] may focus fire on single target
    - [ ] evasive maneuvers for damaged ships
    
## Version 0.6 (menu update)
  * UI:
    - main menu
  * game:
    - load levels from xml

## Unspecified
* optimizations:
  - Spatial data structure to speed up hit tests
  - Reduce dynamic allocs
  - disable projectile/projectile collision tests in physics
* code
  - physics world as ECS?
  - inheritance instead of variant for commands
  - collision handling: use current positions.
  - ship - ship collisions
* gameplay
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