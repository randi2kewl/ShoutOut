#include "Account.h"

//default constructor
Account::Account()
{

}//end of default constructor

//constructor creates new account
Account::Account(string uname, string pword, string mail, string fName, string lName)
{
	username = uname;
	password = pword;
	email = mail;
	firstName = fName;
	lastName = lName;
}//end constructor

//add a shout to the shout list
void Account::addShout(Shout *shout)
{
	shoutList.push_back(shout);
}//end method addShout

//add following to the list
void Account::addFollowing(Account *fing)
{
	followingList.push_back(fing);
}//end method addFollowing

//add follower to the list
void Account::addFollower(Account *fer)
{
	followerList.push_back(fer);
}//end method addFollower

//find match for username (for convenience in searches later)
bool Account::findUsernameMatch(Account *acct,string value)
{
	//create a temporary list
	list<Account*> aList = acct->getFollowingList();

	//create and init iterators for the list
	list<Account*>::iterator start, stop;
	start = aList.begin();
	stop = aList.end();

	//go through list
	for(; start != stop; ++start)
	{
		//if the username matches then return true
		if((*start)->getUsername() == value)
		{
			return true;
		}//end if
	}//end for

	//nothing was found so return false
	return false;
}//end method findUsernameMatch