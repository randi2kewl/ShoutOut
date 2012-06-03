/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: SortList.h
*	Class Description:
*		Functor that sorts the list A-Z
*
*/

#ifndef __SORTLIST_H
#define __SORTLIST_H

#include "Account.h"

class Account;

class SortList
{
public:
	bool operator()(Account *lhv,Account *rhv) 
	{
		return lhv->getUsername() <  rhv->getUsername();
	}
};
#endif