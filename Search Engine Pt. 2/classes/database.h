#include "../setutility.h"
#include <vector>

class Database {

	public:
		Database(std::ifstream& index);
		~Database();
		void processQueries(std::ifstream& queries, std::ofstream& ofile, 
			double& restartProb, int& stepNum);

	protected:
		std::map<std::string, Webpage*> webpages;
		std::set<std::string> queryStrings;
		std::map<std::string, std::set<Webpage*> > WordBank;
		
		void PRINT(const std::string& pageName, std::ofstream& ofile);
		void INCOMING(const std::string& pageName, std::ofstream& ofile);
		void OUTGOING(const std::string& pageName, std::ofstream& ofile);
		void AND(std::ofstream& ofile, double& restartProb, int& stepNum);
		void OR(std::ofstream& ofile, double& restartProb, int& stepNum);
		std::set<Webpage*> candidateExpand(std::set<Webpage*>& currentPages);
		std::vector<Webpage*> PageRank(std::set<Webpage*>& candidates, 
			double& restartProb, int& stepNum);
		void resetPageRank(std::vector<Webpage*>& rankedPages);
};
