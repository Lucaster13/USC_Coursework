#include "webpage.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;

/*
constructor:
initialize name
allocate data for sets
*/
Webpage::Webpage(string name)
{
	this->name = name; 
}


/*
destructor:
delete allocated data
*/
Webpage::~Webpage(){}


/*
parse:
separates individual strings and removes
special characters. Also keeps track of outgoing links.
Passes in the set of Webpages so it can
add outgoing links.
*/
void Webpage::parse(map<string, set<Webpage*> >& WordBank)
{
	ifstream ifile(name.c_str());

	if(!ifile) return;

	while(!ifile.fail())
	{
		string line;
		getline(ifile, line);

		//strip line of any outoging links
		while((signed)line.find('(') != -1)
		{
			int pos1 = (signed)line.find('(');
			int pos2 = (signed)line.find(')');
			string linkName;
			for(int i = pos1+1; i < pos2; i++)
			{
				linkName += line[i];
			}
			outgoing.insert(linkName);
			line.erase(pos1,linkName.size()+2);
			line.insert(pos1, " ");
		}

		stringstream ss(line);
		if(ifile.fail())
		{
			break;
		}

		while(!ss.fail())
		{
			string word;
			ss >> word;
			if(ss.fail())
			{
				break;
			}


			for(size_t i = 0; i < word.size(); i++)
			{
				if(isalpha(word[i]))
				{
					word[i] = tolower(word[i]);
				}
				else if(isdigit(word[i]))
				{

				}
				else if(word[i] == '[' || word[i] == ']')
				{
					word.erase(i,1);
				}
				else
				{
					//must be special char
					word[i] = ' ';
				}
			}
			//allows for the insertion of words if
			//special chars were deleted
			stringstream newWords(word);
			while(!newWords.fail())
			{
				string word;
				newWords >> word;
				if(ss.fail())break;
				WordBank[word].insert(this);
				pageWords.insert(word);
			}
		}
	}
	ifile.close();
}


/*
contains:
checks if Webpage contains given word
*/
bool Webpage::contains(const string& word)
{
	set<string>::iterator it = pageWords.find(word);
	if(it != pageWords.end()) return true;

	return false;
} 


/*
addIncoming:
adds incoming links to webpages matching 
the links in outgoing set
*/
void Webpage::addIncoming(map<string, Webpage*>& webpages)
{
	//set of webpages
	map<string, Webpage*>::iterator it = webpages.begin();

	for(it = webpages.begin(); it != webpages.end(); ++it)
	{
		if(it->second->outgoing.find(this->getName()) != it->second->outgoing.end())
		{
			incoming.insert(it->second->getName());
		}
	}
}


/*
getName:
returns the name of the page
*/
string Webpage::getName() const
{
	return name;
}


/*
printPage:
Prints the given webpage using a queue
and cycling through the items
*/
void Webpage::printPage(ofstream& ofile)
{
	ifstream ifile(name.c_str());

	if(!ifile) return;



	while(!ifile.fail())
	{
		string line;
		getline(ifile, line);

		if(ifile.fail())
		{
			break;
		}

		//strip line of any outoging links
		while((signed)line.find('(') != -1)
		{
			int pos1 = (signed)line.find('(');
			int pos2 = (signed)line.find(')');
			line.erase(pos1,pos2-pos1+1);
		}

		ofile << line << endl;
	}

	ifile.close();
}


/*
numOutgoing & numIncoming:
returns the number of incoming links
and outgoing links
*/
size_t Webpage::numOutgoing()
{

	return outgoing.size();
}

size_t Webpage::numIncoming()
{

	return incoming.size();
} 


/*
printOutgoing & printIncoming:
iterates through sets of incoming links and 
outgoing links and prints them to outfile
*/
void Webpage::printOutgoing(ofstream& ofile)
{
	
	set<string>::iterator it = outgoing.begin();

	for(it = outgoing.begin(); it != outgoing.end(); ++it)
	{
		ofile << *it << endl;
	}
	ofile << endl;
	
}

void Webpage::printIncoming(ofstream& ofile)
{
	
	set<string>::iterator it = incoming.begin();

	for(it = incoming.begin(); it != incoming.end(); ++it)
	{
		ofile << *it << endl;
	}

	ofile << endl;

}