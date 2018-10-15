#include "database.h"
#include <iostream>
#include <sstream>
#include <queue>
#include <cctype>
#include <algorithm>
using namespace std;

/*
pageRank Comparator:

compares the pagerank values of each webpage

comp(p1,p2) = true if p1.pagerank < p2.pagerank
*/
struct PageRankComp
{
	bool operator()(Webpage* p1, Webpage* p2)
	{
		return p1->getPageRank() > p2->getPageRank();
	}
};


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

	//add the incoming links to webpages and initialize in and out degree
	for(it = webpages.begin(); it != webpages.end(); ++it)
	{
		it->second->addIncomingAndOutgoing(webpages);
		it->second->setDegree();
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
void Database::processQueries(ifstream& queries, ofstream& ofile,
	double& restartProb, int& stepNum)
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
			OR(ofile, restartProb, stepNum);
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
				AND(ofile, restartProb, stepNum);
			}
			else if(query == "OR")
			{
				OR(ofile, restartProb, stepNum);
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
void Database::AND(ofstream& ofile, double& restartProb, int& stepNum)
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

	//expanding the Candidate Set
	set<Webpage*> expandedSet = candidateExpand(Intersection);

	//calculate PageRanks and order vector
	vector<Webpage*> rankedPages = PageRank(expandedSet, restartProb, stepNum);

	//output the newly Ranked pages
	ofile << rankedPages.size() << endl;
	for(vector<Webpage*>::iterator it = rankedPages.begin(); it != rankedPages.end(); ++it)
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
void Database::OR(ofstream& ofile, double& restartProb, int& stepNum)
{
	set<Webpage*> Union;
	set<string>::iterator queryWord;
	
	for(queryWord  = queryStrings.begin(); queryWord != queryStrings.end(); ++queryWord)
	{
		set<Webpage*> pages = WordBank[*queryWord];
		Union = setUnion(Union, pages);
	}

	//expanding the Candidate Set
	set<Webpage*> expandedSet = candidateExpand(Union);

	//calculate PageRanks and order vector
	vector<Webpage*> rankedPages = PageRank(expandedSet, restartProb, stepNum);

	//output the newly Ranked pages
	ofile << rankedPages.size() << endl;
	for(vector<Webpage*>::iterator it = rankedPages.begin(); it != rankedPages.end(); ++it)
	{
		ofile << (*it)->getName() << endl;
	}
	ofile << endl;
}


/*
candidateExpand:

expands candidate set by iterating through
all incoming and outgoing links of currentPages
*/
set<Webpage*> Database::candidateExpand(set<Webpage*>& currentPages)
{
	set<Webpage*> expandedPages;

	set<Webpage*>::iterator it;
	for(it = currentPages.begin(); it != currentPages.end(); it++)
	{
		expandedPages.insert(*it);

		set<Webpage*> outgoing = (*it)->getOutgoingLinks();
		set<Webpage*> incoming = (*it)->getIncomingLinks();

		set<Webpage*>::iterator itOut;
		for(itOut = outgoing.begin(); itOut != outgoing.end(); ++itOut)
		{
			expandedPages.insert(*itOut);
		}

		set<Webpage*>::iterator itIn;
		for(itIn = incoming.begin(); itIn != incoming.end(); ++itIn)
		{
			expandedPages.insert(*itIn);
		}
	}
	

	return expandedPages;
}


/*
PageRank:

calculates pagerank for a set of pages
*/
vector<Webpage*> Database::PageRank(set<Webpage*>& candidates, double& restartProb, int& stepNum)
{
	double n = (double)candidates.size(); //num candidates = n
	
	//initialize pagerank values
	for(set<Webpage*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
	{
		(*it)->setPageRank(1/n);//set val to 1/n 

	}

	// the first and last terms of the formula that are constant
	const double term1 = 1 - restartProb;
	const double term3 = restartProb * (1/n);

	for(int i = 0; i < stepNum; i++)
	{
		//keeps track of the previous iteration's incoming link
		map<Webpage*, queue<double> > previousIncomingRanks;
		for(set<Webpage*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
		{
			set<Webpage*> incoming = (*it)->getIncomingLinks();

			for(set<Webpage*>::iterator it2 = incoming.begin(); it2 != incoming.end(); ++it2)
			{
				previousIncomingRanks[*it].push((*it2)->getPageRank());
			}
		}

		//calculate new ranks for pages
		for(set<Webpage*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
		{
			double summation = (*it)->getPageRank() * 1/(*it)->getOutDegree(); //accounts for the self loop

			set<Webpage*> incoming = (*it)->getIncomingLinks();
			queue<double> incomingRanks = previousIncomingRanks[*it];

			//iterate through all incoming links adding the (pagerank*1/outdegree) of each incoming link
			for(set<Webpage*>::iterator itSet = incoming.begin(); itSet != incoming.end(); ++itSet)
			{
				summation += incomingRanks.front() * 1/(*itSet)->getOutDegree(); 
				incomingRanks.pop();
			}

			//PAGERANK FORMULA
			double newPageRank = term1 * summation + term3;

			(*it)->setPageRank(newPageRank);
		}
	}

	//copy pages to vector
	vector<Webpage*> rankedPages;
	for(set<Webpage*>::iterator it = candidates.begin(); it != candidates.end(); ++it)
	{	
		rankedPages.push_back(*it);
	}

	//sort ranked pages
	PageRankComp comp;
	sort(rankedPages.begin(), rankedPages.end(), comp);

	return rankedPages;
}


/*
resetPageRank:

reverts all of the candidates' pageRanks back to 0
when the query is over
*/
void resetPageRank(vector<Webpage*>& rankedPages)
{
	for(vector<Webpage*>::iterator it = rankedPages.begin(); it != rankedPages.end(); ++it)
	{
		(*it)->setPageRank(0);
	}
}


