#include "classes/database.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
using namespace std;

bool main2(int argc, char* argv[]);
void parseConfig(ifstream& config, string& indexName, string& queryName, string& outputName,
				double& restartProb, int& stepNum);

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

parses config.txt, creates database and processes
queries on database
*/
bool main2(int argc, char* argv[])
{
	string indexName, queryName, outputName;
	double restartProb = -1;
	int stepNum = -1;

	//default: no config.txt specified
	if(argc == 1)
	{
		ifstream config("config.txt");
		if(!config)
		{
			throw invalid_argument("Please create a config.txt file!\n\n");
		}

		parseConfig(config, indexName, queryName, outputName, restartProb, stepNum);

		//if any variables did not receive values
		if(indexName == "" || queryName == "" || outputName == "" || 
			restartProb == -1 || stepNum == -1)
		{
			string errorMessage = "config.txt MUST contain:\n\nINDEX_FILE = filename.txt\n";
			errorMessage += "QUERY_FILE = filename.txt\nOUTPUT_FILE = filename.txt\n";
			errorMessage += "RESTART_PROBABILITY = value\nSTEP_NUMBER = value\n\n";
			throw invalid_argument(errorMessage);
		}
	}
	//config.txt specified
	else if(argc == 2)
	{
		ifstream config(argv[1]);
		if(!config)
		{
			throw invalid_argument("Please enter a valid configuration file!\n\n");
		}

		parseConfig(config, indexName, queryName, outputName, restartProb, stepNum);

		//if any variables did not receive values
		if(indexName == "" || queryName == "" || outputName == "" || 
			restartProb == -1 || stepNum == -1)
		{
			string errorMessage = "config.txt MUST contain:\n\nINDEX_FILE = filename.txt\n";
			errorMessage += "QUERY_FILE = filename.txt\nOUTPUT_FILE = filename.txt\n";
			errorMessage += "RESTART_PROBABILITY = value\nSTEP_NUMBER = value\n\n";
			throw invalid_argument(errorMessage);
		}
	}
	else
	{
		throw invalid_argument("Too many arguments, only use ./search or ./search config.txt\n\n");
	}

	//establish database
	ifstream index(indexName);
	Database* webpageDatabase = new Database(index);
	index.close();


	ifstream queries(queryName);
	ofstream output(outputName);
	webpageDatabase->processQueries(queries, output, restartProb, stepNum);

	queries.close();
	output.close();

	delete webpageDatabase;
	return 1;
}


/*
parseConfig:

parses the configuration file and establishes 
index name and outfile name
*/
void parseConfig(ifstream& config, string& indexName, string& queryName, string& outputName,
				double& restartProb, int& stepNum)
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
				string varName, variable;
				newWords >> varName >> variable;

				if(varName == "INDEX_FILE")
				{
					indexName = variable;
				}
				else if(varName == "QUERY_FILE")
				{
					queryName = variable;
				}
				else if(varName == "OUTPUT_FILE")
				{
					outputName = variable;
				}
				else if(varName == "RESTART_PROBABILITY")
				{
					restartProb = stod(variable);
				}
				else if(varName == "STEP_NUMBER")
				{
					stepNum = stoi(variable);
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
				else if(word1 == "QUERY_FILE")
				{
					queryName = word2;
				}
				else if(word1 == "OUTPUT_FILE")
				{
					outputName = word2;
				}
				else if(word1 == "RESTART_PROBABILITY")
				{
					restartProb = stod(word2);
				}
				else if(word1 == "STEP_NUMBER")
				{
					stepNum = stoi(word2);
				}
				break;
			}

			//if it gets here it is in the form "varname = filename"
			if(word1 == "INDEX_FILE")
			{
				indexName = word3;
			}
			else if(word1 == "QUERY_FILE")
			{
				queryName = word3;
			}
			else if(word1 == "OUTPUT_FILE")
			{
				outputName = word3;
			}
			else if(word1 == "RESTART_PROBABILITY")
			{
				restartProb = stod(word3);
			}
			else if(word1 == "STEP_NUMBER")
			{
				stepNum = stoi(word3);
			}
			break;
		}
	}
}