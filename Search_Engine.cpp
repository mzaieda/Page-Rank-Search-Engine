#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h> 

using namespace std;

ifstream f;
string str;
int xmax=0;
double pageRanked1 = 1;
double pageRanked2 = 1;

class page{
private:
	int CTR;
	vector<string> *webPages = new vector<string>;
	double pageRank;
	vector<int> *array_of_pageRanks = new vector<int>;
	string pageLink;
	string pageTitle;
	vector<string> *array_of_keys = new vector<string>;
	page* nextPage;

public:
	void setPageID(int CTR);
	int getPageID();
	void setPageLink(string pageLink);
	string getPageLink();
	bool textSearch(string keyWord);
	int webLinkSearch(string webLink);
	void setRank(double pageRank);
	double getRank();
	void setRanking(int id);
	vector<int>* getRanking();
	page* getNextPage() const;
	void setNextPage(page* nextNodePtr);
	void setpageTitle(string pageTitle);
	string getPageTitle();
	void setKeys(string keyWord);
	void setWebPages(string webLink);
	vector<string> *getWebLink();
};

void page::setPageID(int CTR) {this->CTR = CTR;}

int page::getPageID(){return CTR;}

void page::setPageLink(string pageLink){this->pageLink = pageLink;}

string page::getPageLink(){return pageLink;}

void page::setpageTitle(string pageTitle){this->pageTitle = pageTitle;}

string page::getPageTitle(){return pageTitle;}

void page::setKeys(string keyWord){array_of_keys->push_back(keyWord);}

void page::setWebPages(string webLink){webPages->push_back(webLink);}

vector<string> *page::getWebLink(){return webPages;}

bool page::textSearch(string keyWord){
	for (int i = 0; i < array_of_keys->size(); i++)
		if (keyWord == array_of_keys->at(i))
			return true;
	return false;
}

int page::webLinkSearch(string webLink){
	int counter_num = 0;
	for (int i = 0; i < webPages->size(); i++)
		if (webLink == webPages->at(i))
			counter_num++;

	return counter_num;
}

void page::setRank(double pageRank){this->pageRank = pageRank;}

double page::getRank(){return pageRank;}

void page::setRanking(int id){this->array_of_pageRanks->push_back(id);}

vector<int>* page::getRanking(){return array_of_pageRanks;}

void page::setNextPage(page* nextNodePtr){nextPage = nextNodePtr;}

page* page::getNextPage() const{return nextPage;}

//------------------------------------------------------------------------------------------------
 
class Page_Linked_List{
private:
	int pageCount;
public:
	page *pagePtr;
	Page_Linked_List();
	page* getPageAt(int pos) const;
	int getLength() const;
	bool insertPage(int newPosition);
	bool deletePage(int pos);
	void PageRankSort();
	bool ListEmpty() const;
	~Page_Linked_List();
};

Page_Linked_List::Page_Linked_List() : pagePtr(nullptr), pageCount(0){}

Page_Linked_List::~Page_Linked_List(){
	while (!ListEmpty())
		deletePage(0);
}

bool Page_Linked_List::deletePage(int pos){
	bool removed = (pos >= 0) && (pos < pageCount);
	if (removed){
		page* currentNode = nullptr;
		if (pos == 0){
			currentNode = pagePtr;
			pagePtr = pagePtr->getNextPage();
		}
		else{
			page* previousNode = getPageAt(pos - 1);
			currentNode = previousNode->getNextPage();
			previousNode->setNextPage(currentNode->getNextPage());
		}

		currentNode->setNextPage(nullptr);
		delete currentNode;
		currentNode = nullptr;
		pageCount--;
	}

	return removed;
}

page* Page_Linked_List::getPageAt(int pos) const{
	assert((pos >= 0) && (pos < pageCount));
	page* currentNode = pagePtr;
	for (int i = 0; i < pos; i++)
		currentNode = currentNode->getNextPage();
	return currentNode;
}

bool Page_Linked_List::insertPage(int newPosition){
	bool InsertAvailable = (newPosition >= 0) && (newPosition <= pageCount);
	if (InsertAvailable){
		page* newPagePtr = new page();
		if (newPosition == 0){
			pagePtr = newPagePtr;
			newPagePtr->setNextPage(NULL);
		}
		else{
			page* previousNode = getPageAt(newPosition - 1);
			newPagePtr->setNextPage(previousNode->getNextPage());
			previousNode->setNextPage(newPagePtr);
		}
		pageCount++;
	}
	return InsertAvailable;
}

bool Page_Linked_List::ListEmpty() const{return pageCount == 0;}

int Page_Linked_List::getLength() const{return pageCount;}

void Page_Linked_List::PageRankSort(){
	bool swappingDone;
	page* ptr;
	do{
		swappingDone = 0;
		for (int i = 0; i < (getLength() - 1); i++){
			if (getPageAt(i)->getRank() < getPageAt(i + 1)->getRank()){
				if (i == 0){
					ptr = pagePtr;
					pagePtr = ptr->getNextPage();
					ptr->setNextPage(pagePtr->getNextPage());
					pagePtr->setNextPage(ptr);
				}
				else{
					ptr = getPageAt(i - 1)->getNextPage();
					getPageAt(i - 1)->setNextPage(ptr->getNextPage());
					ptr->setNextPage(getPageAt(i)->getNextPage());
					getPageAt(i)->setNextPage(ptr);
				}
				swappingDone = 1;
			}
		}
	}while (swappingDone);
}

//------------------------------------------------------------------------------------------------
 
void readFiles(int pageNum, Page_Linked_List *node){
	for (int i = 0; i < pageNum; i++){
		node->insertPage(i);
		f.open(to_string(i) + "text.txt");
		f >> str;
		node->getPageAt(i)->setPageID(i);
		f >> str;
		node->getPageAt(i)->setPageLink(str);
		f >> str;
		node->getPageAt(i)->setpageTitle(str);
		f.close();
		f.open(to_string(i) + "KeySearch.txt");
		while (!f.eof()){
			f >> str;
			node->getPageAt(i)->setKeys(str);
		}
		f.close();
		f.open(to_string(i) + "webLink.txt");
		while (!f.eof()){
			f >> str;
			node->getPageAt(i)->setWebPages(str);
		}
		f.close();
	}
}

//------------------------------------------------------------------------------------------------
 
void array_of_pageRanks(int pageNum, Page_Linked_List *node) {
	for (int i = 0; i < pageNum; i++)
		for (int j = 0; j < pageNum; j++){
			int x = node->getPageAt(j)->webLinkSearch(node->getPageAt(i)->getPageLink());
			for (int k = 0; k < x; k++)
				node->getPageAt(i)->setRanking(node->getPageAt(j)->getPageID());
		}

	for (int i = 0; i < pageNum; i++)
		node->getPageAt(i)->setRank(1.0 / pageNum);

	double currentRank;
	double *arrayPtr = new double[pageNum];
	for (int z = 0; z < 3; z++){
		for (int i = 0; i < pageNum; i++){
			currentRank = 0.0;
			vector<int>* ptr = new vector<int>;
			for (int a = 0; a < node->getPageAt(i)->getRanking()->size(); a++)
				ptr->push_back(node->getPageAt(i)->getRanking()->at(a));

			int ptrSize = ptr->size();
			for (int n = 0; n < ptrSize; n++){
				currentRank += (node->getPageAt(ptr->back())->getRank() / node->getPageAt(ptr->back())->getWebLink()->size());
				ptr->pop_back();
			}
			arrayPtr[i] = currentRank;
		}
		for (int i = 0; i < pageNum; i++)
			node->getPageAt(i)->setRank(arrayPtr[i]);
	}
}

//------------------------------------------------------------------------------------------------

void searchEngine(int pageNum, Page_Linked_List *node){
	string keyWord;
	bool notfound = 1;
	cout << "Enter your search key: ";
	cin >> keyWord;
	cout << endl;
	int coco = 1;
	for (int i = 0; i < pageNum; i++)
		if (node->getPageAt(i)->textSearch(keyWord)){
			if(i == 0){
				notfound = 0;
				cout << coco << ". " << node->getPageAt(i)->getPageTitle() << endl;
				cout << node->getPageAt(i)->getPageLink() << "/this" << endl << endl;
				coco++;
			}
			notfound = 0;
			cout << coco << ". " << node->getPageAt(i)->getPageTitle() << endl;
			cout << node->getPageAt(i)->getPageLink() << endl << endl;
			coco++;
		}

	if (notfound)
		cout << "Your search key must be in one of the search documents of this project's repository. Try to enter: boys. " << endl<< endl;
}



//------------------------------------------------------------------------------------------------

int main(){
	Page_Linked_List* node = new Page_Linked_List();
	const int pageNum = 11;   //number of pages stored in the directory 
	readFiles(pageNum, node);
	array_of_pageRanks(pageNum, node);
	node->PageRankSort();
	bool cont = 0;
	int x;
	int choiceUser;
	int numberChoice;
	int pickNum;
	cout << "Welcome!" << endl;
	cout << "What would you like to do?" << endl;
	cout << "1. New Search" << endl;
	cout << "2. Exit" << endl;
	cout << "Type in your choice: ";

	cin >> choiceUser;
	if (choiceUser == 2)
		exit(0);
	else if(choiceUser != 1)
		exit(0);

	do {
		searchEngine:searchEngine(pageNum, node);
		cout << "Would you like to:" << endl;
		cout << "1. Choose a Webpage to open" << endl;
		cout << "2. New Search" << endl;
		cout << "3. Exit" << endl;
		cout << "Type in your choice: ";
		enter:cin >> x;
		if (x == 2)
			cont = 1;
		else if (x == 3)
			exit(0);
		else if (x == 1){
			cont = 0;
			goto chooseWebpage;
		}
		else{
			cout << "Please enter a valid choice:";
			goto enter;
		}
	}while (cont);
	
	chooseWebpage:cout << "Which webpage you want to view: ";
	cin >> numberChoice;
	if(numberChoice == 1)
		goto choiceDone;
	
	else if (numberChoice == 2)
		goto choiceDone;
	
	else
		goto choiceDone;

	choiceDone:cout << "You're now viewing webpage " << numberChoice << "." << endl;
	cout << "Number of CTR is updated in the info text files corresponding to the searchEngine choice." << endl;
	cout << "Would you like to: " << endl;
	cout << "1. New Search" << endl;
	cout << "2. Exit" << endl;
	cin >> pickNum;
	if(pickNum == 1)
		goto searchEngine;
	else
		exit(0);
	
	return 0;
}