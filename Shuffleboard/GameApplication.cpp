#include "GameApplication.h"

using namespace Ogre;


//-------------------------------------------------------------------------------------
GameApplication::GameApplication() : 
	mElapsedTime(0),
    mCurrentPhysicsTime(0),
    mCurrentRenderingTime(0),
    mUpdateRendering( false ),
	mShutDown( false ),
	mFreeLookCam( false ),
	mGetInputForce( false ),
	mIsAITurn(false)
{
	mPuck = PuckManager::instance()->getNextPuck();
}

//-------------------------------------------------------------------------------------
GameApplication::~GameApplication() {
	
}

//-------------------------------------------------------------------------------------
void GameApplication::createCamera() {
	// Create the camera here...
	mCamera = mSceneMgr->createCamera("BoardCamera");//create
	mCamera->setNearClipDistance(1);

	mCamera->setPosition( Vector3(5, 12, 21 ) );
    mCamera->pitch( Ogre::Degree(-35 ) );

	mSceneMgr->getCamera("BoardCamera");//reference

	// create a default camera controller.
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);

	Ogre::Real speed = mCameraMan->getTopSpeed();//slow down the camera movement
    mCameraMan->setTopSpeed( speed / 4 );

}

//-------------------------------------------------------------------------------------   
void GameApplication::createScene() {
	// Initialise the DebugDrawer singleton
    new DebugDrawer(mSceneMgr, 0.5f);

	// Initialise the TextRenderer singleton
    new TextRenderer();
    TextRenderer::getSingleton().addTextBox("RedScore", "", 900, 10, 1100, 30,
    Ogre::ColourValue::Red);
    TextRenderer::getSingleton().addTextBox("YellowScore", "", 1100, 10, 1300, 30,
    Ogre::ColourValue(1.0f, 1.0f, 0) );

	// Create your scene here...	
	Ogre::Real centreX = ( GameConstants::MAX_X - GameConstants::MIN_X ) / 2 + GameConstants::MIN_X;
    Ogre::Vector3 launchPos( centreX, GameConstants::LAUNCH_Y, GameConstants::LAUNCH_Z );

	// Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(15,15,3);

	// Create the PuckManager singleton.
    PuckManager::instance( mSceneMgr, 10 );

	// Create the Shuffleboard singleton
    Shuffleboard::instance( mSceneMgr );   
	//mPuck = new Puck(mSceneMgr,"Yellow",Puck::YELLOWSTONE);


	//Create the AI singleton
	bool easyPlayMode = true;
	AI::instance(easyPlayMode);
	
	//	
	if(mPuck == 0){
		TextRenderer::getSingleton().addTextBox("GameOver", "", 400, 100, 1000, 130, Ogre::ColourValue::Red);

	}
		
}


//-------------------------------------------------------------------------------------   
void GameApplication::destroyScene() {

}

// KeyListener
bool GameApplication::keyPressed(const OIS::KeyEvent &e) {	
	//Puck * puck = PuckManager::instance()->getNextPuck();
	
	switch (e.key) {
	case OIS::KC_ESCAPE: 
		mShutDown = true;
		break;
	
	case OIS::KC_SPACE:
		if (PuckManager::instance()->allPucksStopped() == true && mIsAITurn == false ) {
		    Shuffleboard::instance()->setArrowVisible(false);
			mGetInputForce = true;
		}
		break;

	default:
		break;

	//‘T? is pressed, then we switch the mFreeLookCam flag to false and call resetCamera() function
    case OIS::KC_T:
        if ( mFreeLookCam ) {
            mFreeLookCam = false;
            resetCamera();
        }
        else {
            mFreeLookCam = true;
        }
        break;
	}	
	
	mCameraMan->injectKeyDown(e);
	return true;
}

bool GameApplication::keyReleased(const OIS::KeyEvent &e) {
	//Puck * puck = PuckManager::instance()->getNextPuck();
	//Puck::PuckType puckType = puck->getPuckType();
	switch (e.key) {
        case OIS::KC_SPACE:
            mGetInputForce = false;
            if ( PuckManager::instance()->allPucksStopped() ) {
                Shuffleboard::instance()->launchPuck();			
            }
			
            break;
        default:
            break;
    }
	mCameraMan->injectKeyUp(e);
	return true;
}

bool GameApplication::mouseMoved(const OIS::MouseEvent &e) {					
		
	Shuffleboard::instance()->rotateArrow( e );
	return true;
}

bool GameApplication::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	return true;
}

bool GameApplication::mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	//Game over state
	if (id == OIS::MB_Left){
		if(mPuck == 0){
			Shuffleboard::instance()->resetScore(0);
	        PuckManager::instance( mSceneMgr, 10 );
		}
	}
	return true;
}


//-------------------------------------------------------------------------------------
bool GameApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{		
	//Elapsed time since last frame
	mElapsedTime = evt.timeSinceLastFrame;
	mCurrentPhysicsTime += evt.timeSinceLastFrame;
    mCurrentRenderingTime += evt.timeSinceLastFrame;

	//If the user releases the space bar, mGetInputForce is turned to false
	if ( mGetInputForce ) {
        mGetInputForce = Shuffleboard::instance()->getInputForce();
    }

	if ( mCurrentPhysicsTime > GameConstants::UPDATE_PHYSICS_GRANULARITY ) {
		mCurrentPhysicsTime -= GameConstants::UPDATE_PHYSICS_GRANULARITY;
		if ( mCurrentRenderingTime > GameConstants::UPDATE_RENDERING_GRANULARITY ) {
			mCurrentPhysicsTime -= GameConstants::UPDATE_RENDERING_GRANULARITY;
			mUpdateRendering = true;
		}
		PuckManager::instance()->update( mUpdateRendering );
		//Refresh the DebugDrawer object.
		DebugDrawer::getSingleton().build();
		mUpdateRendering = false;
	}

	if (mWindow->isClosed()) { 
		return false;
	}
	if (mShutDown) {
		return false;
	}
	
	mKeyboard->capture();
	mMouse->capture();
	
	mTrayMgr->frameRenderingQueued(evt);

	// For free look camera style..
	mCameraMan->frameRenderingQueued(evt);




	if ( !mFreeLookCam ) {
        mCameraMan->manualStop();//prevent its movement by calling the built in manualStop function
    }

	if (mGetInputForce == false && PuckManager::instance()->allPucksStopped() == true && PuckManager::instance()->getNextPuckType() == Puck::YELLOWSTONE ) {
		mIsAITurn = true;
		// Now call a function in the AI class to take a turn
		AI::instance()->takeTurn();
		Shuffleboard::instance()->updateScore();
		MoveCameraForward(evt);//update camera
		//turn AI back to false
		mIsAITurn = false;
	}

	if (mGetInputForce == false && PuckManager::instance()->allPucksStopped() == true && PuckManager::instance()->getNextPuckType() == Puck::REDSTONE ) {
		mIsAITurn = false;
		Shuffleboard::instance()->setArrowVisible(true);
		Shuffleboard::instance()->updateScore();
	}
    else {
        Shuffleboard::instance()->setArrowVisible(false);
    }
	//Dont want the HUB visible when the puck is launched.
	DebugDrawer::getSingleton().clear();

	if(mPuck == 0){
		TextRenderer::getSingleton().setText("GameOver", "Game Over! Click to Restart! " );

	}
	else{
		TextRenderer::getSingleton().removeTextBox("GameOver");
	}

	if( PuckManager::instance()->allPucksStopped() == true){
		resetCamera();
	}

	return true;
}

//allow reset camera position
void GameApplication::resetCamera() {
	
	mCamera->setPosition( Vector3(5, 12, 21 ) );

}

void GameApplication::MoveCameraForward(const Ogre::FrameEvent& evt) {

	Ogre::Real centreX= ( GameConstants::MAX_X - GameConstants::MIN_X ) / 2 + GameConstants::MIN_X;
	Ogre::Vector3 dir= Vector3(centreX, GameConstants::LAUNCH_Y, GameConstants::LAUNCH_Z);

	Ogre::Real speed = 10;
    //Ogre::Vector3 dir = mCamera->getDirection();
    dir.normalise();

    mCamera->move( dir*speed * evt.timeSinceLastFrame );
}


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "macUtils.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		GameApplication app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif
