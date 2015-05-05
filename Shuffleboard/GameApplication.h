#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include <OIS/OIS.h>

#include "BaseApplication.h"
#include "GameConstants.h"
#include "Shuffleboard.h"
#include "PuckManager.h"
#include "AI.h"

class GameApplication : public BaseApplication {
public:
	GameApplication();
	virtual ~GameApplication();

protected:
	virtual void createScene();
	virtual void destroyScene();
	virtual void createCamera();
	void resetCamera();//reset camera position
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
	// OIS::KeyListener
	bool keyPressed( const OIS::KeyEvent& evt );
	bool keyReleased( const OIS::KeyEvent& evt );

	// OIS::MouseListener
	bool mouseMoved( const OIS::MouseEvent& evt );
	bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id );

	void MoveCameraForward(const Ogre::FrameEvent& evt);


private:
	bool mIsAITurn;
	Ogre::Real mElapsedTime;
    //Ogre::Real mCurrentTime;
	Ogre::Real mCurrentPhysicsTime;
    Ogre::Real mCurrentRenderingTime;
    bool mUpdateRendering;
	Puck * mPuck;
	bool mShutDown;		
	bool mFreeLookCam;//variable
	bool mGetInputForce; // Indicator for when input force is being applied.

};

#endif 