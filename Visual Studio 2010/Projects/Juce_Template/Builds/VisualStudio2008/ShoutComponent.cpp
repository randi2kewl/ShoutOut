#include "ShoutComponent.h"
#include "FileIO.h"

ShoutComponent::ShoutComponent(Dashboard *dashboard)
{
	dash = dashboard;

	
	shoutLabel = new Label(T("Shout label"),T("Shout:"));
	shoutLabel->setBounds(0,0,50,20);
	addAndMakeVisible(shoutLabel);

	instructShoutLabel = new Label(T("Private Instruct Label"), T("*Start with \"DS\" followed by @username to private message."));
	instructShoutLabel->setBounds(0,100,300,20);
	addAndMakeVisible(instructShoutLabel);

	shoutText = new TextEditor(T("Enter shout message"));
	shoutText->setBounds(60,0,200,20);
	addAndMakeVisible(shoutText);

	submitShoutButton = new TextButton(T("Shout!"),T("Submit shout"));
	submitShoutButton->setBounds(60,30,100,20);
	addAndMakeVisible(submitShoutButton);
	submitShoutButton->addListener(this);

	cancelShoutButton = new TextButton(T("Cancel"),T("Cancel shout"));
	cancelShoutButton->setBounds(170, 30, 100, 20);
	addAndMakeVisible(cancelShoutButton);
	cancelShoutButton->addListener(this);
}

void ShoutComponent::buttonClicked(Button *button)
{
	if(button==cancelShoutButton)
	{
		//clear the component
		this->deleteAllChildren();

		//create and show the main menu
		MainMenu *mainMenu = new MainMenu(dash);
		mainMenu->setBounds(0,0,300,500);
		addAndMakeVisible(mainMenu);

	}
	else if(button==submitShoutButton)
	{
		Shout *s;

		//create the shout and add to the list for this account
		s->createShout(shoutText->getText(), dash->getAccount());

		//delete all child components
		this->deleteAllChildren();

		//create and show the main menu
		MainMenu *mainMenu = new MainMenu(dash);
		mainMenu->setBounds(0,0,300,500);
		addAndMakeVisible(mainMenu);

	}
}