#ifndef COLLISONMANAGER_H
#define COLLISONMANAGER_H

#include <utility>
#include <vector>
#include <Ogre.h>
#include "GameConstants.h"


using std::pair;

using namespace Ogre;

class Puck;	// Forward reference (there is a circular dependency between CollisionManager and Puck).
class PuckManager;

class CollisionManager {

public:
	static CollisionManager * instance(SceneManager * sceneMgr = 0) ;
	
	pair<bool,Ogre::String> checkCollisions( Entity const * const entity );

	void handleCollisions( Puck * originPuck, Puck * targetPuck );
		

protected:
	CollisionManager( SceneManager * sceneMgr );

private:
	static CollisionManager * mInstance;	
	Ogre::SceneManager * mSceneMgr;
};

#include "Puck.h"
#include "PuckManager.h"
#endif
