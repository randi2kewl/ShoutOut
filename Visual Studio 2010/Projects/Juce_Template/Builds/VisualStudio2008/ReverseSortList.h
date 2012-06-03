/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: ReverseSortList.h
*	Class Description:
*		Functor for comparing usernames from Z-A
*
*/

#ifndef __REVERSESORTLIST_H
#define __REVERSESORTLIST_H

#include "Account.h"

class Account;

class ReverseSortList
{
public:
	bool operator()(Account *lhv,Account *rhv) 
	{
		return lhv->getUsername() >  rhv->getUsername();
	}
};
#endif