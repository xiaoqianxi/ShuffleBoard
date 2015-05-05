#include "CollisionManager.h"

CollisionManager * CollisionManager::mInstance = 0;

// Protected constructor
CollisionManager::CollisionManager( SceneManager * sceneMgr ) : mSceneMgr( sceneMgr ) {
}


CollisionManager * CollisionManager::instance(SceneManager * sceneMgr) {
	if (mInstance == 0)  {  
		mInstance = new CollisionManager( sceneMgr );
	}
    return mInstance;
}

/**
 * Checks for a collision between the supplied entity and the entity for each Puck in the used list.
 *
 *  @param movingEntity The entity that represents the moving puck.
 *  @returns a std::pair<bool, Ogre::String>, the bool indicates hit (true) or miss (false)....the String is the name of the entity
 *     that was intersected or an empty String if no entity was intersected.
 */

pair<bool,Ogre::String> CollisionManager::checkCollisions(  Entity const * const movingEntity ) {
	String targetPuckName = "";
	bool collided = false;
	AxisAlignedBox puckBox = movingEntity->getWorldBoundingBox();	
	std::vector<Puck *> usedPucks = PuckManager::instance()->getUsedPucks();
	for ( std::vector<Puck *>::iterator curr = usedPucks.begin(); curr != usedPucks.end() && collided == false ; ++curr ) {
		// Complete this for loop...
		Puck * p = (*curr);

		AxisAlignedBox currBox = p->getEntity()->getWorldBoundingBox();

		if(currBox != puckBox && currBox.intersects(puckBox) ) {
		{
			//AxisAlignedBox currBox = p->getEntity()->getWorldBoundingBox();

				collided = true;
				targetPuckName = p->getEntity()->getName();
			}
		}

	}
	return make_pair(collided, targetPuckName);	
}


/**
* Implements a "placeholder" collision response when originPuck has collided with targetPuck.
* The correct collision respose will be developed in tutorial 5.
*
* @param originPuck The colliding puck.
* @param targetPuck The puck on the receiving end of the collision (collidee).
*/

void CollisionManager::handleCollisions( Puck * originPuck, Puck * targetPuck ) {	

	Vector3 n = originPuck->getPosition() - targetPuck->getPosition();
	n.normalise();
	Vector3 Ua = originPuck->getDirection() * originPuck->getVelocity();
	Vector3 Ub = targetPuck->getDirection() * targetPuck->getVelocity();
	
	Vector3 temp = Ua - Ub;
	Vector3 Va = Ua - (Ogre::Real)(1+0.9)/2 * temp.dotProduct(n) * n ;
	Vector3 Vb = Ub + (Ogre::Real)(1+0.9)/2 * temp.dotProduct(n) * n ;

	Vector3 originDir = originPuck->getDirection();
	originDir.z *= -1;
	originPuck->setVelocity(Va.length());
	originPuck->setDirection(originDir, false);

	Vector3 targetDir = targetPuck->getDirection();
	targetPuck->setVelocity(Vb.length());
	targetPuck->setDirection(targetDir, false);

	
}

