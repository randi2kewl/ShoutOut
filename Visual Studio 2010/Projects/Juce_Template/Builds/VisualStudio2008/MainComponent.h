/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: MainComponent.h
*
*/

#ifndef __MAINCOMPONENT_H
#define __MAINCOMPONENT_H

#include "../../Source/MainWindow.h"
#include "Dashboard.h"
#include "Account.h"
#include <list>


class MainComponent : public Component , public ButtonListener
{
public:

	//constructor
	MainComponent(list<Account*>*);
	
	//destructor
	~MainComponent();

	//operations
	void errorMessage(String error);

	//required operations
	void buttonClicked(Button *);

private:

	//private member vars
	TextButton *loginButton;
	TextButton *createAcctButton;
	Label *instructLabel;
	TextEditor *uname;
	TextEditor *pass;
	Label *usernameLabel;
	Label *passwordLabel;

	//master account list
	list<Account*> *acctList;

};
#endif