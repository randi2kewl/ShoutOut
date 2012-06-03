/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: Registration.h
*
*/

#ifndef __REGISTRATION_H
#define __REGISTRATION_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "Account.h"
#include <list>


class Registration : public Component, public ButtonListener
{
public:
	Registration(list<Account*>*);
	~Registration() { deleteAllChildren(); }


	void buttonClicked(Button*);

private:
	TextEditor *usernameText;
	TextEditor *passwordText;
	TextEditor *repasswordText;
	TextEditor *emailText;
	TextEditor *firstnameText;
	TextEditor *lastnameText;

	Label *titleLabel;
	Label *usernameLabel;
	Label *passwordLabel;
	Label *repasswordLabel;
	Label *emailLabel;
	Label *firstnameLabel;
	Label *lastnameLabel;

	TextButton *submitButton;
	TextButton *resetButton;

	list<Account*> *acctList;
};
#endif