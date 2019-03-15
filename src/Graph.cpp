#include "Graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <queue>

#define SIZE 2
using namespace std;

/*initialize the facebookID field in the graph*/
Graph:: Graph(void)
    :facebookID(0){} 

/*destructor*/
Graph::~Graph(void) {
    for (auto itr : facebookID) {
        delete itr.second;
    }
}
 
/* Read in relationships from an inputfile to create a graph */
bool Graph::loadFromFile(const char* in_filename) {

    ifstream infile(in_filename);

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(s);
        }

        if (record.size() != SIZE) {
            continue;
        }

        
        /*record input value*/
        int first_id = stoi(record[0]);
        int second_id = stoi(record[1]);

        /*search for the first id*/
        unordered_map<int, Node*>::const_iterator checkExist = facebookID.find
                      (first_id);

        /*if not yet exist, create one*/
        if (checkExist == facebookID.end()) {
            Node * user = new Node(first_id);
            pair<int, Node *> userElement(first_id, user);
            facebookID.insert(userElement);
            checkExist = facebookID.find(first_id);
        }

        /*search for the second id*/
        unordered_map<int, Node *>::const_iterator checkExist_friend = facebookID.find
                      (second_id);
    
        /*if not yet exist, create one*/
        if (checkExist_friend == facebookID.end()) {
            Node * userfriend = new Node(second_id);
            pair<int,Node*> friendElement(second_id, userfriend);
            facebookID.insert(friendElement);
            checkExist_friend = facebookID.find(second_id);
        }

        /*establish the connections*/
        checkExist -> second -> adj.push_back (checkExist_friend -> second);
        checkExist_friend -> second -> adj.push_back(checkExist -> second);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }

    infile.close();
    return true;
}

/*recursively find the path from from to to*/
bool Graph::pathfinder(Node* from, Node* to) {
    if(from->id == to->id) {
        cout << "the starting id is the ending id" << endl;
        return 1;
    }
    queue<Node*> toExplore;
    auto fNode = facebookID.find(from->id);
    auto tNode = facebookID.find(to->id);
    if( fNode == facebookID.end()) {
        cout << "the staring id is not here" << endl;
        return 0;
    }
    if( tNode == facebookID.end()) {
        cout << "the ending ID is not here" << endl;
        return 0;
    }
    from->distance = 0;
    toExplore.push(fNode->second);
    while(!toExplore.empty()) {
        Node* curr = toExplore.front();
        curr->visited = true;
        toExplore.pop();
        if(curr->id == to->id) {
            cout << "find the path successfully" << endl;
            return 1;
        }
        for(unsigned int i = 0; i < curr->adj.size(); i++) {
            if(curr->adj[i]->distance == numeric_limits<int>::max() && curr->adj[i]->visited == 
               false ) {
                curr->adj[i]->distance = curr->distance++;
                curr->adj[i]->prev = curr->id;
                toExplore.push(curr->adj[i]);
            }
        }   
    }
    return 0;
}

/*calculate social ablity number for everynode in this graph and record*/
void Graph::socialWeightCalculate () {

    for (auto n : facebookID) {

        double total = 0;

        for (unsigned int i = 0; i < n.second -> adj.size()/2; i++) {
            total = total + 1.0/(double)(n.second -> adj[i] -> adj.size()/2);
        }
        
        n.second -> socialWeight = total;
    }
}

void Graph::socialgathering(vector<string>& invitees, const int& k) {
    priority_queue<Node*, vector<Node*>, Comp> pq;
    for(unsigned int i = 0; i < invitees.size(); i++) {
        unordered_map<int, Node*>::const_iterator checkExist = facebookID.find
                                                                (stoi(invitees[i]));
        Node* tmp = checkExist->second;
        tmp->core = tmp->adj.size();
        pq.push(tmp);
    }

    while(!pq.empty()) {
        if((signed int) pq.top()->core >= k) {
            cout << "Preparing Invitees List" << endl;
            break;
        }
        Node* tmp = pq.top();
        for( unsigned int i = 0; i < tmp->adj.size(); i++) {
            pq.top()->adj[i]->core--; 
        }
        pq.pop();
    }
        return;
}
