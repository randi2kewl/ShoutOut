#include "MainMenu.h"


MainMenu::MainMenu(Dashboard *dashboard)
{
	//pass pointer to dashboard (holds account info)
	this->dash = dashboard;

	//add search button
	searchButton = new TextButton(T("Search"), T("Search shouts and users"));
	searchButton->setBounds(0,30,100,20);
	addAndMakeVisible(searchButton);
	searchButton->addListener(this);

	//add follower button
	followerButton = new TextButton(T("View Followers"), T("View all followers"));
	followerButton->setBounds(110,90,100,20);
	addAndMakeVisible(followerButton);
	followerButton->addListener(this);

	//add following button
	followingButton = new TextButton(T("View Following"), T("View all following"));
	followingButton->setBounds(110,60,100,20);
	addAndMakeVisible(followingButton);
	followingButton->addListener(this);

	//add shouts button
	shoutsButton = new TextButton(T("View Shouts"), T("View public and private shouts"));
	shoutsButton->setBounds(110,30,100,20);
	addAndMakeVisible(shoutsButton);
	shoutsButton->addListener(this);

	//add new shout label
	newShoutLabel = new Label(T("Shout:"), T("Send new shout"));
	newShoutLabel->setBounds(0,0,0,0);
	addAndMakeVisible(newShoutLabel);

	//add main menu label
	mainMenuLabel = new Label(T("Main Menu"), T("Main Menu:"));
	mainMenuLabel->setBounds(0, 0, 180, 20);
	addAndMakeVisible(mainMenuLabel);

}//end constructor

//works with button click actions
void MainMenu::buttonClicked(Button *button)
{
	//if search button is clicked
	if(button == searchButton)
	{
		//clear the menu screen
		this->deleteAllChildren();

		//open the search menu component
		Search *searchMenu = new Search(dash);
		searchMenu->setBounds(0,0,600,400);
		addAndMakeVisible(searchMenu);
	}
	//if follower button is clicked
	else if(button == followerButton)
	{
		//clear the screen
		this->deleteAllChildren();

		//add ViewFollowers component
		ViewFollowers *viewFollowers = new ViewFollowers(dash);
		viewFollowers->setBounds(0,0,400,400);
		addAndMakeVisible(viewFollowers);
	}
	//if follwing button is clicked
	else if(button == followingButton)
	{
		//clear the screen
		this->deleteAllChildren();

		//add ViewFollowing component
		ViewFollowing *viewFollowing = new ViewFollowing(dash);
		viewFollowing->setBounds(0,0,400,400);
		addAndMakeVisible(viewFollowing);
	}
	//if shout button is clicked
	else if(button == shoutsButton)
	{
		//clear the component
		this->deleteAllChildren();

		//add ViewShouts component
		ViewShouts *viewShouts = new ViewShouts(dash);
		viewShouts->setBounds(0,0,400,400);
		addAndMakeVisible(viewShouts);
	}
}//end method buttonClicked