#include <set>
#include <string>
#include <queue>
#include <fstream>
#include <map>

class Webpage {

	public:
		Webpage(std::string& name);
		~Webpage();
		void parse(std::map<std::string, std::set<Webpage*> >& WordBank);
		const std::string getName();
		void printPage(std::ofstream& ofile);
		size_t numOutgoing();
		size_t numIncoming();
		void printOutgoing(std::ofstream& ofile);
		void printIncoming(std::ofstream& ofile);
		void addIncomingAndOutgoing(const std::map<std::string, Webpage*>& webpages);
		const std::set<Webpage*> getOutgoingLinks();
		const std::set<Webpage*> getIncomingLinks();
		const std::set<std::string> getOutgoingLinkNames();
		const std::set<std::string> getIncomingLinkNames();
		const double getInDegree();
		const double getOutDegree();
		void setDegree();
		double getPageRank();
		void setPageRank(const double& rank);
		
		
	protected:
		std::string name;
		std::queue<std::string> printQueue;
		std::set<std::string> pageWords;
		std::set<std::string> incomingLinkNames;
		std::set<std::string> outgoingLinkNames;
		std::set<Webpage*> incomingLinks;
		std::set<Webpage*> outgoingLinks;
		size_t outDegree;
		size_t inDegree;
		bool contains(const std::string& word);
		double pageRank;
};