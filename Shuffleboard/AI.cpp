#include "AI.h"

AI * AI::mInstance = 0;

AI::AI( bool easyMode ) :
	mEasyMode( easyMode ) 
{	
		 
}

//------------------------------------------------------------------------------------------------
void AI::takeTurn(){
	Puck * puck = PuckManager::instance()->getNextPuck();
	if(puck!=0){
		Ogre::Real centreX;
	    Ogre::Vector3 launchPos;

		//launch position for the puck
		centreX = ( GameConstants::MAX_X - GameConstants::MIN_X ) / 2 + GameConstants::MIN_X;
	    launchPos = Vector3( centreX, GameConstants::LAUNCH_Y, GameConstants::LAUNCH_Z );

		puck->activate( launchPos );	
		puck->applyForce( Ogre::Math::RangeRandom(5, 9));
	}
}