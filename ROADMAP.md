## Version 0.9 (commands update)
  * UI:
    - [ ] new flight commands:
      + [ ] hold distance
      + [ ] evade
    - [ ] optimize waypoints (do not brake too much)
    - [ ] allow management of multiple weapons
    - [ ] allow setting system activity
    - [ ] UI panels
    - [ ] select multiple ships
  * game:
    - [ ] powerplant requires fuel
  * AI:
    - [x] smart selection of ammunition
  * physics:
    - [x] allow applying impulse during callback
    - [x] write tests for unit io
    - [ ] write test  for advanced collision handling
  * code:
    - [ ] clean separation between in and out events of game simulation
    - [x] remove non UI code from UI
    - [x] opaque typedefs for IDs
    
## Version 0.10
  * game:
    - rockets and missiles

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
  
* Asteroids:
  - passive objects
  - block shots and cannot be flown through
  - need some visualization

* Ship view:
  - a menu page that shows the stats of different ships
  - include useful secondary information

