#include "database.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>

using namespace std;

/*
Constructor:
allocates memory for each webpage
parses all webpages
adds all incoming links to each webpage
*/
Database::Database(ifstream& index)
{
	//create all webpages
	while(!index.fail())
	{
		string linkName;
		index >> linkName;
		if(!index.fail())
		{
			Webpage* newPage = new Webpage(linkName);
			webpages[linkName] = newPage;
		}
	}

	//parse all webpages
	map<string, Webpage*>::iterator it;
	for(it = webpages.begin(); it != webpages.end(); ++it)
	{
		it->second->parse(WordBank);
	}

	//add the incoming links to webpages
	for(it = webpages.begin(); it != webpages.end(); ++it)
	{
		it->second->addIncoming(webpages);
	}
}


/*
Destructor:
Deallocates all memory for webpages
*/
Database::~Database()
{
	map<string, Webpage*>::iterator it = webpages.begin();
	while(it != webpages.end())
	{
		Webpage* temp = it->second;
		++it;
		delete temp;
	}
}


/*
processQueries:
Takes the queries from query.txt
processes the commands and then
performs the necessary functions 
to carry out the query
*/
void Database::processQueries(ifstream& queries, ofstream& ofile)
{
	while(!queries.fail())
	{
		string queryLine;
		getline(queries, queryLine);
		stringstream ss(queryLine);
		vector<string> queryVec;
		string query;
		ss >> query;

		if(ss.fail())break;

		if(query == "PRINT" || query == "INCOMING" || query == "OUTGOING")
		{
			string fileName;
			ss >> fileName;
			if(ss.fail())
			{
				queryVec.push_back(query);
			}
			else
			{
				queryVec.push_back(query);
				queryVec.push_back(fileName);
			}
				
		}
		else if(query == "AND" || query == "OR")
		{
			queryVec.push_back(query);

			while(!ss.fail())
			{
				string word;
				ss >> word;

				if(ss.fail())break;

				for(size_t i = 0; i < word.size(); i++)
				{
					if(!isdigit(word[i]))
					{
						word[i] = tolower(word[i]);
					}
				}

				queryVec.push_back(word);
			}
			
		}
		else if(query == "")
		{
			ofile << "Invalid query\n" << endl;
			continue;
		}
		else
		{
			for(size_t i = 0; i < query.size(); i++)
			{
				if(!isdigit(query[i]))
				{
					query[i] = tolower(query[i]);
				}
			}
			queryVec.push_back(query);

			while(!ss.fail())
			{
				string word;
				ss >> word;
				if(ss.fail())break;

				queryVec.push_back(word);
			}
		}



		if(queryVec.size() == 1)
		{
			vector<string>::iterator it = queryVec.begin();
			for(size_t i = 0; i < it->size(); i++)
			{
				if(!isdigit(it->at(i)))
				{
					it->at(i) = tolower(it->at(i));
				}
			}
			queryStrings.insert(*it);
			OR(ofile);
		}
		else if(queryVec.size() > 1)
		{
			vector<string>::iterator it = queryVec.begin();
			queryVec.erase(it);
			for(it = queryVec.begin(); it != queryVec.end(); ++it)
			{
				queryStrings.insert(*it);
			}

			if(query == "AND")
			{
				AND(ofile);
			}
			else if(query == "OR")
			{
				OR(ofile);
			}
			else if(query == "PRINT")
			{
				it = queryVec.begin();
				PRINT(*it, ofile);
			}
			else if(query == "INCOMING")
			{
				it = queryVec.begin();
				INCOMING(*it, ofile);
			}
			else if(query == "OUTGOING")
			{
				it = queryVec.begin();
				OUTGOING(*it, ofile);
			}
			else
			{
				ofile << "Invalid query\n" << endl;
			}

			
		}
		queryStrings.clear();
	}
	ofile.close();
}




/*
PRINT:
finds the webpage with given name
calls printPage function on that webpage
*/
void Database::PRINT(const string& pageName, ofstream& ofile)
{
	map<string, Webpage*>::iterator it = webpages.find(pageName);

	if(it != webpages.end())
	{
		ofile << pageName << endl;
		it->second->printPage(ofile);
		return;
	}

	//if file does not exist
	ofile << "Invalid query\n" << endl;
}


/*
INCOMING:
finds webpage w given name
calls printIncoming on webpage
*/
void Database::INCOMING(const string& pageName, ofstream& ofile)
{
	map<string, Webpage*>::iterator it = webpages.find(pageName);

	if(it != webpages.end())
	{
		ofile << it->second->numIncoming() << endl;
		it->second->printIncoming(ofile);
		return;
	}

	//if file does not exist
	ofile << "Invalid query\n" << endl;
}


/*
OUTGOING:
finds webpage w given name
calls printOutgoing on webpage
*/
void Database::OUTGOING(const string& pageName, ofstream& ofile)
{
	map<string, Webpage*>::iterator it = webpages.find(pageName);

	if(it != webpages.end())
	{
		ofile << it->second->numOutgoing() << endl;
		it->second->printOutgoing(ofile);
		return;
	}

	//if file does not exist
	ofile << "Invalid query\n" << endl;
}


/*
AND:
calls searchAnd on every webpage for a given query word
then adds those that contain the words to Intersection set
then prints all webpages to ofile
*/
void Database::AND(ofstream& ofile)
{
	set<Webpage*> Intersection;
	set<string>::iterator queryWord;
	
	for(queryWord  = queryStrings.begin(); queryWord != queryStrings.end(); ++queryWord)
	{
		set<Webpage*> pages = WordBank[*queryWord];
		//if no pages contain word
		if(!pages.size())
		{
			Intersection.clear();
			break;
		}

		if(queryWord == queryStrings.begin())
		{
			Intersection = pages;
		}
		else
		{
			Intersection = setIntersection(Intersection, pages);
		}
	}

	ofile << Intersection.size()  << endl;

	set<Webpage*>::iterator it;
	for(it = Intersection.begin(); it != Intersection.end(); ++it)
	{
		ofile << (*it)->getName() << endl;
	}
	ofile << endl;
}


/*
OR:
calls searchOr on every webpage for a given query word
then adds those that contain the words to Union set
then prints all webpages to ofile
*/
void Database::OR(ofstream& ofile)
{
	set<Webpage*> Union;
	set<string>::iterator queryWord;
	
	for(queryWord  = queryStrings.begin(); queryWord != queryStrings.end(); ++queryWord)
	{
		set<Webpage*> pages = WordBank[*queryWord];
		Union = setUnion(Union, pages);
	}

	ofile << Union.size()  << endl;

	set<Webpage*>::iterator it; 
	for(it = Union.begin(); it != Union.end(); ++it)
	{
		ofile << (*it)->getName() << endl;
	}
	ofile << endl;
}

