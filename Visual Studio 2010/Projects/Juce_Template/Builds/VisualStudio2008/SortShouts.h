#ifndef __SORTSHOUTS_H
#define __SORTSHOUTS_H

#include "Shout.h"

class Shout;

class SortShouts
{
public:
	
	bool operator()(Shout *lhv, Shout *rhv)
	{
		return lhv->getDate() > rhv->getDate();
	}

};
#endif