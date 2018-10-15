#include <set>
#include <string>
#include <queue>
#include <fstream>
#include <map>

class Webpage {

	public:
		Webpage(std::string name);
		~Webpage();
		void parse(std::map<std::string, std::set<Webpage*> >& WordBank);
		std::string getName() const;
		void printPage(std::ofstream& ofile);
		size_t numOutgoing();
		size_t numIncoming();
		void printOutgoing(std::ofstream& ofile);
		void printIncoming(std::ofstream& ofile);
		void addIncoming(std::map<std::string, Webpage*>& webpages);
		
		
	protected:
		std::string name;
		std::queue<std::string> printQueue;
		std::set<std::string> pageWords;
		std::set<std::string> incoming;
		std::set<std::string> outgoing;
		bool contains(const std::string& word);
		

		
};