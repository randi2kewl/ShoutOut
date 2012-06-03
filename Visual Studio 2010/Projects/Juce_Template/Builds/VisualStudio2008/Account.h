/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: Account.h
*	Class Description:
*		The account class is used to create, obtain, and maintain
*	user accounts. It stores the account's username, password, first name,
*	last name, email. The account's lists of shouts, followers, and 
*	individuals that they follow are also kept in this class.
*
*/


#ifndef __ACCOUNT_H
#define __ACCOUNT_H

#include <string>
#include "../../Source/MainWindow.h"
#include <algorithm>
#include <list>
#include "Shout.h"
#include <cctype>


using namespace std;

//forward declaration for the shout class
class Shout;

class Account 
{

public:

	//default constructor
	Account();

	//5-arg complete constructor (use this to create account)
	Account(string, string, string, string, string);

	//destructor
	~Account();

	//accessors
	string getUsername() const { return username; }
	string getPassword() const { return password;} 
	string getEmail() const { return email;}
	string getFirstName() const { return firstName; }
	string getLastName() const { return lastName; }

	//for maintaining the labels on dashboard
	int getFollowingCount() { return followingList.size(); }
	int getFollowerCount() { return followerList.size(); }
	
	//returns the actual lists
	list<Account*> getFollowingList() { return followingList; }
	list<Account*> getFollowerList() { return followerList; }
	list<Shout*> getShoutList() { return shoutList; }

	//mutators
	void setFollowing(list<Account*> a) { followingList = a; }
	void setFollowerList(list<Account*> a) { followerList = a; }

	//add accounts to the lists
	void addFollowing(Account *);
	void addFollower(Account *);
	void addShout(Shout *);

	//remove accounts from the list
	void removeFollowing (list<Account*>::iterator itr) { followingList.erase(itr); }
	
	//returns true if there is an account matching that username
	bool findUsernameMatch(Account*,string);

	//get this account (typically used with dashboard)
	Account* getAccount() { return this; }

	//this just returns the tempShout which is used for temporary purposes only
	Shout * getTempShout() { return tempShout; }
	
private:

	//private member vars

	//these are the account specific data that needs to be maintained
	string username;
	string password;
	string email;
	string lastName;
	string firstName;

	//temporary shout pointer
	Shout *tempShout;

	//lists for holding followers, following, and shouts
	list<Shout*> shoutList;
	list<Account*> followingList;
	list<Account*> followerList;
};

#endif