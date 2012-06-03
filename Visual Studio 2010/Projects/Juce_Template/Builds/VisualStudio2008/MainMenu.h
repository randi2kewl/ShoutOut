/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: MainMenu.h
*
*/

#ifndef __MAINMENU_H
#define __MAINMENU_H

#include "Account.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "Dashboard.h"
#include "Search.h"
#include "ViewFollowers.h"
#include "ViewFollowing.h"
#include "ViewShouts.h"

class Account;

class MainMenu : public Component, public ButtonListener
{
public:
	MainMenu();
	MainMenu(Dashboard *);
	~MainMenu(){ deleteAllChildren(); }

	void buttonClicked(Button*);

private:
	TextButton *searchButton;
	TextButton *followerButton;
	TextButton *followingButton;
	TextButton *shoutsButton;
	Label *newShoutLabel;
	Label *mainMenuLabel;

	Dashboard *dash;
};
#endif