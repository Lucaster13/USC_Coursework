#include <iostream>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <set>
#include <string>
using namespace std;

bool main2(int argc, char* argv[]);
void parseConfig(ifstream& config, string& indexName, string& outputName);
void crawl(set<string>& pageNames, string currPage, ofstream& ofile);



int main(int argc, char* argv[])
{
	try
	{
		if(main2(argc, argv))
		{
			return 0;
		}
	}
	catch(invalid_argument& e)
	{
		cout << e.what();
		return 1;
	}
}

/*
main2:

performs most ooperations for the crawler
throws exceptions for invalid inputs and invalid 
variables in config.txt

parses config.txt and calls crawl
*/
bool main2(int argc, char* argv[])
{
	string indexName, outputName;
	//no filename specified
	if(argc == 1)
	{
		ifstream config("config.txt");
		if(!config)
		{
			throw invalid_argument("Please create a config.txt file!\n\n");
		}

		parseConfig(config, indexName, outputName);

		if(indexName == "" || outputName == "")
		{
			string errorMessage = "config.txt MUST contain:\n\nINDEX_FILE = filename.txt\n";
			errorMessage += "OUTPUT_FILE = filename.txt\n\n";
			throw invalid_argument(errorMessage);
		}
	}
	//filename specified
	else if(argc == 2)
	{
		ifstream config(argv[1]);
		if(!config)
		{
			throw invalid_argument("Please enter a valid configuration file!\n\n");
		}

		parseConfig(config, indexName, outputName);

		if(indexName == "" || outputName == "")
		{
			string errorMessage = "config.txt MUST contain:\n\nINDEX_FILE = filename.txt\n";
			errorMessage += "OUTPUT_FILE = filename.txt\n\n";
			throw invalid_argument(errorMessage);
		}
	}
	else
	{
		throw invalid_argument("Too many arguments, only use ./search or ./search config.txt\n\n");
	}


	ifstream seed(indexName.c_str());
	ofstream ofile(outputName.c_str());
	set<string> pageNames;

	while(!seed.fail())
	{
		string seedName;
		seed >> seedName;

		if(seed.fail()) break;

		crawl(pageNames, seedName, ofile);
	}

	seed.close();
	ofile.close();

	return 1;
}

/*
parseConfig:

parses the configuration file and establishes 
index name and outfile name
*/
void parseConfig(ifstream& config, string& indexName, string& outputName)
{
	while(!config.fail())
	{
		string line;
		getline(config, line);
		if(config.fail()) break;

		//remove any comments in file
		if((signed)line.find('#') != -1)
		{
			int pos = (signed)line.find('#');
			int pos2 = (signed)line.find('\0');
			line.erase(pos, pos2);
		}

		stringstream ss(line);

		while(!ss.fail())
		{
			string word1;
			ss >> word1;

			//no words that arent comments
			if(ss.fail()) break;

			string word2;
			ss >> word2;

			//only one word
			if(ss.fail())
			{
				int pos = (signed)word1.find('=');
				word1.erase(pos, 1);
				word1.insert(pos," ");

				stringstream newWords(word1);
				string varName, fileName;
				newWords >> varName >> fileName;

				if(varName == "INDEX_FILE")
				{
					indexName = fileName;
				}
				else if(varName == "OUTPUT_FILE")
				{
					outputName = fileName;
				}
				break;
			}

			string word3;
			ss >> word3;

			//two words
			if(ss.fail())
			{
				if((signed)word1.find('=') != -1)
				{
					int pos = (signed)word1.find('=');
					word1.erase(pos, 1);
				}
				else
				{
					int pos = (signed)word2.find('=');
					word2.erase(pos, 1);
				}

				if(word1 == "INDEX_FILE")
				{
					indexName = word2;
				}
				else if(word1 == "OUTPUT_FILE")
				{
					outputName = word2;
				}
				break;
			}

			//if it gets here it is in the form "varname = filename"
			if(word1 == "INDEX_FILE")
			{
				indexName = word3;
			}
			else if(word1 == "OUTPUT_FILE")
			{
				outputName = word3;
			}
			break;
		}
	}
}

/*
crawl:

searches through a file and whenever it finds a link
it makes a recursive call and starts looking at that page's
links
*/
void crawl(set<string>& pageNames, string currPage, ofstream& ofile)
{
	ifstream ifile(currPage.c_str());

	//check if page exists
	if(!ifile) return;

	//check if page already visited
	if(pageNames.find(currPage) != pageNames.end()) return;

	//if page hasnt been visited add to ofile and set
	ofile << currPage << endl;
	pageNames.insert(currPage);

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
			line.erase(pos1,linkName.size()+2);
			line.insert(pos1, " ");
			crawl(pageNames, linkName, ofile);//recursive call
		}
	}

	//page contains no links
	return;
}