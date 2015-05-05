#ifndef PUCKCOMPARER_H
#define PUCKCOMPARER_H

#include "Puck.h"

class PuckComparer {
public:
	bool operator() ( Puck const * p1, Puck const * p2 ) const;
};


#endif

