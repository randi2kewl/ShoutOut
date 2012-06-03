#include "ViewFollowers.h"
#include "Account.h"
#include <list>
#include "SortList.h"
#include "ReverseSortList.h"

//ListBoxModel for creating the ListBox
class FollowerList : public ListBox, public ListBoxModel
{
public:

Dashboard *dash;//dashboard (contains the current accounts info)

//constructor  to create the ListBox
FollowerList(Dashboard *dashboard) : ListBox("Followers", 0)
{
	this->dash = dashboard;
	setModel(this);
}//end constructor

~FollowerList(){  }

//returns the number of accounts in the follower list
int getNumRows()
{
	return dash->getAccount()->getFollowerList().size();
}//end getNumRows

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

	list<Account*> temp = dash->getAccount()->getFollowerList();

	list<Account*>::iterator start, stop;
	start = temp.begin();

	advance(start, rowNumber);

	//draws the text
	g.drawText(String((*start)->getUsername().c_str()), 5, 0, width, height, Justification::centredLeft, true);

}//end method paintListBoxItem

//paints the background behind the list
void paint(Graphics& g)
{
    g.fillAll(Colours::white.withAlpha(0.7f));
}//end method paint

//@TODO: add unfollow button to remove from following list
//this will set the clicking of the account in the list then press button
//to do the removal process.

//void listBoxItemClicked(int row, const MouseEvent& e)
//{
//   //val = row;

//}
};//end class

FollowerList* lbox;//the listBox

//constructor takes in a dashboard and makes listbox visible
ViewFollowers::ViewFollowers(Dashboard* dashboard)
{
	this->dash = dashboard;

	lbox = new FollowerList(dash);
    addAndMakeVisible(lbox);

	//create unfollow button
	/*unfollowButton = new TextButton(T("Unfollow"), T("Unfollow"));
	unfollowButton->setBounds(210, 180, 80, 30);
	addAndMakeVisible(unfollowButton);
	unfollowButton->addListener(this);*/

	//create list a-z button
	a_zButton = new TextButton(T("A-Z"), T("A-Z"));
	a_zButton->setBounds(210, 0, 80, 30);
	addAndMakeVisible(a_zButton);
	a_zButton->addListener(this);

	//create list z-a button
	z_aButton = new TextButton(T("Z-A"), T("Z-A"));
	z_aButton->setBounds(210, 90, 80, 30);
	addAndMakeVisible(z_aButton);
	z_aButton->addListener(this);
}//end constructor

//paint around the box (keeping clear to match the rest)
void ViewFollowers::paint (Graphics& g)
{
   //g.fillAll (Colour (0xffa18a8a));
}//end method paint

void ViewFollowers::resized()
{
	lbox->setBounds(0,0,200,250);
}

//tells what to do with button clicks
void ViewFollowers::buttonClicked (Button* button)
{
	if(button == a_zButton)
	{
		list<Account*> templist = dash->getAccount()->getFollowerList();
		templist.sort(SortList());

		dash->getAccount()->setFollowerList(templist);

		lbox->updateContent();
		lbox->repaint();
	}
	else if(button == z_aButton)
	{
		list<Account*> templist = dash->getAccount()->getFollowerList();
		templist.sort(ReverseSortList());

		dash->getAccount()->setFollowerList(templist);

		lbox->updateContent();
		lbox->repaint();
	}
}//end method buttonClicked