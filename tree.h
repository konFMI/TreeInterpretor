#pragma once
#include <string>
#include <climits>
#include <fstream>
#include <vector>

const int InvalidData = INT_MIN;

class Tree {
public:
    Tree();
    ~Tree();

    bool Subtree(Tree t);

    bool Remove(Tree t);
    void Clear();
    void Import(std::string fileName);
    void Export(std::string fileName);
    bool operator==(const Tree & t);

private:
    struct Node {
        int data;
        std::vector<Node> children; 
    };

    Node root;
};
