#include "Puck.h"


Puck::Puck( Ogre::SceneManager *sceneMan, Ogre::String const & puckName, PuckType type ) : 
	mSceneMgr( sceneMan ),
	mForceKineticFriction( GameConstants::COEFF_KINETIC_FRICTION * GameConstants::MASS * GameConstants::GRAVITY ),
	mDeceleration( mForceKineticFriction / GameConstants::MASS ),	
	mAcceleration(0),
	mVelocity(0),
	mDirection(0,0,0),
	mPuckName(puckName),
	mIsVisible( false ),
	mPuckType( type ) {
	
	//Ogre::Entity *ent;
	

    // Start here...
	if(type == YELLOWSTONE)
	{
		mEntity = mSceneMgr->createEntity(mPuckName, "Yellowstone.mesh");
		//mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeYellowStone");	    
	}
	else
	{
		mEntity = mSceneMgr->createEntity(mPuckName, "Redstone.mesh");
	    	   
	}
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("node" + puckName);
	mNode->scale(1.5f,1.5f,1.5f);
	mNode->attachObject(mEntity);
	mNode->setPosition(0,0,1000);

}

void Puck::updatePhysics() {		
	
	// Only update if this puck is visible and moving.
	if ( mIsVisible && mVelocity > 0 ) {	
		// Check for collision...
		std::pair<bool, Ogre::String> result = CollisionManager::instance()->checkCollisions( mEntity );
		// If a collision occurred.
		if ( result.first == true ) {
			//roll back to old position
			mNode->setPosition( mLastPos );
			String name = result.second;
			Puck *collidee = PuckManager::instance()->getPuckByName(name);
			CollisionManager::instance()->handleCollisions(this, collidee);

		}
			
			
	}
	
}
void Puck::updateRendering() {
	mLastPos = mNode->getPosition();
	// When node is yawed 180, TS_LOCAL switches z directions!
	mNode->translate( mDirection * GameConstants::UPDATE_RENDERING_GRANULARITY * mVelocity, Ogre::Node::TS_PARENT );	
	updateMotion();

}

void Puck::updateMotion() {	
		if ( mAcceleration > -mDeceleration )  {
			// Subtract deceleration due to friction from acceleration.	
			mAcceleration -= mDeceleration;
		}

		// Add acceleration to objects velocity.
		if ( Ogre::Math::IFloor(mVelocity * 10) > 0 ) {	
			mVelocity = mVelocity + ( mAcceleration * GameConstants::UPDATE_RENDERING_GRANULARITY );
			// Check if puck is out of bounds.			
			Ogre::Vector3 pos = this->getPosition();			

			if ( pos.x < GameConstants::MIN_X || pos.x > GameConstants::MAX_X || pos.z < GameConstants::MIN_Z ) {
				removeFromBoard();
			}
		}
		else { // Puck has stopped on the board.
			mVelocity = mAcceleration = 0;		

			// If puck has stopped before the foul line.
			if ( mNode->getPosition().z > GameConstants::FOUL_LINE ) {
				removeFromBoard();
			}
		}
		/*Ogre::String val = Ogre::StringConverter::toString(mAcceleration);
		TextRenderer::getSingleton().setText("Acceleration", val);
		val = Ogre::StringConverter::toString(mVelocity);
		TextRenderer::getSingleton().setText("Velocity", val);*/
}




void Puck::activate( Ogre::Vector3 const & pos ) {
	mNode->setPosition(pos);
	mDirection.z = -1; 
	mIsVisible = true;
	
}


void Puck::applyForce( Ogre::Real const & force ) {
	/*mAcceleration=force-mForceKineticFriction / GameConstants::MASS;
	if(mAcceleration > 0)
	{
		mVelocity = mAcceleration ;
	}*/
	mVelocity = force / GameConstants::MASS;

}

// This method rotates the Puck to the direction it is travelling
void Puck::rotate() {		
	Ogre::Vector3 directionFacing = mNode->getOrientation() * Ogre::Vector3::UNIT_Z;
	if ((1.0f + directionFacing.dotProduct(mDirection)) < 0.0001f) {
			mNode->yaw(Ogre::Degree(180));
	}
	else {
		Ogre::Quaternion quat = directionFacing.getRotationTo(mDirection);
		mNode->rotate(quat);
	} 
}


void Puck::removeFromBoard() {
	mIsVisible = false;
	mVelocity = 0;
	mAcceleration = 0;
	mNode->setPosition(0,0,1000);
	mNode->setVisible(false);

}

Ogre::Entity * Puck::getEntity() const {
	return mEntity;
}

Ogre::String Puck::getName() const {
	return mPuckName;
}

Ogre::Vector3 Puck::getPosition() const {	
	return mNode->getPosition();	
}

Ogre::Vector3 Puck::getDirection() const {
	return mDirection;
}

Ogre::Real Puck::getVelocity() const {
	return mVelocity;
}

Puck::PuckType Puck::getPuckType() const {
 	return mPuckType;
}


bool Puck::isVisible() const {
	return mIsVisible;
}

void Puck::setVelocity( Ogre::Real const & velocity ) {
	mVelocity = velocity;
}


void Puck::setDirection( Ogre::Vector3 const & dir, bool rotate ) {
	mDirection = dir;
	if(rotate)
	    this->rotate();
}
