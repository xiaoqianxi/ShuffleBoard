#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <Ogre.h>

class GameConstants {
public:

// static integral constants can be initialized inside the
// class interface as shown below.
// Other types such as static floats and non-const statics must be
// initialized outside the class interface.
static int const sPLANE_HEIGHT = 1500;
static int const sPLANE_WIDTH = 400;

// Max angle permitted for rotation of arrow
static int const MAX_ANGLE = 20;


// Max/min force permitted for launch of puck.
static int const MAX_FORCE = 9;

// Multiplier value for the puck force 
static Ogre::Real const PUCK_FORCE_MULTIPLIER; 

// Important dimensions for the Shuffleboard.
static Ogre::Real const MIN_X;
static Ogre::Real const MAX_X;
static Ogre::Real const MIN_Z;

// Launch coordinates for the Puck.
static Ogre::Real const LAUNCH_Y;
static Ogre::Real const LAUNCH_Z;

// Z coordinates of scoring lines.
static Ogre::Real const SCORING_LINE_1;
static Ogre::Real const SCORING_LINE_2;
static Ogre::Real const SCORING_LINE_3;

// Z coordinate of foul line.
static Ogre::Real const FOUL_LINE;


// Scores associated with each scoring line.
static int const SCORING_LINE_1_SCORE = 1;
static int const SCORING_LINE_2_SCORE = 2;
static int const SCORING_LINE_3_SCORE = 3;

static Ogre::Vector3 const MIN_EXTENTS_OUTER;
static Ogre::Vector3 const MAX_EXTENTS_OUTER;

static Ogre::Vector3 const MIN_EXTENTS_INNER;
static Ogre::Vector3 const MAX_EXTENTS_INNER;


// Default number of pucks used in the game (4 per side).
static int const sNUMBER_PUCKS = 8;



static Ogre::Real const COEFF_KINETIC_FRICTION;
static Ogre::Real const GRAVITY;
static Ogre::Real const MASS;


static Ogre::Real const UPDATE_GRANULARITY;	 

static Ogre::Real const UPDATE_PHYSICS_GRANULARITY;
static Ogre::Real const UPDATE_RENDERING_GRANULARITY;
};
#endif