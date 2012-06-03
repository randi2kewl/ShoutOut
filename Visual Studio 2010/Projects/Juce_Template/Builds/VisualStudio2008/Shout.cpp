#include "Shout.h"

Shout::Shout(string msg, string send, string ment, bool pub, time_t d)
{
	message = msg;
	sender = send;
	mention = ment;
	isPublic = pub;
	date = d;
}

void Shout::createShout(String keyShout, Account *acct)
{
	std::string shout(keyShout.toUTF8());
	unsigned int temp;

	string message;
	string sender;
	string mention;
	bool isPublic;

	if(shout[0]=='D' && shout[1]=='S')
	{
		if(shout[3]=='@')
		{
			for(unsigned int j = 4; j < shout.length(); j++)
			{
				if(shout[j]==' ')
				{
					temp = j;
					break;
				}
				else
				{
					mention.append(1,shout[j]);
				}
			}
		}
		isPublic = false;
	}
	else
	{
		temp = 0;
		mention = " ";
		isPublic = true;
	}

	for(unsigned int i = temp; i < shout.size(); i++)
	{
		message.append(1,shout[i]);
	}

	sender = acct->getUsername();
	time_t date = time(NULL);

	Shout *s = new Shout(message,sender,mention,isPublic,date);

	acct->addShout(s);
}