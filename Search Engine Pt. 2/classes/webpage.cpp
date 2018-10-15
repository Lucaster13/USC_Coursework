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
Webpage::Webpage(string& name)
{
	this->name = name;
	pageRank = 0; 
}


/*
destructor:

delete allocated data
*/
Webpage::~Webpage(){}


/*
parse:

separates individual strings and removes
special characters. Also keeps track of outgoing link names.
Passes in the wordbank so it can this page for every encountered
word
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
			outgoingLinkNames.insert(linkName);
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
setDegree & getIn + getOut Degree:

initializes the inDegree and outDegree for the webpage

gets the in and out degree for webpage
*/
void Webpage::setDegree()
{
	//The + 1 is to account for self-loop
	inDegree = incomingLinkNames.size() + 1;
	outDegree = outgoingLinkNames.size() + 1;
}

const double Webpage::getInDegree()
{
	return (double)inDegree;
}

const double Webpage::getOutDegree()
{
	return (double)outDegree;
}


/*
addIncomingAndOutgoing:

adds incoming link names from pages that contain
this one in their outgoingLinkNames set

creates the incomingLinks and outgoingLinks sets that
contain the actual Webpage* 
*/
void Webpage::addIncomingAndOutgoing(const map<string, Webpage*>& webpages)
{
	//set of webpages
	map<string, Webpage*>::const_iterator it = webpages.begin();

	for(it = webpages.begin(); it != webpages.end(); ++it)
	{
		//add matching incoming links to both incoming sets
		if(it->second->outgoingLinkNames.find(name) != it->second->outgoingLinkNames.end())
		{
			incomingLinkNames.insert(it->second->getName());
			incomingLinks.insert(it->second);
		}

		//add matching outgoing links to outgoingLinks set
		if(this->outgoingLinkNames.find(it->second->getName()) != this->outgoingLinkNames.end())
		{
			outgoingLinks.insert(it->second);
		}
	}
}


/*
getName:

returns the name of the page
*/
const string Webpage::getName()
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
and outoging links
*/
size_t Webpage::numOutgoing()
{

	return outgoingLinkNames.size();
}

size_t Webpage::numIncoming()
{

	return incomingLinkNames.size();
} 

/*
getOutgoing & getIncoming:

returns either the set of incoming/outoging names
or the set of incoming/outgoing Webpage*
*/
const set<Webpage*> Webpage::getOutgoingLinks()
{
	return outgoingLinks;
}

const set<Webpage*> Webpage::getIncomingLinks()
{
	return incomingLinks;
}

const set<std::string> Webpage::getOutgoingLinkNames()
{
	return outgoingLinkNames;
}

const set<std::string> Webpage::getIncomingLinkNames()
{
	return incomingLinkNames;
}


/*
printOutgoing & printIncoming:

iterates through sets of incomingLinkNames and 
outgoingLinkNames and prints them to outfile
*/
void Webpage::printOutgoing(ofstream& ofile)
{
	
	set<string>::iterator it = outgoingLinkNames.begin();

	for(it = outgoingLinkNames.begin(); it != outgoingLinkNames.end(); ++it)
	{
		ofile << *it << endl;
	}
	ofile << endl;
	
}

void Webpage::printIncoming(ofstream& ofile)
{
	
	set<string>::iterator it = incomingLinkNames.begin();

	for(it = incomingLinkNames.begin(); it != incomingLinkNames.end(); ++it)
	{
		ofile << *it << endl;
	}

	ofile << endl;

}


/*
get and set pagerank:
*/
double Webpage::getPageRank()
{
	return pageRank;
}

void Webpage::setPageRank(const double& rank)
{
	pageRank = rank;
}