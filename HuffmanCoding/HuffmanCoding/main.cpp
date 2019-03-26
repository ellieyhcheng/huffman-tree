#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <map>
using namespace std;

struct Node {
	char value;
	int weight;
	Node* right;
	Node* left;
};

class HuffTree
{
public:
	HuffTree(string input);
	~HuffTree();

private:
	Node* root;
	void freeTree(Node* cur);
	void makeTable(Node* cur, string& code);
	map<char, string> table;
};

HuffTree::HuffTree(string input)
{
	if (input == "") {
		cerr << "No input" << endl;
		exit(2);
	}
	root = nullptr;
	// Get frequency distribution of input
	int histogram[128];
	for (int i = 0; i < 128; i++)
		histogram[i] = 0;
	for (int i = 0; i < input.size(); i++) {
		//if (input[i] == ' ')
		//	histogram[26]++;
		//else if (!islower(input[i])) {
		//	cerr << "Must be all lowercase letters" << endl;
		//	exit(1);
		//}
		//else 
		histogram[input[i]]++;
	}
	// Put forest of single nodes into priority queue less frequent to more frequent
	auto comp = [](const Node* n1, const Node* n2) { return n1->weight > n2->weight; };
	priority_queue<Node*, vector<Node*>, decltype(comp)> q(comp);
	for (int i = 0; i < 128; i++) {
		if (histogram[i] != 0) {
			Node* n = new Node;
			n->right = nullptr;
			n->left = nullptr;
			n->value = i;
			n->weight = histogram[i];

			q.push(n);
		}
	}
	/*if (histogram[26] != 0) {
		Node* n = new Node;
		n->right = nullptr;
		n->left = nullptr;
		n->value = ' ';
		n->weight = histogram[26];

		q.push(n);
	}*/
	
	int numLetters = q.size();
	/*while (!q.empty()) {
		cout << q.top()->value << ": " << q.top()->weight << endl;
		delete q.top();
		q.pop();
	}*/

	// Put nodes together into tree
	while (q.size() > 1) {
		Node* n1 = q.top();
		q.pop();
		Node* n2 = q.top();
		q.pop();

		Node* branch = new Node;
		branch->weight = n1->weight + n2->weight;
		branch->value = n1->weight + n2->weight;
		branch->right = n1;
		branch->left = n2;
		q.push(branch);
	}
	// Set root to the tree
	if (q.size() > 0) {
		root = q.top();
		q.pop();
	}
	// Make table of encoding
	cout << "Table of character to binary: \n";
	string code;
	makeTable(root, code);
	map<char, string>::iterator it = table.begin();
	for (; it != table.end(); it++)
		cout << it->first << ' ' << it->second << endl;

	//0000 11 0100 100 00010 011 11 00011 101 011 01010 101 11 100 01011 101 0000 11 0100 00111 00101 00100 00110 011 100 11
	cout <<endl << "Encoded message: ";
	for (int i = 0; i < input.size(); i++)
		cout << table[input[i]];

}

HuffTree::~HuffTree()
{
	freeTree(root);
}

void HuffTree::freeTree(Node* cur) {
	if (cur == nullptr)
		return;
	freeTree(cur->left);
	freeTree(cur->right);
	delete cur;
}

void HuffTree::makeTable(Node* cur, string& code) {
	if (cur == nullptr)
		return;
	if (cur->left == nullptr && cur->right == nullptr) {
		table[cur->value] = code;
		code = "";
		return;
	}
	string left = code + '0';
	string right = code + '1';
	makeTable(cur->left, left);
	makeTable(cur->right, right);
}

int main() {
	string input;
	cout << "Enter your input: " << endl;
	getline(cin, input);
	
	HuffTree tree(input);
	
}