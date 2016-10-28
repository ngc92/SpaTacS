## Version 0.9 (commands update)
  * UI:
    - new flight commands:
      + hold distance
      + evade
    - optimize waypoints (do not brake too much)
    - allow management of multiple weapons
    - allow setting system activity
    - select multiple ships
  * AI:
    - smart selection of ammunition (use for player too)
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

