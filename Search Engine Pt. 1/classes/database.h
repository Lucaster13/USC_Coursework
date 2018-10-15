#include "setutility.h"


class Database {

	public:
		Database(std::ifstream& index);
		~Database();
		void processQueries(std::ifstream& queries, std::ofstream& ofile);

	protected:
		std::map<std::string, Webpage*> webpages;
		std::set<std::string> queryStrings;
		std::map<std::string, std::set<Webpage*> > WordBank;
		
		void PRINT(const std::string& pageName, std::ofstream& ofile);
		void INCOMING(const std::string& pageName, std::ofstream& ofile);
		void OUTGOING(const std::string& pageName, std::ofstream& ofile);
		void AND(std::ofstream& ofile);
		void OR(std::ofstream& ofile);
};