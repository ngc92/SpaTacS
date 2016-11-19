## Version 0.10
  * UI:
    - [ ] limit render thread fps
    - [ ] present levels in sensible order
  * game:
    - [ ] configurable shield radius
    - [ ] allow initial spawn velocity in level files
  * AI:
    - [ ] prevent stacking
  * physics:
    - [ ] improve advanced collision
    - [ ] more tests for advanced collision
  * code:
    - [ ] clean separation between in and out events 
            of game simulation
    - [ ] better separation between core and game classes.
    - [ ] test whether it makes sense to use CEGUI
    
## Version 1.0
  * UI:
    - [ ] allow management of multiple weapons
    - [ ] select multiple ships
  * game:
    - [ ] rockets and missiles 
  * code:
    - [ ] ECS rewrite
      + [ ] Signatures
      + [ ] Convenience layer
      + [ ] Better parameter support for Systems (i.e 
         pass Entity, pass Entity ID etc)
    - [ ] Separate user input from irrlicht rendering.

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
 * dynamic movement targets
 * level scripting


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

