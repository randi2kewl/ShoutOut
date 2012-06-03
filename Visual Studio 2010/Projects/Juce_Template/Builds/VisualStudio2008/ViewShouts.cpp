#include "ViewShouts.h"
#include "Account.h"
#include <list>
#include "SortShouts.h"

//ListBoxModel for creating the ListBox
class ShoutList : public ListBox, public ListBoxModel
{
public:

Dashboard *dash;//dashboard (contains the current accounts info)
list<Shout*> allShouts;//list that holds all available shouts viewable to user
int val;//number of rows

//constructor  to create the ListBox
ShoutList(Dashboard *dashboard) : ListBox("Shouts", 0)
{
	this->dash = dashboard;

	//create master list of accounts for shouts to be printed
	list<Account*>::iterator start, stop;
	start = dash->getAccountList()->begin();
	stop = dash->getAccountList()->end();

	//iterate through account list
	for(; start != stop; ++start)
	{
		//create temp shout list for the account
		list<Shout*> temp = (*start)->getShoutList();

		//create and init the iterators
		list<Shout*>::iterator s_start, s_stop;
		s_start = temp.begin();
		s_stop = temp.end();

		//check each shout to see if its allowed to be seen by user
		for(; s_start != s_stop; ++s_start)
		{
			//print if it is private and user is mentioned
			if((*s_start)->getmMention() == dash->getAccount()->getUsername() && (*s_start)->getIsPublic() == false)
			{
				allShouts.push_back((*s_start));
			}
			//print if private and user was sender
			else if((*s_start)->getSender() == dash->getAccount()->getUsername() && (*s_start)->getIsPublic() == false)
			{
				allShouts.push_back((*s_start));
			}
			//print if private
			else if((*s_start)->getIsPublic() == true)
			{
				allShouts.push_back((*s_start));
			}//end else if
		}//end for
	}//end for

	allShouts.sort(SortShouts());

	//set the model for the list box
	setModel(this);

}//end constructor

~ShoutList(){  }

//returns the number of accounts in the follower list
int getNumRows()
{
	return allShouts.size();
}//end getNumRows

list<Shout*> getAllShouts() { return allShouts; }

//prints the list onto the listbox
void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
	//highlight if selected
	if(rowIsSelected) 
	{
		g.fillAll(Colours::lightblue);
	}//end if

	//set font type and color
	g.setColour(Colours::black);
	g.setFont(height * 0.7f);

	//list<Shout*> temp = dash->getAccount()->getShoutList();

	list<Shout*>::iterator start, stop;
	start = allShouts.begin();

	advance(start, rowNumber);

	if((*start)->getSender() != "")
	{
		if(!(*start)->getIsPublic() && (*start)->getmMention() == dash->getAccount()->getUsername())
		{
			//draws the text
			g.setColour(Colours::red);
			g.drawText(String((*start)->getSender().append(" - ").append((*start)->getMessage()).c_str()), 5, 0, width, height, Justification::centredLeft, true);
		}
		else if(!(*start)->getIsPublic() && (*start)->getSender() == dash->getAccount()->getUsername())
		{
			//draws the text
			g.setColour(Colours::blue);
			g.drawText(String((*start)->getmMention().append(" - ").append((*start)->getMessage()).c_str()), 5, 0, width, height, Justification::centredLeft, true);
		}
		else //if((*start)->getIsPublic())
		{
			//draws the text
			g.setColour(Colours::black);
			g.drawText(String((*start)->getSender().append(" - ").append((*start)->getMessage()).c_str()), 5, 0, width, height, Justification::centredLeft, true);
		}
	}
}//end method paintListBoxItem

//paints the background behind the list
void paint(Graphics& g)
{
    g.fillAll(Colours::white.withAlpha(0.7f));
}//end method paint

//when item is clicked
void listBoxItemClicked(int row, const MouseEvent& e)
{
	val = row;
}

int getVal()
{
	return val;
}

void listBoxItemDoubleClicked(int row, const MouseEvent& e)
{
	//prints all the data for the shout clicked on
	list<Shout*>::iterator itr;
	itr = allShouts.begin();

	advance(itr, row);

	time_t temp = (*itr)->getDate();
	struct tm * timeinfo;
	char date[80];
	timeinfo = localtime( &temp );

	strftime(date, 80, "%c"  , timeinfo);

	std::string message = (*itr)->getSender() + "\n";
	message += date;
	message += "\n\n\"";

	if(!(*itr)->getIsPublic())
		message += "@" + (*itr)->getmMention() + " ";

	message += (*itr)->getMessage() + "\"";

	String shout(message.c_str());

	//popup box with the details for the shout
	AlertWindow::showMessageBoxAsync(AlertWindow::NoIcon, T("Shout:"),shout);
}

};//end class

ShoutList* lbox;//the listBox

//constructor takes in a dashboard and makes listbox visible
ViewShouts::ViewShouts(Dashboard* dashboard)
{
	this->dash = dashboard;

	lbox = new ShoutList(dash);
    addAndMakeVisible(lbox);

	reshoutButton = new TextButton(T("Reshout"), T("Reshout"));
	reshoutButton->setBounds(210, 0, 80, 30);
	addAndMakeVisible(reshoutButton);
	reshoutButton->addListener(this);
	
}//end constructor

//paint around the box (keeping clear to match the rest)
void ViewShouts::paint (Graphics& g)
{
   //g.fillAll (Colour (0xffa18a8a));
}//end method paint

void ViewShouts::resized()
{
	lbox->setBounds(0,0,200,250);
}

//tells what to do with button clicks
void ViewShouts::buttonClicked (Button* button)
{
	if(button == reshoutButton)
	{
		int val = lbox->getVal();
		list<Shout*> tempList = lbox->getAllShouts();

		list<Shout*>::iterator itr;
		itr = tempList.begin();

		advance(itr, val);

		string message = (*itr)->getMessage();
		string sender = dash->getAccount()->getUsername();
		string mention = (*itr)->getmMention();
		bool isPublic = (*itr)->getIsPublic();
		time_t date = (*itr)->getDate();

		Shout *s = new Shout(message, sender, mention, isPublic, date);

		dash->getAccount()->addShout(s);
	}//end if

}//end method buttonClicked