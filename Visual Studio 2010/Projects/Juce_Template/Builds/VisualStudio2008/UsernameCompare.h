/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: UsernameCompare.h
*	Class Description:
*		Functor for comparing if usernames match
*
*/

#ifndef __USERNAMECOMPARE_H
#define __USERNAMECOMPARE_H

#include "Account.h"
#include "Search.h"

class UsernameCompare
{
public:
	bool operator()(const Account *lhv,const Account *rhv)
	{ 
		return lhv->getUsername() == rhv->getUsername();
	}
};
#endif