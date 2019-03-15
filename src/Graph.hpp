#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <limits>
#include <vector>

using namespace std;



class Node {
    public:
        int id;               //facebook id number
        vector<Node*> adj;    //the adjacency list
        
        bool visited;         //if the current node is visited
        int distance;
        int core;
        int prev;             //used for traversal

        double socialWeight;

        Node (int IDnumber):id(IDnumber), visited(0), distance(numeric_limits<int>::max()), 
        core(numeric_limits<int>::max()), prev(-1), socialWeight(0) {}
};
struct Comp {
    bool operator()(Node* A, Node* B) {
        return A->core > B->core;
    }
};
class Graph {
    public:
        
        Graph(void);

        ~Graph(void);

        /*create the graph as a field with nodes*/
        unordered_map <int, Node*> facebookID;

        bool loadFromFile(const char* in_filename);

        bool pathfinder(Node* from, Node* to);
   
        void socialWeightCalculate();

        void socialgathering(vector<string>& invitees, const int& k);

};



#endif  // GRAPH_HPP
