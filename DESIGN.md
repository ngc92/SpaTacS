# SpaTacS

## Setting
...

## Weapons
...

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
              Only useable at large ranges, expensive, can only carry few.

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
Propulsion can be provided by two different ship components. Engines can only produce thrust in a single direction,
but are fuel efficient and can provide significant acceleration. 
Maneuvering thrusters can provide thrust in arbitrary directions and rotate the ship, but are less fuel efficient and
can only cause very slow accelerations.
It is also important to note that the maximum acceleration a ship can achieve is limited by what its crew can sustain. 
There should not be more than 3g for longer periods of time, and burst acceleration should not exceed 15g.
This limitation does not exist for purely robotic drones.

There are three different kinds of engines:
* **Combustion Engine**: Propulsion by means of burning fuel. Cheap, very fuel intensive. Immune to EMP.
* **Ion Engine**: Propulsion by accelerating electrically charged particles. Very fuel efficient, high power consumption. Limited maximum thrust. An ion drive can share the reactor with the ships gauss weapon systems.
* **Fusion Drive**: Propulsion by "burning" fuel using fusion. Fuel efficient, does not require reactor power, but also cannot feed the fusion energy back into the ships systems. 

Maneuvering thrusters can only be combustion or ion based.

## Ship Classes
* **Light Fighter** Small, agile ship with low operational length and weak weapons that cannot penetrate the armour of larger vessels. Useful for screening, torpedo defence and to intercept light enemy fighters.
* **Heavy Fighter** Slightly larger version of the light fighter that can fire small missiles. Its main use is to kill enemy fighters and bombers, but it can also damage light vessels.
* **Bomber** A modified version of the fighter that sacrifices some agility and the main gun for the possibility to carry a torpedo and large-caliber rockets. A bomber can be easily defetead by enemy fighters, but its oversized armament make it a danger to larger enemy vessels.
* **Partol Ship** Reconnaissance vessel with only weak armament but high quality sensors and long-range scanners. 
* **Corvette**
* **Frigate**
* **Light Cruiser**
* **Heavy Cruiser**
* **Gunship**
* **Battlecruiser**
* **Battleship**
* **Escort Carrier**
* **Carrier**

Ship           |     Size    | Operational Range  |    Armament     | Agility      |  Armour | Ship's boats |   Engine
----------------|------------|--------------------|-----------------|--------------|---------|--------------|------------
Light Fighter    |   20 m    | 2000 km            | Gun, Rockets    | very high    |  20 mm  |   -          | Combustion
Heavy Fighter    |   25 m    | 2000 km            | Gun, Missiles   | very high    |  25 mm  |   -          | Combustion
Bomber           |   25 m    | 3000 km            | Rockets, Torpedo  | high       |  15 mm  |   -          | Combustion
Partol Ship      |   50 m    | 10000 km           | Gun Turret      | high         |  25 mm  |   -          | Combustion
Corvette         |   60 m    | 500 Mm             | Gun Turrets     | high         |  75 mm  |  1 fighter   | Fusion
Frigate          |   80 m    | 3 AU               | Gauss Turrets   | low          |  75 mm  |   -          | Ion
Light Cruiser    |  100 m    | 5 AU               | Gun Turrets, Missiles | medium | 150 mm  |  2 fighters  | Fusion
Heavy Cruiser    |  200 m    | 5 AU               | Gauss, Msl, Trp | low          | 250 mm  |  5 ftr       | Ion
Gunship          |  150 m    | 500 Mm             | Railgun, Trp    | low          | 100 mm  |   -          | Ion
Battlecruiser    |  500 m    | 10 AU              | Gun, Gauss, Mls, Trp | medium  | 350 mm  |  5 ftr, 1 pt | Fusion
Battleship       |  800 m    | 10 AU              | Railgun, Gauss, Mls, Trp | low | 500 mm  | 5 ftr, 1 cvt | Fusion
Escort Carrier   |  400 m    | 10 AU              | Gun Turrets     | very low     | 150 mm  | 10 ftr, 2 cvt| Ion
Carrier          |  500 m    | 20 AU              | Gun Turrets, Trp | very low    | 250 mm  | 15 ftr, 5 cvt| Ion


## Damage Model
...

## Micro AI
...

## AI Player
...

Stuff to consider:
https://en.wikipedia.org/wiki/Spall
https://en.wikipedia.org/wiki/All_or_nothing_(armor)
