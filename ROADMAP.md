## Version 0.4 (systems update)
  * UI:
    - [x] system status view
    - [x] shield indicator
    - [ ] energy management
  * game:
    - [x] FuelTank component
    - [ ] PowerBank component
    - [x] LifeSupport component
    - [ ] fuel and power consumption curves
    - [ ] overheating of systems
  * code:
    - [x] allow component interaction (for crew <-> life support)
    - [x] hull not a system
    - [x] rework position plotting

## Version 0.5 (combat update)
  * UI:
    - [ ] waypoints for move command
    - [ ] show currently active targets
    - [ ] pause game
  * game:
    - [ ] change weapon modes into actual ammo types
    - [ ] rework damage model
    - [ ] shield radius
  * code:
    - [ ] make physics ECS
      + [ ] components: kinematic state, mass, shape, userdata
      + [ ] systems: update, collision
  * physics:
    - [ ] more collision tests
    - [ ] hit scans
  * AI:
    - [ ] switch ammo based on target status
    - [ ] may focus fire on single target
    - [ ] evasive maneuvers for damaged ships
    
## Version 0.6 (menu update)
  * UI:
    - [ ] main menu
  * game:
    - [ ] load levels from xml
    - [ ] custom combat
    - [ ] add a few ship types

## Unspecified
* optimizations:
  - Spatial data structure to speed up hit tests
  - Reduce dynamic allocs
  - disable projectile/projectile collision tests in physics
* code
  - physics world as ECS?
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