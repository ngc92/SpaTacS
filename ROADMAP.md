## Version 0.9 (commands update)
  * UI:
    - [ ] new flight commands:
      + [ ] hold distance
      + [ ] evade
    - [x] optimize waypoints (do not brake too much)
    - [x] allow setting system activity
    - [ ] UI panels
  * game:
    - [x] powerplant requires fuel
  * AI:
    - [x] smart selection of ammunition
  * physics:
    - [x] allow applying impulse during callback
    - [x] write tests for unit io
    - [x] write test  for advanced collision handling
  * code:
    - [x] remove non UI code from UI
    - [x] opaque typedefs for IDs
    
## Version 0.10
  * UI:
    - [ ] allow management of multiple weapons
    - [ ] select multiple ships
  * game:
    - [ ] rockets and missiles 
    - [ ] configurable shield radius
  * physics:
    - [ ] improve advanced collision
    - [ ] more tests for advanced collision
  * code:
    - [ ] clean separation between in and out events of game simulation

## Version 1.0
The plan is to add nothing "major" until this version, 
but significantly improve what is there now, and flesh out 
the content a bit. Also, a lot of code cleanup has to happen.

## The future
Major changes that are planned for after 1.0 are
 * orientation: give ships an orientation, and add firing 
    angles etc.
 * AI unit cooperation 
 * sound
 * networking
 * moving frames of reference


## Easy projects:
* Skybox:
  - just load and display a skybox
  - make it configurable by level.xml
  - find some textures

* Settings menu:
  - an in game menu to configure the settings
  - save settings to xml  

* Loadout menu:
  - set ammo etc you take with you into a mission
  
* Asteroids:
  - passive objects
  - block shots and cannot be flown through
  - need some visualization

* Ship view:
  - a menu page that shows the stats of different ships
  - include useful secondary information

