## Version 0.8 (UI upgrade)
  * UI:
    - [x] multiple levels
    - [ ] enable/disable detailed status view
      - [ ] show everything with hitpoints
      - [ ] show energy production/consumption
      - [ ] show activity degree
    - [ ] show current ammo data
    - [ ] allow setting system activity
    - [ ] set ammo type for all weapons simultaneously
    - [ ] ingame menu
      + [ ] main menu
      + [ ] continue
    - [ ] mark currently selected ships
  * AI:
    - [ ] detect when out of ammo type
  * game:
    - [ ] engine efficiency
    - [ ] define weapon loadout/fuel in level file
    - [ ] turn on/off systems
    - [ ] add a few levels
    - [ ] win game when no enemies present
  * code:
    - [x] remove need for physics events, simplify physics code
    - [ ] clean up UI code. 
    - [x] power units for energy management


## Version 0.9 (commands update)
  * UI:
    - new flight commands:
      + hold distance
      + evade
    - optimize waypoints (do not brake too much)
    - allow management of multiple weapons
  * physics:
    - allow applying impulse during callback
    - write tests for unit io
    - write test  for advanced collision handling
  * code:
    - clean separation between in and out events of game simulation
    - opaque typedefs for IDs

## Version 1.0
The plan is to add nothing "major" until this version, 
but significantly improve what is there now, and flesh out 
the content a bit. Also, a lot of code cleanup has to happen.

Major changes that are planned for after 1.0 are
 * orientation: give ships an orientation, and add firing 
    angles etc.
 * AI unit cooperation 
 * sound
 * networking


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
  - include useful secondary information

