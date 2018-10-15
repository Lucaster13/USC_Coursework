#include <queue>
#include <string>
#include <stdexcept>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>


using namespace std;

class Hypernode
{
	public:

		Hypernode(const string& val);
		~Hypernode();

		const string getVal() {return this->val;};

		const int getG() {return this->g;};
		void setG(int dist) {this->g = dist;};

		const int getH() {return this->h;};

		const bool isVisited() {return visited;};
		void setVisit() {visited = 1;};

		const int getF() {return this->f;};
		void setF() {this->f = this->g + this->h;};

		Hypernode* getPred() {return this->pred;};
		void setPred(Hypernode* pred) {this->pred = pred;};

	private:

		string val;
		int g;
		int h;
		int f;
		Hypernode* pred;
		bool visited;
};

Hypernode::Hypernode(const string& val)
{
	this->val = val;

	this->g = -1;

	this->h = 0;

	for(size_t i = 0; i < val.length(); i++) 
	{
		if(val[i] == '0') this->h++;
	}

	this->f = this->g + this->h;

	this->pred = NULL;

	this->visited = 0;
}

Hypernode::~Hypernode()
{
}

struct HyperComp
{
	//comparator for Hypernode, compares priorities
	//returns true lhs->getF() has greater f, g or bit value than rhs->getF()
	bool operator()(Hypernode* lhs, Hypernode* rhs)
	{
		//if lhs is goal node
		if(!lhs->getH()) return false;

		//if inserting goal node, put at front of queue
		if(!rhs->getH()) return true;

		if(lhs->getF() > rhs->getF()) 
		{
			return true;
		}
		else if(lhs->getF() < rhs->getF())
		{
			return false;
		}
		else if(lhs->getG() > rhs->getG())
		{
			return true;
		}
		else if(lhs->getG() < rhs->getG())
		{
			return false;
		}
		else
		{
			for(size_t i = 0; i < lhs->getVal().length(); i++)
			{
				if(lhs->getVal().at(i) > rhs->getVal().at(i)) return true;
			}
			return false;
		}
	}
};



void solveHypercube(char* startNode, char* permitted)
{
	ifstream ifile(permitted);

	string error = string(permitted) + " does not exist!";
	if(!ifile) throw (invalid_argument(error));

	//contains all permitted nodes
	map<string, Hypernode*> NodeMap;

	//get numPermitted
	string line1;
	getline(ifile, line1);
	stringstream ss(line1);
	int numPermitted;
	ss >> numPermitted;

	//add permitted nodes to NodeMap
	for(int i = 0; i < numPermitted; i++)
	{
		string nodeVal;
		getline(ifile, nodeVal);

		NodeMap[nodeVal] = new Hypernode(nodeVal);

		//if start node
		if(nodeVal == string(startNode))
		{
			NodeMap[nodeVal]->setG(0);
			NodeMap[nodeVal]->setF();
		}

	}

	priority_queue<Hypernode*, vector<Hypernode*>, HyperComp > AlmightyQueue;
	//push start Node to queue
	AlmightyQueue.push(NodeMap[string(startNode)]);

	int numExpansions = 0;

	//establish goal node
	string goalNodeVal = "";
	map<string, Hypernode*>::iterator it = NodeMap.begin();
	for(size_t i = 0; i < it->second->getVal().length(); i++)
	{
		goalNodeVal += "1";
	}


	vector<string> shortestPath;
	while(!AlmightyQueue.empty())
	{
		Hypernode* curr = AlmightyQueue.top();
		AlmightyQueue.pop();

		if(curr->isVisited()) continue;

		curr->setVisit();

		//goal reached
		if(curr->getVal() == goalNodeVal)
		{
			while(curr)
			{
				shortestPath.push_back(curr->getVal());
				curr = curr->getPred();
			}

			break;
		}

		numExpansions++;

		//generate all possible neighbors
		queue<string> neighbors;
		for(size_t i = 0; i < curr->getVal().length(); i++)
		{
			string neighbor = curr->getVal();
			if(neighbor[i] == '0')
			{
				neighbor[i] = '1';
			}
			else
			{
				neighbor[i] = '0';
			}
			neighbors.push(neighbor);
		}


		//iterate through neighbors
		while(!neighbors.empty())
		{
			string neighborVal = neighbors.front();
			neighbors.pop();

			//node is permitted
			if(NodeMap[neighborVal])
			{
				Hypernode* neighbor = NodeMap[neighborVal];

				if(neighbor->isVisited()) continue;

				//if distance is larger than curr + 1, change node
				if((neighbor->getG() > curr->getG() + 1) || neighbor->getG() == -1)
				{
					neighbor->setG(curr->getG() + 1);
					neighbor->setPred(curr);
					neighbor->setF();
					AlmightyQueue.push(neighbor);
				}
			}
		}

	}

	if(shortestPath.empty())
	{
		cout << "No transformation\nexpansions = " << numExpansions << endl;
	}
	else
	{
		//print queue in reverse
		while(!shortestPath.empty())
		{
			string curr = shortestPath.back();
			shortestPath.pop_back();

			cout << curr << endl;
		}
		cout << "expansions = " << numExpansions  << endl;
	}

	//deallocate all memory
	while(it != NodeMap.end())
	{
		Hypernode* temp = it->second;
		++it;
		delete temp;
	}
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		cout << "Invalid # of arguments: ./hypercube [start node] permitted.txt\n";
	} 

	try
	{
		solveHypercube(argv[1], argv[2]);
	}
	catch(invalid_argument& e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}