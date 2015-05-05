#include "Shuffleboard.h"

Shuffleboard * Shuffleboard::mInstance = 0;

//-------------------------------------------------------------------------------------
Shuffleboard::Shuffleboard( SceneManager *sceneMan ) :
	mSceneMgr( sceneMan ),
	mForce(0),
    mMinExtentsOuter( GameConstants::MIN_EXTENTS_OUTER ),
    mMaxExtentsOuter( GameConstants::MAX_EXTENTS_OUTER ),
    mMinExtentsInner( GameConstants::MIN_EXTENTS_INNER ),
    mMaxExtentsInner( GameConstants::MAX_EXTENTS_INNER ),
    mBoxOuter( mMinExtentsOuter, mMaxExtentsOuter ),
    mBoxInner( mMinExtentsInner, mMaxExtentsInner ){

	initialise();
}

//-------------------------------------------------------------------------------------
void Shuffleboard::initialise() {
	// Create the Shuffleboard.
	Entity * ent = mSceneMgr->createEntity("Shuffleboard", "Shuffleboard.mesh");
	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeShuffleboard");
	node->attachObject( ent );	//attack object shuffleboard to new sceneNode "nodeShuffleboard"
	node->setPosition( 0, 0, 0);

	// Create the Arrow.
    Entity *entA = mSceneMgr->createEntity("Arrow", "arrow.mesh");
    mArrow = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeArrow");
    mArrow->attachObject( entA );
    mArrow->setPosition( 5, 4, 15 );
    mArrow->yaw( Ogre::Degree(90) );
	
}


//-------------------------------------------------------------------------------------
Shuffleboard * Shuffleboard::instance(SceneManager * sceneMgr ) {
	if (mInstance == 0)  {  
		mInstance = new Shuffleboard( sceneMgr );
	}
    return mInstance;
}

//---------------------------------------------------------------------------------
void Shuffleboard::setArrowVisible(bool visible) {
    mArrow->setVisible( visible );
}

//-----------------------------------------------------------------------------
void Shuffleboard::rotateArrow(OIS::MouseEvent const & e) {
    Ogre::Real xMovement = e.state.X.rel * 0.1f;
    // Arrow initially faces in positive X direction.
    Ogre::Vector3 directionFacing = mArrow->getOrientation() * Vector3::UNIT_X;
    Ogre::Vector3 straightAhead(0, 0, -1);
    Ogre::Degree angle = directionFacing.angleBetween( straightAhead );
    if ( angle < Ogre::Degree((Ogre::Real)GameConstants::MAX_ANGLE) ) {
        mArrow->yaw( Ogre::Degree(xMovement * -1) );
    }
	else if ((directionFacing.x < 0 && xMovement > 0 )||(directionFacing.x > 0 && xMovement < 0 )) {
        mArrow->yaw( Ogre::Degree(xMovement * -1 ) );
    }
}

//------------------------------------------------------------------------------------
void Shuffleboard::launchPuck() {
    Puck * puck = PuckManager::instance()->getNextPuck();
	if ( puck != 0 && puck->getPuckType() == Puck::REDSTONE) {
        Ogre::Vector3 directionFacing = mArrow->getOrientation() * Vector3::UNIT_X;
		Ogre::Real centreX= ( GameConstants::MAX_X - GameConstants::MIN_X ) / 2 + GameConstants::MIN_X;
		Ogre::Vector3 launchPos= Vector3(centreX, GameConstants::LAUNCH_Y, GameConstants::LAUNCH_Z); //launch position for the puck
		
		puck->activate( launchPos );	
		puck->setDirection( directionFacing );
		mForce *= 1.8f;
		if ( mForce >= GameConstants::MAX_FORCE ) {
			mForce = GameConstants::MAX_FORCE;
		}
		puck->applyForce( mForce );

		// Reset the force and the arrow orientation.
        mForce = 0;
        resetArrowOrientation();
    }
}

void Shuffleboard::resetArrowOrientation() {
    Ogre::Vector3 direction = mArrow->getOrientation() * Vector3::UNIT_X;
    Ogre::Quaternion quat = direction.getRotationTo( Ogre::Vector3(0, 0, -1) );
    mArrow->rotate(quat);
}

//-------------------------------------------------------------------------
bool Shuffleboard::getInputForce() {

    bool getInputForce = true;
    mForce += ( Ogre::Math::Pow(2.0f, mForce ) * GameConstants::PUCK_FORCE_MULTIPLIER );
    // draw the HUD line with mTotalElapsedTime sizing x -axis.
    DebugDrawer::getSingleton().drawQuad( mBoxOuter.getAllCorners(), Ogre::ColourValue(0, 1.0f, 0) );

    if ( mMaxExtentsInner.x + mForce < mMaxExtentsOuter.x ) {
        mBoxInner.setExtents( mMinExtentsInner, Ogre::Vector3(mMaxExtentsInner.x + mForce, mMaxExtentsInner.y, mMaxExtentsInner.z ) );
    }

	/*else {
        getInputForce = false;
        mForce = GameConstants::MAX_FORCE;
    }*/
    DebugDrawer::getSingleton().drawQuad( mBoxInner.getAllCorners(), Ogre::ColourValue(1.0f, 0, 0) );
    DebugDrawer::getSingleton().build();

	return getInputForce;
}

//---------------------------------------------------------------------------------
void Shuffleboard::updateScore() {
    std::vector<Puck *> usedPucks = PuckManager::instance()->getUsedPucks();
    int score = 0;
    if ( usedPucks.empty() == false ) {
        sort( usedPucks.begin(), usedPucks.end(), PuckComparer() );
		//Get the puck type for the first puck in the list
        Puck::PuckType puckType = usedPucks.at(0)->getPuckType();
        std::vector<Puck *>::iterator curr = usedPucks.begin();
        std::vector<Puck *>::iterator end = usedPucks.end();
        for ( ; curr != end && puckType == (*curr)->getPuckType() ; ++curr ) {
            score += getScore( (*curr)->getPosition().z );
            puckType = (*curr)->getPuckType();
        }
        if ( puckType == Puck::YELLOWSTONE ) {
            TextRenderer::getSingleton().setText("YellowScore", "Yellow: " + Ogre::StringConverter::toString( score ) );
            TextRenderer::getSingleton().setText("RedScore", "Red: " + Ogre::StringConverter::toString( 0 ) );
        }
        else {
            TextRenderer::getSingleton().setText("RedScore", "Red: " + Ogre::StringConverter::toString( score ) );
            TextRenderer::getSingleton().setText("YellowScore", "Yellow: " + Ogre::StringConverter::toString( 0 ) );
        }
    }
}

//-------------------------------------------------------------------------
int Shuffleboard::getScore(Ogre::Real const & zPos) {
    int score = 0;

	if( zPos < GameConstants::SCORING_LINE_1 && zPos > GameConstants::SCORING_LINE_2 ) {
		score += GameConstants::SCORING_LINE_1_SCORE;
	}
	else if( zPos <= GameConstants::SCORING_LINE_2 && zPos > GameConstants::SCORING_LINE_3 ) {
		score += GameConstants::SCORING_LINE_2_SCORE;
	}
	else if( zPos <= GameConstants::SCORING_LINE_3 && zPos > GameConstants::MIN_Z) {
		score += GameConstants::SCORING_LINE_3_SCORE;
	}

	return score;
}

//--------------------------------------------------------------------------
void Shuffleboard::resetScore(Ogre::Real score) {
	TextRenderer::getSingleton().setText("RedScore", "Red: " + Ogre::StringConverter::toString( score ) );
    TextRenderer::getSingleton().setText("YellowScore", "Yellow: " + Ogre::StringConverter::toString( score ) );
}