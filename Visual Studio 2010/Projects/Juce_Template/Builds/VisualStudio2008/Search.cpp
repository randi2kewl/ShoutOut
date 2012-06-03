#include "Search.h"

Search::Search(Dashboard *dashboard)
{
	this->dash = dashboard;

	menuLabel = new Label(T("Menu Label"), T("Search Menu:"));
	menuLabel->setBounds(0, 0, 180, 20);
	addAndMakeVisible(menuLabel);

	searchUsersButton = new TextButton(T("Search Users"), T("Search for other users"));
	searchUsersButton->setBounds(0, 30, 100, 20);
	addAndMakeVisible(searchUsersButton);
	searchUsersButton->addListener(this);

	searchShoutsButton = new TextButton(T("Search Shouts"), T("Search for shouts"));
	searchShoutsButton->setBounds(0, 60, 100, 20);
	addAndMakeVisible(searchShoutsButton);
	searchShoutsButton->addListener(this);

}//end constructor

void Search::buttonClicked(Button *button)
{
	if(button == searchUsersButton)
	{
		searchUserView();
		
	} else if(button == uButton)
	{
		searchUsers();
	} else if(button == backButton)
	{
		searchUserView();
	} else if(button == followButton)
	{
		if(!dash->getAccount()->findUsernameMatch(dash->getAccount(),searchedAccount->getUsername()))
		{
			if(dash->getAccount()->getUsername() != searchedAccount->getUsername())
			{
				dash->getAccount()->addFollowing(searchedAccount);
				dash->updateFollowingCount();
				searchedAccount->addFollower(dash->getAccount());
			}
			else
			{
				AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, T("Follow Error"),T("You cannot follow yourself!"));
			}
			
		}
		else
		{
			AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, T("Follow Error"),T("You are already following this person!"));
		}
	}
	
}//end method buttonClicked

void Search::searchUsers()
{
	list<Account*>::iterator start, stop;
	start = dash->getAccountList()->begin();
	stop = dash->getAccountList()->end();

	std::string uname = username->getText().toUTF8();

	for(; start != stop; ++start)
	{
		if((*start)->getUsername() == uname)
		{
			String user = username->getText();
			searchedAccount = *start;

			this->deleteAllChildren();

			Label *unameLabel = new Label(T("Uname Label"), T("Username:"));
			unameLabel->setBounds(0,0,100,20);
			this->addAndMakeVisible(unameLabel);

			Label *usernameLabel = new Label(T("Username Label"), user);
			usernameLabel->setBounds(110,0,100,20);
			this->addAndMakeVisible(usernameLabel);

			Label *foundLabel = new Label(T("First Name"), T("First Name:"));
			foundLabel->setBounds(0,30,100,20);
			this->addAndMakeVisible(foundLabel);

			Label *fnameLabel = new Label(T("Fname"), String((*start)->getFirstName().c_str()));
			fnameLabel->setBounds(110,30,100,20);
			this->addAndMakeVisible(fnameLabel);

			Label *lnameLabel = new Label(T("Lname Label"), T("Last Name:"));
			lnameLabel->setBounds(0,60,100,20);
			this->addAndMakeVisible(lnameLabel);

			Label *lastNameLabel = new Label(T("Lname"), String((*start)->getLastName().c_str()));
			lastNameLabel->setBounds(110,60,100,20);
			this->addAndMakeVisible(lastNameLabel);

			followButton = new TextButton(T("Follow"), T("Follow this user"));
			followButton->setBounds(0, 90, 100, 20);
			this->addAndMakeVisible(followButton);
			followButton->addListener(this);

			backButton = new TextButton(T("Search Again"), T("Try another search"));
			backButton->setBounds(110,90,100,20);
			this->addAndMakeVisible(backButton);
			backButton->addListener(this);
		}
	}
}

void Search::searchUserView()
{
	this->deleteAllChildren();
		
	Label *uLabel = new Label(T("Username Label"), T("Username search"));
	uLabel->setBounds(0,0,100,20);
	this->addAndMakeVisible(uLabel);

	username = new TextEditor(T("Username"), 0U);
	username->setBounds(0,30,190,20);
	this->addAndMakeVisible(username);

	uButton = new TextButton(T("Search"),T("Search Button"));
	uButton->setBounds(200,30,100,20);
	this->addAndMakeVisible(uButton);
	uButton->addListener(this);
}