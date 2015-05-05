#include "GameConstants.h"

Ogre::Real const GameConstants::COEFF_KINETIC_FRICTION = 0.08f;		// For curling stone on ice...0.0168;
Ogre::Real const GameConstants::GRAVITY = 9.80f;					// Force of gravity...9.8 m/s
Ogre::Real const GameConstants::MASS = 1;							// Assume mass of 1Kg for the puck.

Ogre::Real const GameConstants::UPDATE_GRANULARITY = 0.03f;			// Update physics subsystem every 1/30 second (.05 = 1/20)

// Update physics subsystem every 1/100 second
Ogre::Real const GameConstants::UPDATE_PHYSICS_GRANULARITY = 0.01f;
// Update rendering subsystem every 1/30 second
Ogre::Real const GameConstants::UPDATE_RENDERING_GRANULARITY = 0.03f;

// Multiplier value for the puck force (increase the number to make the force build up more quickly).
Ogre::Real const GameConstants::PUCK_FORCE_MULTIPLIER = 0.004f;

Ogre::Real const GameConstants::MIN_X = 1.51f;
Ogre::Real const GameConstants::MAX_X = 8.76f;
Ogre::Real const GameConstants::MIN_Z = -17.8f;


Ogre::Real const GameConstants::LAUNCH_Y = 2.4f;
Ogre::Real const GameConstants::LAUNCH_Z = 17.0f;


Ogre::Real const GameConstants::SCORING_LINE_1 = -1.85f;
Ogre::Real const GameConstants::SCORING_LINE_2 = -7.0f;
Ogre::Real const GameConstants::SCORING_LINE_3 = -12.15f;

Ogre::Real const GameConstants::FOUL_LINE = 1.75f;

Ogre::Vector3 const GameConstants::MIN_EXTENTS_OUTER(10, 7, 5);
Ogre::Vector3 const GameConstants::MAX_EXTENTS_OUTER(14.2, 7.5, 5.5);

Ogre::Vector3 const GameConstants::MIN_EXTENTS_INNER(10.05, 7.05, 5);
Ogre::Vector3 const GameConstants::MAX_EXTENTS_INNER(10.1, 7.45, 5.05);
