#include "PuckManager.h"

PuckManager * PuckManager::mInstance = 0;

PuckManager::PuckManager( Ogre::SceneManager *sceneMan, int pucks ) :
	mSceneMgr( sceneMan ), 
	mPucks( pucks ) {	
		createPucks(pucks);	  
}

void PuckManager::createPucks(int pucks) {
	int i = 0;
	for ( int i = 0; i < pucks ; i++ ) {
		if ( ( i % 2 ) == 0 ) {
			mFreePucks.push_back( new Puck( mSceneMgr, "Yellowstone" + Ogre::StringConverter::toString(i+1), Puck::YELLOWSTONE ) );
		}
		else {
			mFreePucks.push_back( new Puck( mSceneMgr, "Redstone" + Ogre::StringConverter::toString(i+1), Puck::REDSTONE ) );
		}
	}
}

void PuckManager::update( bool updateRendering ) {
	std::vector<Puck *>::iterator curr = mUsedPucks.begin(); //Request an iterator to the first puck in the used list.
	std::vector<Puck *>::iterator end = mUsedPucks.end();    //Gets an iterator that points to “one past the end?of the vector

	//Step through each puck in the list of used pucks.
	//An iterator works like a pointer and always refers to one particular element in a data structure at a time.
	for ( ; curr != end ; ++curr ) {
		(*curr)->updatePhysics();
		if( updateRendering )
			(*curr)->updateRendering();
	}
}


// Get a puck from the free list.
// Warning...returns 0 if no more pucks available.
Puck * PuckManager::getNextPuck() {
	Puck * puck = 0;			// returns 0 if no free pucks available.
	
	if(mFreePucks.size() != 0)
	{
		puck = mFreePucks.at(mFreePucks.size()-1);
		mUsedPucks.push_back(puck);
		mFreePucks.pop_back();

	}


	return puck;
}



std::vector<Puck *> & PuckManager::getUsedPucks() {
	return mUsedPucks;
}

PuckManager * PuckManager::instance(Ogre::SceneManager * sceneMgr, int pucks ) {
	if (mInstance == 0)  {  
		mInstance = new PuckManager( sceneMgr, pucks );
	}
    return mInstance;
}

Puck * PuckManager::getPuckByName( Ogre::String const & puckName ) const {
	std::vector<Puck *>::const_iterator curr = mUsedPucks.begin();
	std::vector<Puck *>::const_iterator end = mUsedPucks.end();
	for ( ; curr != end ; ++curr ) {
		if ( (*curr)->getName().compare( puckName ) == 0 ) {
			return *curr;
		}
	}
	// curr == end, i.e. no matching puck, so return null.
	return 0;
}

bool PuckManager::allPucksStopped() const {
	bool pucksStopped = true;
	std::vector<Puck *>::const_iterator curr = mUsedPucks.begin();
	std::vector<Puck *>::const_iterator end = mUsedPucks.end();
	for ( ; curr != end ; ++curr ) {
		if ( (*curr)->getVelocity() > 0 ) {
			pucksStopped = false;
		}
		else {
			pucksStopped = true;
		}
		  //return true;
	}

	return pucksStopped;
}



Puck::PuckType PuckManager::getNextPuckType() const {
 	// return the puck colour at the rear of the free list
	Puck * next = mFreePucks.at(mFreePucks.size()-1);
	return next->getPuckType();
}