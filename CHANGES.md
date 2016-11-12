## Version 0.8
  * UI:
    - multiple levels
    - show energy production/consumption
    - show current ammo data
    - set ammo type for all weapons simultaneously
  * game:
    - set weapon loadout and fuel in level file
    - add a few levels

## Version 0.7
  * UI:
    - improved enemy status view
  * game:
    - fuel affects total mass
    - ammunition
    - armour penetration
    - redefined damage model
    - multiple weapons per ship
    - minute variations in projectile speed

## Version 0.6
  * UI:
    - main menu
    - gfx for enemy status
  * game:
    - load levels from xml
    - added cruiser ship type
    - settings (from file)
  * physics
    - runtime unit loading

## Version 0.5
  * UI:
    - waypoints for move command
    - show currently active targets
    - pause game
    - select target velocity
  * game:
    - shield radius
  * physics:
    - fixtures
  * AI:
    - switch ammo based on target status
    - evasive/aggressive maneuvers for damaged ships

## Version 0.4
  * UI:
    - system status view
    - shield indicator
  * game:
    - FuelTank component
    - LifeSupport component
    - overheating of shield generator

## Version 0.3
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

## Version 0.2
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

## Version 0.1
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
   