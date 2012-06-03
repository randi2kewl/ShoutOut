/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: ViewShouts.h
*	Class Description:
*		Container for the shouts list component
*/

#ifndef __VIEWSHOUTS_H
#define __VIEWSHOUTS_H

#include "Dashboard.h"
#include "Account.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Shout.h"
#include <list>
#include <sstream>
#include <string>

class ViewShouts : public Component, public ButtonListener
{
public:

	//constructors
	ViewShouts();
	ViewShouts(Dashboard *);

	//destructors
	~ViewShouts(){ deleteAllChildren(); listBox = nullptr; }

	//required operations
	void paint(Graphics& g);
	void resized();
	void buttonClicked(Button*);

	//better debugging
	juce_UseDebuggingNewOperator


private:

	//private members
	ViewShouts (const ViewShouts&);
	const ViewShouts& operator= (const ViewShouts&);
	Dashboard *dash;
	ListBox *listBox;
	TextButton *reshoutButton;
	
	
};
#endif