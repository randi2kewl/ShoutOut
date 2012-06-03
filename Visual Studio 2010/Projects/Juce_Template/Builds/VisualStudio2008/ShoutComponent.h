/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: ShoutComponent.h
*	Class Description:
*		Shout Component is for the actual submission of a 
*	new shout.
*
*	NOTE: Private messages - "DS @username ....words...."
*
*/

#ifndef SHOUTCOMPONENT_H
#define SHOUTCOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Dashboard.h"
#include "Account.h"
#include "MainMenu.h"

class Account;

class ShoutComponent : public Component, public ButtonListener
{
public:
	//constructor
	ShoutComponent(Dashboard*);

	//destructor
	~ShoutComponent(){deleteAllChildren();}

	void buttonClicked(Button*);

	//void paint(Graphics&);

private:
	//component to hold te shoutbox
	Component *shoutBox;

	//shoutbox components
	Label *shoutLabel;
	Label *instructShoutLabel;
	TextEditor *shoutText;
	TextButton *submitShoutButton;
	TextButton *cancelShoutButton;

	void clickedButton(Button *);

	//holds the dashboard with the account data in it
	Dashboard *dash;

};

#endif