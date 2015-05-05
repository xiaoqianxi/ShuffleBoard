#ifndef SHUFFLEBOARD_H
#define SHUFFLEBOARD_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <algorithm>

#include "GameConstants.h"
#include "PuckManager.h"
#include "DebugDrawer.h"
#include "TextRenderer.h"
#include "PuckComparer.h"

using namespace Ogre;

class Shuffleboard {
public:	
	
	static Shuffleboard * instance(SceneManager * sceneMgr = 0 );

	void initialise();

	void setArrowVisible(bool visible = true);

	void rotateArrow(OIS::MouseEvent const & e);

	void launchPuck();

	void resetArrowOrientation();

	bool getInputForce();

	void updateScore();

	int getScore(Ogre::Real const & zPos);

	void resetScore(Ogre::Real score = 0);

protected:
	Shuffleboard( SceneManager *sceneMan );

	SceneManager *mSceneMgr;		// The overall scene manager.
	
	static Shuffleboard * mInstance;

	Ogre::SceneNode *mArrow; // The scene node to which the arrow is attached.

	// The input force to apply to the puck.
    Ogre::Real mForce;
    // Store top left corner of outer rectangle.
    Ogre::Vector3 mMinExtentsOuter;
    // Store bottom right corner of outer rectangle.
    Ogre::Vector3 mMaxExtentsOuter;
    // Store top left corner of inner rectangle.
    Ogre::Vector3 mMinExtentsInner;
    // Store bottom right corner of inner rectangle.
    Ogre::Vector3 mMaxExtentsInner;
    // Boxes for the HUD which will be drawn.
    Ogre::AxisAlignedBox mBoxInner;
    Ogre::AxisAlignedBox mBoxOuter;

};



#endif

