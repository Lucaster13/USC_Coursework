#include "classes/database.h"
#include <iostream>
#include <stdexcept>
using namespace std;

bool main2(int argc, char* argv[]);

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

bool main2(int argc, char* argv[])
{
	if(argc < 4)
		{
			throw invalid_argument("Must Enter 4 Arguments!\n\n");
		}

		ifstream index(argv[1]);

		if(!index)
		{
			throw invalid_argument("Please enter valid index.txt file name\n\n");
		}

		Database* pages = new Database(index);
		index.close();
		

		ifstream query(argv[2]);

		if(!query)
		{
			throw invalid_argument("Please enter valid query.txt file name\n\n");
		}

		ofstream output(argv[3]);

		if(!output)
		{
			throw invalid_argument("Please enter valid output.txt file name\n\n");
		}

		pages->processQueries(query, output);


		query.close();
		output.close();

		delete pages;
		return 1;
}