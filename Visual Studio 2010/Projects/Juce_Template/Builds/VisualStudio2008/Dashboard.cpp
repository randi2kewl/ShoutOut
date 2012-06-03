/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: Dashboard.h
*	Class Description:
*		The dashboard class pretty much contains all the information
*	for the currently logged in account while also serving as
*	the container for the buttons/labels/images.
*
*/

#include "Dashboard.h"
#include "Search.h"
#include "MainMenu.h"
#include "ShoutComponent.h"
#include "LogoComponent.h"

/* Class: Dashboard
 * Method: Dashboard
 * 
 * This constructor creates all the buttons and holds
 * the labels/image.
 *
 * @returns void
 * @params Account*, list<Account*>*
 *
*/
Dashboard::Dashboard(Account *theAcct, list<Account*> *aList)
{
	acct = theAcct; //sets the logged in account to be used
	acctList = aList;//sets the list of all accounts

	//create the welcome message to show on screen with username
	string welcomeMessage = "Welcome, " + acct->getUsername() + "!";

	//AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, T("Login Error"),T("here"));
	welcome = new Label(T("Welcome"), String(welcomeMessage.c_str()));	
	welcome->setBounds(0, 0, 300, 20);
	addAndMakeVisible(welcome);

	//create logout button
	logoutButton = new TextButton(T("Logout"), T("Logout of account"));
	logoutButton->setBounds(390,10,100,20);
	addAndMakeVisible(logoutButton);
	logoutButton->addListener(this);

	//create main menu button
	mainMenuButton = new TextButton(T("Main Menu"), T("Back to main menu"));
	mainMenuButton->setBounds(290, 10, 100, 20);
	addAndMakeVisible(mainMenuButton);
	mainMenuButton->addListener(this);

	//create shout button
	shoutButton = new TextButton(T("New Shout!"), T("Send a new shout"));
	shoutButton->setBounds(190, 10, 100, 20);
	addAndMakeVisible(shoutButton);
	shoutButton->addListener(this);

	//get the follower count and create String
	String ferCount("Followers:");
	ferCount += acct->getFollowerCount();

	//print the follower count on label
	followerCountLabel = new Label(T("Total followers"), ferCount);
	followerCountLabel->setBounds(10,50,180,20);
	addAndMakeVisible(followerCountLabel);

	//create Juce::String with following total count
	String fingCount("Following:");
	fingCount += acct->getFollowingCount();

	//print the following count on the label
	followingCountLabel = new Label(T("Total following"), fingCount);
	followingCountLabel->setBounds(10,70,180,20);
	addAndMakeVisible(followingCountLabel);

	//inner component that holds the main menu (for positioning stability)
	container = new Component();
	container->setBounds(150,100,300,300);
	addAndMakeVisible(container);

	//create and show the main menu on first run
	MainMenu *mainMenu = new MainMenu(this);
	mainMenu->setBounds(0,0,300,300);
	addAndMakeVisible(mainMenu);
	container->addChildComponent(mainMenu);

	//creates the cached image on the left of dashboard
	LogoComponent *logo = new LogoComponent();
	logo->setBounds(-30,90, 100, 270);
	addAndMakeVisible(logo);

}//end constructor

/* Class: Dashboard
 * Method: buttonClicked
 * 
 * Does operations appropriate to the button that
 * was clicked.
 *
 * @returns void
 * @params Button
 *
*/
void Dashboard::buttonClicked(Button *button)
{
	//the logout button was clicked
	if(button == logoutButton)
	{
		//clear the component
		deleteAllChildren();

		//recreate and show the main login screen again
		MainComponent *mainComp = new MainComponent(acctList);
		mainComp->setBounds(0,0,600,400);
		addAndMakeVisible(mainComp);

	} 
	//if the main menu button is clicked
	else if(button == mainMenuButton)
	{
		//clear the component
		container->deleteAllChildren();

		//create and show the main menu
		MainMenu *mainMenu = new MainMenu(this);
		mainMenu->setBounds(0,0,300,500);
		addAndMakeVisible(mainMenu);
		container->addChildComponent(mainMenu);
	}
	//if the shout button is clicked
	else if(button == shoutButton)
	{
		container->deleteAllChildren();
		ShoutComponent *shoutComponent = new ShoutComponent(this);
		shoutComponent->setBounds(0,0,300,500);
		addAndMakeVisible(shoutComponent);
		container->addAndMakeVisible(shoutComponent);
	}
}//end method buttonClicked

/* Class: Dashboard
 * Method: updateFollowingCount
 * 
 * Updates the label on the dashboard with 
 * the new following count.
 *
 * @returns void
 * @params none
 *
*/
void Dashboard::updateFollowingCount()
{
	//get the new follower count and put into String
	String fingCount("Following:");
	fingCount += acct->getFollowingCount();

	//update the labe with the following count
	followingCountLabel->setText(fingCount, false);
}//end method updateFollowingCount