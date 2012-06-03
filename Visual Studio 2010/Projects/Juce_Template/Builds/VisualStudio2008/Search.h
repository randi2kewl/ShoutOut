/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: Search.h
*	Class Description:
*		This is a component that displays the menu
*	options for searching and also allows for searching
*	of users.
*
*/

#ifndef __SEARCH_H
#define __SEARCH_H

#include "Account.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Dashboard.h"

class Search : public Component, public ButtonListener
{

public:

	//default constructor
	Search(){}

	//create component
	Search(Dashboard*);

	//destructor
	~Search(){ deleteAllChildren(); }

	//the view components
	void searchUserView();
	void searchUsers();

	//what to do with all those buttons
	void buttonClicked(Button *);

private:

	//labels, editors, and buttons.. Oh my!
	Label *menuLabel;
	TextButton *searchUsersButton;
	TextButton *searchShoutsButton;
	TextButton *uButton;
	TextEditor *username;
	TextButton *backButton;
	TextButton *followButton;

	//dashboard holds the master list and the current account
	Dashboard *dash;

	//temporary storage of the account searched for
	Account *searchedAccount;

};
#endif