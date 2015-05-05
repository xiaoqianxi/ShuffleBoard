#include "PuckComparer.h"

// Compares pucks by their z values.
bool PuckComparer::operator() ( Puck const * p1, Puck const * p2 ) const {
		return p1->getPosition().z < p2->getPosition().z;
}