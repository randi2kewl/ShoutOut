/*
*	Program Name: ShoutOut
*	Version: 0.0.1 Beta
*	Author: Randy Miller - techrandy@gmail.com
*	
*
*	File name: Registration.h
*
*/

#ifndef __FILEIO_H
#define __FILEIO_H

#include <fstream>
#include "../../Source/MainWindow.h"
#include "Account.h"
#include <list>
#include <algorithm>
#include "MainComponent.h"
#include <exception>
#include "Shout.h"
#include <sstream>

using namespace std;

class Account;

class FileIO
{
public:
	//constructors
	FileIO(){}

	//destructors
	~FileIO(){}

	//operations
	void readFile();
	void createAccount(ifstream &);
	void createShout(string, Account&);
	void createFollowing();

	//return the account list
	list<Account*>* getAccountsList() { return &acctList; }

	//maybe remove? might be unnecessary
	list<Account*>::iterator getBegin();
	list<Account*>::iterator getEnd();
	list<Shout*>::iterator getShoutBegin();
	list<Shout*>::iterator getShoutEnd();

	//functions to print to files
	void outputFiles();
	void outputAccountIndex();
	void outputFollowingList();
	void outputShoutList();

private:

	list<Account*> acctList; //full account list of all users
	list<Shout*> shoutList; //full shout list of all users

};
#endif;