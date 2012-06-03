#include "MainComponent.h"
#include "FileIO.h"
#include <algorithm>
#include "Account.h"
#include <list>
#include <string>
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Registration.h"

MainComponent::MainComponent(list<Account*> *aList)
{
	//create file object that will read in accounts and shouts
	acctList = aList;

	//create instruction label
	instructLabel = new Label(T("instructLablel"),T("Login or create a new account."));
	instructLabel->setBounds(150, 50, 300, 20);
	addAndMakeVisible(instructLabel);

	//create login button
	loginButton = new TextButton(T("Login"), T("Login to existing account."));
	loginButton->setBounds(150, 200, 100, 30); 
	addAndMakeVisible(loginButton);
	
	//create account button
	createAcctButton = new TextButton(T("Register"), T("For creating new account."));
	createAcctButton->setBounds(260, 200, 100, 30);
	addAndMakeVisible(createAcctButton);

	//create username label
	usernameLabel = new Label(T("usernameLabel"), T("Username:"));
	usernameLabel->setBounds(150, 80, 100, 20);
	addAndMakeVisible(usernameLabel);
	
	//get username text
	uname = new TextEditor(T("Username"));
	uname->setBounds(150, 100, 200, 20);
	addAndMakeVisible(uname);

	//create password label
	passwordLabel = new Label(T("passwordLabel"), T("Password:"));
	passwordLabel->setBounds(150, 140, 100, 20);
	addAndMakeVisible(passwordLabel);

	//get password text with security '*'
	pass = new TextEditor(T("Password"), '*');
	pass->setBounds(150, 160, 200, 20);
	addAndMakeVisible(pass);

	//add listeners to buttons
	loginButton->addListener(this);
	createAcctButton->addListener(this);


}//end constructor

//destructor
MainComponent::~MainComponent()
{
	deleteAllChildren();
}//end destructor

void MainComponent::buttonClicked(Button *button)
{
	//check if login button was clicked
	if(button == loginButton)
	{
		//give error if they are empty
		if(uname->getText().isEmpty() || pass->getText().isEmpty())
		{
			errorMessage("Please enter a username and password to login.");
		} 
		else 
		{
			bool check = false;

			//turn the juce::String into a std::string for comparing
			std::string username(uname->getText().toUTF8());
			std::string password(pass->getText().toUTF8()); 

			////create and init iterators
			list<Account*>::iterator start, stop;
			start = acctList->begin();
			stop = acctList->end();

			//loop through the whole account list
			for(; start != stop; ++start)
			{
				//if the username and password match then run if statement
				
				if(username == (*start)->getUsername() && password == (*start)->getPassword())
				{
					this->deleteAllChildren();
					Dashboard *dashboard = new Dashboard((*start)->getAccount(), acctList);
					dashboard->setBounds(0,0,600,400);
					addAndMakeVisible(dashboard);

					check = true;
					break;
				}//end if
			}//end for loop
			
			if(check == false)
			{
				errorMessage("The username and password that you entered don't match our system.");
			}
		}//end else		
	}
	else if(button == createAcctButton)
	{
		this->deleteAllChildren();
		Registration *reg = new Registration(acctList);
		reg->setBounds(0,0,600,400);
		addAndMakeVisible(reg);
	}
}

void MainComponent::errorMessage(String message)
{
	AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, T("Login Error"),message);

}
