#include "FileIO.h"

//reads in the account index file and the associated shouts files
void FileIO::readFile()
{
	//create an input file stream
	ifstream acctFile;

	//try block setup to catch file reading errors
	try
	{
		//open the index accounts file
		acctFile.open("Accounts.acct");

		//run loop until end of file
		while(!acctFile.eof())
		{
			//create account
			createAccount(acctFile);
		}//end while loop
	}//end try block
	catch(ifstream::failure error)
	{
		//alert the user that there was a file error
		AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, T("Login Error"),T("Error opening file."));
	}//end catch block

	createFollowing();
}//end method readFile

//return the iterator for acctList
list<Account*>::iterator FileIO::getBegin()
{
	return acctList.begin();
}//end method getBegin

//return iterator for end of acctList
list<Account*>::iterator FileIO::getEnd()
{
	return acctList.end();
}//end getEnd

list<Shout*>::iterator FileIO::getShoutBegin()
{
	return shoutList.begin();
}//end method getShoutBegin

list<Shout*>::iterator FileIO::getShoutEnd()
{
	return shoutList.end();
}//end method getShoutEnd

//reads in the file and parses data
void FileIO::createAccount(ifstream &file)
{
	//strings for holding read-in data
	string username;
	string password;
	string email;
	string fname;
	string lname;

	//get the data separated by '|' and end of line
	getline(file,username,'|');
	getline(file,password,'|');
	getline(file,email,'|');
	getline(file,fname,'|');
	getline(file,lname);

	//create a new account pointer
	Account *acct = new Account(username, password, email, fname, lname);

	createShout(username, *acct);
	
	//put account pointer into the list
	acctList.push_back(acct);
}//end method createAccount

//adds shouts to the shoutlist
void FileIO::createShout(string filename, Account &acct)
{
	//filename structure: "username.acct"
	filename.append(".shout");

	//open file
	ifstream file;
	file.open(filename);

	//strings to get info
	string message;
	string sender;
	string mentions;
	bool isPublic;
	string pub;
	string sdate;
	time_t date;

	while(!file.eof())
	{
		//get strings sep. by |
		getline(file, message, '|');

		if(message != "")
		{
			getline(file, sender, '|');
			getline(file, mentions, '|');
			getline(file, pub, '|');
			getline(file, sdate);

			//check if public or not then set true, or false
			if(pub=="true")
			{
				isPublic = true;
			} else {
				isPublic = false;
			}

			//change the string into a date
			stringstream ss;
			ss << sdate;
			ss >> date;

			//create a shout
			Shout *shout = new Shout(message, sender, mentions, isPublic, date);
	
			//add shout
			acct.addShout(shout);
		}
	}
	file.close();
}//end method createShout

void FileIO::createFollowing()
{
	string filename;
	
	//iterator to check each person's file for followings
	list<Account*>::iterator start, stop;
	start = acctList.begin();
	stop = acctList.end();

	//loop through all accounts
	for(; start != stop; ++start)
	{
		//filename structure is "username.fol"
		filename = (*start)->getUsername();
		filename.append(".fol");

		//open the file
		ifstream file;
		file.open(filename);

		string uname;//holds the string read in
		
		while(!file.eof())
		{
			//usernames are line by line
			getline(file, uname);

			if(uname != "")
			{
				//iterator for looking for following account
				list<Account*>::iterator f_start, f_stop;
				f_start = acctList.begin();
				f_stop = acctList.end();

				//cycle through and find the account to add
				for(; f_start != f_stop; ++f_start)
				{
					//if the username matches then add to the following list
					if((*f_start)->getUsername() == uname)
					{
						(*start)->addFollowing(*f_start);
						(*f_start)->addFollower(*start);
					}
				}//end for loop of account list for inner
			}
		}
		//close the file
		file.close();

	}//end for loop of account list for outer
}//end method createFollowing

//prints out the entire main index file with all accounts and info
void FileIO::outputAccountIndex()
{
	//open the index account file
	ofstream file;
	file.open("Accounts.acct");

	//iterate through the master account list
	list<Account*>::iterator start, stop;
	start = acctList.begin();
	stop = acctList.end();
	
	//loop through and print info for each account
	for(; start != stop; ++start)
	{
		if((*start)->getUsername() != "")
		{
			file << (*start)->getUsername() << "|";
			file << (*start)->getPassword() << "|";
			file << (*start)->getEmail() << "|";
			file << (*start)->getFirstName() << "|";
			file << (*start)->getLastName() << endl;
		}
	}//end for loop

	//close the index file
	file.close();
}//end method outputAccountIndex

//prints to file all accounts following list
void FileIO::outputFollowingList()
{
	string filename;

	//iterate through master list
	list<Account*>::iterator start, stop;
	start = acctList.begin();
	stop = acctList.end();

	//loop through ALL accounts to print following
	for(; start != stop; ++start)
	{
		//filename structure: "username.fol"
		filename = (*start)->getUsername();
		filename.append(".fol");

		//create the output file
		ofstream file;
		file.open(filename);

		list<Account*> tempList = (*start)->getFollowingList();

		//create iterators for the inner loop of following list
		list<Account*>::iterator f_start, f_stop;
		f_start = tempList.begin();
		f_stop = tempList.end();

		//print off all follower's usernames
		for(; f_start != f_stop; ++f_start)
		{
			file << (*f_start)->getUsername() << endl;
		}//end inner loop to get usernames
	}//end outer loop through all accounts
}//end method outputFollowingList

void FileIO::outputFiles()
{
	//run all output functions
	outputAccountIndex();
	outputFollowingList();
	outputShoutList();
}

void FileIO::outputShoutList()
{
	string filename;//holds the filename for output
	
	//iterate through master list
	list<Account*>::iterator a_start, a_stop;
	a_start = acctList.begin();
	a_stop = acctList.end();

	for(; a_start != a_stop; ++a_start)
	{
		if((*a_start)->getUsername() != "")
		{
			//create the filename
			filename = (*a_start)->getUsername().append(".shout");

			//open or create the file
			ofstream file;
			file.open(filename);

			//set temp list
			list<Shout*> tempList = (*a_start)->getShoutList();

			//create and init iterators
			list<Shout*>::iterator s_start, s_stop;
			s_start = tempList.begin();
			s_stop = tempList.end();

			//2nd loop to go through each shout
			for(; s_start != s_stop; ++s_start)
			{
				//continue only if nothing funky happened
				if((*s_start)->getMessage() != "")
				{
					file << (*s_start)->getMessage() << "|";
					file << (*s_start)->getSender() << "|";

					//see if there is a mention
					if((*s_start)->getmMention() != "")
					{
						file << (*s_start)->getmMention() << "|";
					}
					else
					{
						file << " " << "|";
					}
					
					//print if it is a public or not
					if((*s_start)->getIsPublic() == true)
					{
						file << "true" << "|";
					}
					else
					{
						file << "false" << "|";
					}

					//set date to string
					std::stringstream ss;
					ss << (*s_start)->getDate();
					std::string date = ss.str();
			
					//print date in seconds
					file << date << endl;
				}//end if
			}//end for

			//close the file
			file.close();
		}//end if
	}//end for
}//end method outputShoutList