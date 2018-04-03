# SpaTacS

## Setting
...

## Weapons
### Turrets and Guns
 * **Turrets**: Can (and need to) rotate to fire at a specific target. Rotation happens with respect to the turrets axis. The angle w.r.t. that axis is restricted to an interval [a, b]. 
                For intervals of the form [0, b] the turret shoots along its axis, for [a, 90°] the turret rotates around that axis and has a maximum elevation of a. If we add ship customization 
				turrets should be easily customizable.
				There are three types of turrets:
   - Gun turret: Kinetic energy of the projectile is supplied at the moment of fire by the ammunition. 
   - Rocket Launcher: Supplies a small amount of kinetic energy at fire, shoots self propelled ammunition.
   - Gauss Cannon: Supplies significant kinetic energy to the projectile but requires a huge amount of energy.
 * **Launch Tube**: Launches homing projectiles. Cannot rotate. 
   - Missile Launcher: Small, homing missiles
   - Torpedo Tube: Huge, expensive, devastating torpedos.
 * **Rail Gun**: A huge version of the gauss cannon, cannot be rotated. Needs to be installed along the length of the ship, so it cannot easily replaced with a different weapon. 
                 Has its own, separate battery / capacitor.
 
Weapon         | Energy Consumption | Effective Range  | Projectile | Launch Speed
----------------|-------------------|------------------|------------|--------------|
Gun turret       |   low            | low              | Shell      | ~ 1 - 10 km/s
Rocket Launcher  |   low            | medium           | Rocket     | ~ 10 - 100 m/s
Gauss Cannon     |   medium         | low - medium     | Shell      | ~ 2 - 20 km/s
Missile Launcher |   low            | medium           | Missile    | ~ 20 m/s
Torpedo Tube     |   low            | high             | Torpedo    | ~ 10 m/s
Rail Gun         |   high           | medium - high    | Shell      | ~ 100 km/s

### Projectiles
* **Shell**: A dumb piece of metal with a warhead on top. Flies in straight lines.
* **Rocket**: A dumb, self propelled projectile. Can reach (at longer ranges) higher velocities than shells and thus is suitable for somewhat larger ranges.
* **Missile**: A homing projectile. Has an acceleration phase (thrust head on toward target) , a cruise phase (almost no thrust) and a homing phase (course corrections when approaching the target).
* **Torpedo** A highly intelligent, lethal projectile. Homes towards a target while also performing evasive maneuvers againt defence guns. Lower acceleration than missiles, but significantly more fuel. 
              Only useable at large ranges.

### Warheads
* **Armour Piercing** Increased chance of penetrating armour, but low structural damage. Does only localized, piercing damage.
* **High Explosive** Low chance of penetrating armour (but devastating if it does). Does explosive, blunt damage (dangerous to periperie e.g. sensors, turrets) and spawns several smaller metal spliters that do piercing damage.
* **HEAT** A combination of the above. Does not spawn additional splinters.
* **EMP** An electromagentic pulse that fries all nearby sensors and target computers.


### Fuses
* **Impact** Projectile detonates on impact
* **Timed** Projectile detonates after given time
* **Proximity** Projectile detonates when it closes in on the target.

## Sensors and Countermeasures
...

## Propulsion
...

## Damage Model
...

## Micro AI
...

## AI Player
...