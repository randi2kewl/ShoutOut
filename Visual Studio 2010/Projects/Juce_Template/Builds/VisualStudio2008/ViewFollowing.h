/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: ViewFollower.h
*	Class Description:
*		Component for showing the following list
*
*/

#ifndef __VIEWFOLLOWING_H
#define __VIEWFOLLOWING_H

#include "Dashboard.h"
#include "Account.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "SortList.h"

class ViewFollowing : public Component, public ButtonListener
{
public:

	//constructors
	ViewFollowing();
	ViewFollowing(Dashboard *);

	//destructors
	~ViewFollowing(){ deleteAllChildren(); }

	//required operations
	void paint(Graphics& g);
	void resized();
	void buttonClicked(Button*);

	//better debugging
	juce_UseDebuggingNewOperator


private:

	//private members
	ViewFollowing (const ViewFollowing&);
	const ViewFollowing& operator= (const ViewFollowing&);
	Dashboard *dash;
	ListBox *listBox;
	TextButton *unfollowButton;
	TextButton *a_zButton;
	TextButton *z_aButton;
};
#endif