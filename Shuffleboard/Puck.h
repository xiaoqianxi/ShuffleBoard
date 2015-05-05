#ifndef PUCK_H
#define PUCK_H

#include <vector>
#include "GameConstants.h"
#include "CollisionManager.h"
//#include "TextRenderer.h"

class Puck {

public:
	enum PuckType {YELLOWSTONE,REDSTONE};

	Puck( Ogre::SceneManager *sceneMan, Ogre::String const & puckName, PuckType type );

	void updatePhysics();

	void updateRendering();

	void activate( Ogre::Vector3 const & pos);

	void applyForce( Ogre::Real const & force );

	void rotate();

	void removeFromBoard();

	Ogre::Entity * getEntity() const;   //Get reference to the entity

	Ogre::String getName() const;

	Ogre::Vector3 getPosition() const;

	Ogre::Vector3 getDirection() const;

	Ogre::Real getVelocity() const;

	PuckType getPuckType() const;

	bool isVisible() const;	

	void setVelocity( Ogre::Real const & velocity );

	void setDirection( Ogre::Vector3 const & dir, bool rotate = true );
	
	
private:
	Ogre::Real mForceKineticFriction;
	Ogre::Real mAcceleration;
	Ogre::Real mDeceleration;	
	Ogre::Real mVelocity;	
	Ogre::Vector3 mDirection;		// Direction vector for the puck.

	Ogre::SceneManager *mSceneMgr;
	Ogre::Entity * mEntity;
	Ogre::SceneNode *mNode;		// The scene node this puck is attached to.
	Ogre::String mPuckName;

	PuckType mPuckType;
	bool mIsVisible;
	Ogre::Vector3 mLastPos;
	void updateMotion();	// helper method to update velocity, acceleration.

};

#endif