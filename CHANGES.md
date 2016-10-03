## Version 0.3 (damage update)
  * controls:
    - switch ammo command
  * UI:
    - show ammo info, hull status
    - show hit chance on attack move
  * game:
    - hitpoints for components
    - projectile damage depends on relative velocity
    - add Hull component
    - ammo types (SP, AP, HE)
  * physics:
    - use units library in all physics code.
    - collision tests.

## Version 0.2 (UI Update)
 * controls:
   - move camera
 * UI:
   - Show attack info (hit chance)
   - Correctly interpolate object movements
   - Show currently selected ship's target
   - moveable camera
   - ship names
 * game:
   - power generator component
   - engine: generate force instead of acceleration
 * AI:
   - move ships that are under fire
 * physics:
   - masses and real forces
   - vector tests

## Version 0.1 (Initial Version)
 * Mouse control of ships:
   - attack
   - move
 * AI:
   - just attack
 * game:
   - single weapon type (projectile weapon)
   - simple shields
   - simple engine
   - base power 1
   - load all component descriptions from xml
 * UI:
   - ship positions
   - projectiles        
   - hit effect
   - ship shield/hp info
   - status of active ship
 * physics:
   - objects without orientation
   - produce collision events
   - handle acceleration integration
   