/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: ViewFollowers.h
*	Class Description:
*		Container for the followers list component
*
*/

#ifndef __VIEWFOLLOWERS_H
#define __VIEWFOLLOWERS_H

#include "Dashboard.h"
#include "Account.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class ViewFollowers : public Component, public ButtonListener
{
public:

	//constructors
	ViewFollowers();
	ViewFollowers(Dashboard *);

	//destructors
	~ViewFollowers(){ deleteAllChildren(); }

	//required operations
	void paint(Graphics& g);
	void resized();
	void buttonClicked(Button*);

	//better debugging
	juce_UseDebuggingNewOperator


private:

	//private members
	ViewFollowers (const ViewFollowers&);
	const ViewFollowers& operator= (const ViewFollowers&);
	Dashboard *dash;
	ListBox *listBox;
	TextButton *a_zButton;
	TextButton *z_aButton;
};
#endif