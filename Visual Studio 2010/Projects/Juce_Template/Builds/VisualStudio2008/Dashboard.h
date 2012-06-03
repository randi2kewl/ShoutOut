/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: Dashboard.h
*	Class Description:
*		The dashboard class pretty much contains all the information
*	for the currently logged in account while also serving as
*	the container for the buttons/labels/images.
*
*/

#ifndef __DASHBOARD_H
#define __DASHBOARD_H

#include "Account.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include <list>


//class Account;

class Dashboard : public Component, public ButtonListener
{

public:

	//default constructor
	Dashboard();

	//constructor that builds the logged in account and master list
	Dashboard(Account*, list<Account*>*);

	//destructor.. it eats your children
	~Dashboard(){ deleteAllChildren(); }

	//a pointer to the master account list
	list<Account*> * getAccountList() { return acctList; }

	//returns a pointer to the currently logged in account
	Account * getAccount() { return acct; }

	//updates the following label (would add follower too but it would be useless)
	void updateFollowingCount();

	//does actions based on the button clicked
	void buttonClicked(Button*);

	//depricated..removing later
	Button * getLogoutButton() { return logoutButton; }

private:

	//container for all the menu goodies
	Component *container;

	//labels and button pointers
	Label *welcome;
	TextButton *logoutButton;
	Label *followerCountLabel;
	Label *followingCountLabel;
	Label *newShoutLabel;
	TextButton *mainMenuButton;
	TextButton *shoutButton;

	//master list
	list<Account*> *acctList;

	//logged in account
	Account *acct;

};
#endif