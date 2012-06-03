#include "ViewFollowing.h"
#include "Account.h"
#include <list>
#include "ReverseSortList.h"

//ListBoxModel for creating the ListBox
class FollowingList : public ListBox, public ListBoxModel
{
private:
	int val;

public:

Dashboard *dash;//dashboard (contains the current accounts info)
list<Account*>::iterator itr;//iterator for the followers list


//constructor  to create the ListBox
FollowingList(Dashboard *dashboard) : ListBox("Following", 0)
{
	this->dash = dashboard;
	setModel(this);
}//end constructor

~FollowingList(){}

//returns the number of accounts in the follower list
int getNumRows()
{
	return dash->getAccount()->getFollowingList().size();
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

	list<Account*> temp = dash->getAccount()->getFollowingList();

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

void listBoxItemClicked(int row, const MouseEvent& e)
{
   val = row;

}

int getVal() { return val; }
};//end class

FollowingList* lbox;//the listBox

//constructor takes in a dashboard and makes listbox visible
ViewFollowing::ViewFollowing(Dashboard* dashboard)
{
	this->dash = dashboard;

	lbox = new FollowingList(dash);
    addAndMakeVisible(lbox);

	//create unfollow button
	unfollowButton = new TextButton(T("Unfollow"), T("Unfollow"));
	unfollowButton->setBounds(210, 180, 80, 30);
	addAndMakeVisible(unfollowButton);
	unfollowButton->addListener(this);

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
void ViewFollowing::paint (Graphics& g)
{
   //g.fillAll (Colour (0xffa18a8a));
}//end method paint

void ViewFollowing::resized()
{
	lbox->setBounds(0,0,200,250);
}

//tells what to do with button clicks
void ViewFollowing::buttonClicked (Button* button)
{
	if(button == unfollowButton)
	{
		if(lbox->getVal() >= 0)
		{
			list<Account*> tempList = dash->getAccount()->getFollowingList();

			list<Account*>::iterator s_start;
			s_start = tempList.begin();
			
			advance(s_start, lbox->getVal());
			string name = (*s_start)->getAccount()->getUsername();

			tempList.erase(s_start);

			dash->getAccount()->setFollowing(tempList);
			dash->updateFollowingCount();

			//list<Account*>::iterator start, stop;
			//start = dash->getAccountList()->begin();
			//stop = dash->getAccountList()->end();

			//for(; start != stop; ++start)
			//{
			//	if((*start)->getUsername() == name)
			//	{

			//		list<Account*>::iterator t_start, t_stop;
			//		t_start = (*start)->getFollowerList().begin();
			//		t_stop = (*start)->getFollowerList().end();

			//		for(; t_start != t_stop; ++t_start)
			//		{
			//			if((*t_start)->getUsername() == dash->getAccount()->getUsername())
			//			{
			//				(*start)->getAccount()->removeFollower(dash->getAccount());
			//			}
			//		}
			//	}
			//}


			lbox->updateContent();
			lbox->repaint();
		}
	}
	else if(button == a_zButton)
	{
		list<Account*> templist = dash->getAccount()->getFollowingList();
		templist.sort(SortList());

		dash->getAccount()->setFollowing(templist);

		lbox->updateContent();
		lbox->repaint();
	}
	else if(button == z_aButton)
	{
		list<Account*> templist = dash->getAccount()->getFollowingList();
		templist.sort(ReverseSortList());

		dash->getAccount()->setFollowing(templist);

		lbox->updateContent();
		lbox->repaint();
	}

}//end method buttonClicked