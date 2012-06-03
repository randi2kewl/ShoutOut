/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: Shout.h
*	Class Description:
*		Contains all of the details on a shout
*	mainly the message, sender, mention, date
*	and if it is public or not.
*
*/

#ifndef __SHOUT_H
#define __SHOUT_H

#include <time.h>
#include <list>
#include <string>
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Account.h"

class Account;

using namespace std;

class Shout
{

public:

	//default constructor
	Shout(){}

	//main constructor (use this)
	Shout(string, string, string, bool, time_t);

	//destructor
	~Shout(){}

	//accessors
	string getMessage() { return message; }
	string getSender() { return sender; }
	string getmMention() { return mention; }
	bool getIsPublic() { return isPublic; }
	time_t getDate() { return date; }


	//mutators
	void setIsPublic(bool pub) { isPublic = pub; } 


	//operations
	void createShout(String, Account*);

private:

	//private member vars
	string message;
	string sender;
	string mention;
	bool isPublic;
	time_t date;
};
#endif