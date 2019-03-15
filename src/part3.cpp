#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include "Graph.hpp"
#include <limits>

#define SIZE 4

using namespace std;

void usage(char* program_name) {
    cerr << program_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << program_name
         << " friendship_pairs_file test_pairs_file output_file"
         << endl;
    exit(-1);
}

int main(int argc, char* argv[]) {
  
    if (argc != SIZE) {
        usage(argv[0]);
    }

    char* graph_filename = argv[1];
    char* pairs_filename = argv[2];
    char* output_filename = argv[3];
  
    Graph fID;
    bool checkExist = fID.loadFromFile(graph_filename);
 
    if (!checkExist) {
        cout << "error when building the graph" << endl;
    }

    fID.socialWeightCalculate();
    
    ofstream outfile(output_filename);
    ifstream infile(pairs_filename);
  
    while (infile) {
    
        vector <string> record;
        string s;
        if (!getline(infile, s)) break;
        istringstream ss(s);
    
        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(s);
        }
        
        if (record.size() != 2) {
            cout << "not exactly two ids in the file" << endl;
            continue;
        }
        
        int first_id(stoi(record[0]));
        int second_id(stoi(record[1]));
        double social_one;
        double social_two;

        unordered_map<int, Node*>::const_iterator firstID = fID.facebookID.find(first_id);

        if (firstID == fID.facebookID.end()) {
            cout << "firstID does not exist" << endl;
            social_one = -1;
        }

        else {
            social_one = firstID -> second -> socialWeight;
        }

        unordered_map<int, Node *>::const_iterator secondID = fID.facebookID.find(second_id);
        
        if (secondID == fID.facebookID.end()) {
            cout << "secondID does not exist" << endl;
            social_two = -1;
        }

        else {
            social_two = secondID -> second -> socialWeight;
        }


        if (social_one == -1 && social_two == -1) {
            outfile << endl;
            continue;
        }
       
        if (social_one > social_two) {
            outfile << first_id;
        }

        else if (social_one < social_two) {
            outfile << second_id;
        }

        else if (social_one == social_two) {
            outfile << first_id;
            outfile << " ";
            outfile << second_id;
        }
        outfile << endl;
    }
    return 1;
}  
