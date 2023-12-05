//
// Created by Aidan Parsons on 12/3/23.
//

#ifndef COP3530MENINBLACK_MY_KD_TREE_H
#define COP3530MENINBLACK_MY_KD_TREE_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>


using namespace std;

// Class for a data structure that contains a 2-dimensional kd tree used to quickly store and perform a range search through
// large amounts of data containing geographic coordinates.
class MyKDTree{

    private:

        // Node struct for kd-tree
        struct Node {

            // Left and right children
            Node* left = nullptr;
            Node* right = nullptr;

            // X and Y coordinates stored in pairs. For non-leaf nodes, the unused of the two will be set to -181, since this falls
            // out of the range of any possible coordinates, which helps with debugging.
            pair<float, float> coordinate_pair;

            // Default constructor
            Node(){}

            // Constructor that allows you to specify x and y values
            Node(int x, int y){
                coordinate_pair.first = x;
                coordinate_pair.second = y;
            }
        };

        // Root node
        Node* root;

        void BuildKDTree(vector<pair<float, float>> &coordinate_vector, int depth, Node* root);

    public:
        MyKDTree(){}

        MyKDTree(const vector<vector<string>> &string_coordinate_vector);

        ~MyKDTree();

        void DestroyRecursive(Node* n);

        int RangeSearch(string x_string, string y_string);

        int RangeSearchRecursive(float x_min, float x_max, float y_min, float y_max, int depth, Node* root);
};

// Comparison function to allow the sort algorithm to sort by second value in pairs
bool MyComparison(const pair<int, int> &a,const pair<int, int> &b)
{
    return a.second < b.second;
}

// Constructor for KD Tree, provides base case for recursive build function
// Takes vector of string vectors representing the vector of pairs so that the main doesn't have to
// do this conversion.
MyKDTree::MyKDTree(const vector<vector<string>> &string_coordinate_vector){

    vector<pair<float, float>> coordinate_vector;

    // Switches the coordinate format from strings in a 2d vector of width 2 to a vector of pairs of coordinates
    for (auto i : string_coordinate_vector){
        coordinate_vector.push_back(make_pair(stof(i[0]), stof(i[1])));
    }

    //Base case for BuildKDTree
    // Sorts the new vector using introsort
    sort(coordinate_vector.begin(), coordinate_vector.end());

    // This is to avoid running coordinate_vector.size() repeatedly, since it is so large
    int size = coordinate_vector.size();

    // Sets the node's x value to this the median (or the number to the right of the median, if even)
    // then creates new nodes
    root = new Node(coordinate_vector[size / 2].first, -181);
    root->left = new Node();
    root->right = new Node();

    // The data to the left of the median goes with the left node and the data to the right
    // goes with the right node, both in subvectors.
    vector<pair<float, float>> left_vector(coordinate_vector.begin(), coordinate_vector.begin() + size / 2);
    BuildKDTree(left_vector, 1, root->left);
    vector<pair<float, float>> right_vector(coordinate_vector.begin() + size / 2, coordinate_vector.end());
    BuildKDTree(right_vector, 1, root->right);
}

// Destructor
MyKDTree::~MyKDTree(){
    DestroyRecursive(root);
}

// Uses post order traversal to recursively destroy the tree.
void MyKDTree::DestroyRecursive(Node* n){
    DestroyRecursive(n->left);
    DestroyRecursive(n->right);
    delete n;
}

// Recursively builds the KD tree, alternating between dividing by the x median and y median.
void MyKDTree::BuildKDTree(vector<pair<float, float>> &coordinate_vector, int depth, Node* root){

    // If it runs out of pairs to assign to nodes, return
    if (coordinate_vector.empty()){
        return;
    }

    int size = coordinate_vector.size();
    // if only one node left, turn into leaf
    if (size == 1){
        root->coordinate_pair = coordinate_vector[0];
        return;
    }


    // If depth is even, sort and split by x coordinate
    if (depth % 2 == 0) {
        sort(coordinate_vector.begin(), coordinate_vector.end());

        // -181 is the null value, it is fully outside the range of possible coordinates.
        root->coordinate_pair = make_pair(coordinate_vector[size / 2].first, -181);
        root->left = new Node();
        root->right = new Node();
    }

    // If depth is odd, sort and split by y coordinate
    if (depth % 2 == 1) {
        sort(coordinate_vector.begin(), coordinate_vector.end(), MyComparison);
        root->coordinate_pair = make_pair(-181, coordinate_vector[size / 2].second);
        root->left = new Node();
        root->right = new Node();
    }

    // The data to the left of the median goes with the left node and the data to the right
    // goes with the right node, both in subvectors.
    vector<pair<float, float>> left_vector(coordinate_vector.begin(), coordinate_vector.begin() + size / 2);
    BuildKDTree(left_vector, depth + 1, root->left);
    vector<pair<float, float>> right_vector(coordinate_vector.begin() + size / 2, coordinate_vector.end());
    BuildKDTree(right_vector, depth + 1, root->right);
}

// Base for algorithm to search for all sightings within a -.25 degree square of the given coordinates
int MyKDTree::RangeSearch(string x_string, string y_string){

    // Converting from string format from input in main to float format
    float x = stof(x_string);
    float y = stof(y_string);

    // Runs recursive search algo
    return RangeSearchRecursive(x - 0.25, x + 0.25, y - 0.25, y + 0.25, 0, root);
}

// Recursively searches the tree for points within the box area detailed by x and y constraints in O(sqrt(n)) time
int MyKDTree::RangeSearchRecursive(float x_min, float x_max, float y_min, float y_max, int depth, Node* root){

    // Number of nodes within the area found in the tree below it
    int sum = 0;

    // Checks if it's reached a leaf node (the only place where actual coordinates are stored)
    if (root->left == nullptr && root->right == nullptr){

        // Checks if the coordinates match the constraints, if so adds 1 to the
        if (root->coordinate_pair.first > x_min && root->coordinate_pair.first < x_max
            && root->coordinate_pair.second > y_min && root->coordinate_pair.second < y_max){

            cout << "sighting found" << endl;
            return 1;

        } else {
            return 0;
        }
    }

    // If even, it must use x coordinates to determine whether both or one of the sides of the median potentially
    // falls within the box area. If odd, it will do this with y coordinates.
    if (depth % 2 == 0){

        // Only goes down a branch of the tree that is on the side of the median with the box area.
        if (root->coordinate_pair.first >= x_min && root->left != nullptr){
            sum += RangeSearchRecursive(x_min, x_max, y_min, y_max, depth + 1, root->left);
        }
        if (root->coordinate_pair.first <= x_max && root->right != nullptr){
            sum += RangeSearchRecursive(x_min, x_max, y_min, y_max, depth + 1, root->right);
        }

    } else if (depth % 2 == 1){
        if (root->coordinate_pair.second >= y_min && root->left != nullptr){
            sum += RangeSearchRecursive(x_min, x_max, y_min, y_max, depth + 1, root->left);
        }
        if (root->coordinate_pair.second <= y_max && root->right != nullptr){
            sum += RangeSearchRecursive(x_min, x_max, y_min, y_max, depth + 1, root->right);
        }
    }

    return sum;
}

#endif //COP3530MENINBLACK_MY_KD_TREE_H
