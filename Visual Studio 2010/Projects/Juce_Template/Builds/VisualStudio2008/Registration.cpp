#include "Registration.h"

Registration::Registration(list<Account*> *aList)
{
	acctList = aList;

	titleLabel = new Label(T("Registration Label"), T("Create A New Account:"));
	titleLabel->setBounds(200,40,200,20);
	this->addAndMakeVisible(titleLabel);

	usernameLabel = new Label(T("Username Label"), T("Username:"));
	usernameLabel->setBounds(50,80,200,20);
	this->addAndMakeVisible(usernameLabel);

	usernameText = new TextEditor(T("Username"));
	usernameText->setBounds(210,80,200,20);
	this->addAndMakeVisible(usernameText);

	passwordLabel = new Label(T("Password Label"), T("Password:"));
	passwordLabel->setBounds(50,110,200,20);
	this->addAndMakeVisible(passwordLabel);

	passwordText = new TextEditor(T("Password"), '*');
	passwordText->setBounds(210,110,200,20);
	this->addAndMakeVisible(passwordText);

	repasswordLabel = new Label(T("Repassword Label"), T("Re-enter Password"));
	repasswordLabel->setBounds(50,140,200,20);
	this->addAndMakeVisible(repasswordLabel);

	repasswordText = new TextEditor(T("Repass"), '*');
	repasswordText->setBounds(210,140,200,20);
	this->addAndMakeVisible(repasswordText);

	emailLabel = new Label(T("Email"),T("Email:"));
	emailLabel->setBounds(50,170,200,20);
	this->addAndMakeVisible(emailLabel);

	emailText = new TextEditor(T("Email"));
	emailText->setBounds(210,170,200,20);
	this->addAndMakeVisible(emailText);

	firstnameLabel = new Label(T("First Name"), T("First Name:"));
	firstnameLabel->setBounds(50,200,200,20);
	this->addAndMakeVisible(firstnameLabel);

	firstnameText = new TextEditor(T("First Name Text"));
	firstnameText->setBounds(210,200,200,20);
	this->addAndMakeVisible(firstnameText);

	lastnameLabel = new Label(T("Last Name"), T("Last Name:"));
	lastnameLabel->setBounds(50,230,200,20);
	this->addAndMakeVisible(lastnameLabel);

	lastnameText = new TextEditor(T("Last name text"));
	lastnameText->setBounds(210,230,200,20);
	this->addAndMakeVisible(lastnameText);

	submitButton = new TextButton(T("Submit"),T("Submit new account"));
	submitButton->setBounds(150,280,100,20);
	this->addAndMakeVisible(submitButton);

	resetButton = new TextButton(T("Reset"),T("Reset text fields"));
	resetButton->setBounds(260,280,100,20);
	this->addAndMakeVisible(resetButton);

	submitButton->addListener(this);
	resetButton->addListener(this);
}

void Registration::buttonClicked(Button *button)
{
	if(button == resetButton)
	{
		
	} else if(button == submitButton)
	{
		if(passwordText->getText() == repasswordText->getText())
		{
			std::string username(usernameText->getText().toUTF8());
			std::string password(passwordText->getText().toUTF8());
			std::string email(emailText->getText().toUTF8());
			std::string fname(firstnameText->getText().toUTF8());
			std::string lname(lastnameText->getText().toUTF8());
			Account *acct = new Account(username, password, email, fname, lname);
			acctList->push_back(acct);

			deleteAllChildren();
			//recreate and show the main login screen again
			MainComponent *mainComp = new MainComponent(acctList);
			mainComp->setBounds(0,0,600,400);
			addAndMakeVisible(mainComp);
		} else {
			AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, T("Password Match Error"),T("Both passwords need to match"));
		}
	}
}