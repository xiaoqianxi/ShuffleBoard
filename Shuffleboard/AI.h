#ifndef AI_H
#define AI_H

#include "PuckManager.h"

class AI {
public:

	static AI * instance( bool easyMode = true );
	
	void takeTurn();

private:
	bool mEasyMode;

protected:
	AI(bool easyMode);
	static AI * mInstance;

};

#endif