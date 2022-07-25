#include "tree.h"
#include "common.h"
#include <array>
Tree::Tree() {

    root.data = InvalidData;

    root.children.clear();

}


Tree::~Tree() {}


bool Tree::Subtree(Tree t) {

    bool isSubtree = false;

    return isSubtree;

}


bool Tree::Remove(Tree t) {

    bool isRemoved = false;

    return isRemoved;

}


void Tree::Clear() {}

int ExtractNumbers(std::string &line, std::vector<int> &numbers) {
    int result = 0;
    std::string number;

    for (int i = 0; i < line.size(); i++)
    {
        
        while (i < line.size() && std::iswspace(line[i]))
        {
            i++;
        }

        while (i < line.size() && std::iswspace(line[i]) == 0)
        {
            number+= line[i];
            i++;
        }
        numbers.push_back(atoi(number.c_str()));
        number.clear();
    }

    return result;
}

void Tree::Import(std::string fileName) {

    std::vector<std::vector<int>> levels;
    std::vector<int> level;
    std::ifstream tree;
    std::string line;
    int result = 0;
    Node tmp_root;

    tree.open(fileName);

    if (tree.is_open()) 
    {

        while (std::getline(tree, line, '|'))
        {

            if (line.size())
            {
                result = ExtractNumbers(line, level);

                if (!result)
                {
                    levels.push_back(level);
                    if (tmp_root.data == InvalidData && level.size() == 1)
                    {
                        tmp_root.data = level[0];
                    }
                }
                level.clear();
            }

        }
    }

    int i = 0;
    for (auto level : levels)
    {
        std::cout << "level i:" << i << std::endl;
        for (auto number : level)
        {
            std::cout << number << " "; 
        }
        std::cout << std::endl;
        i++;
    }

    tree.close();
}
//LOAD d /home/kon/workspace/fmi/20212022/sdp/homework/ProjectTree/build/test1.txt

void Tree::Export(std::string fileName) {

    

}


bool Tree::operator==(const Tree & t) {

    bool isEqual = false;
    

    return isEqual;

}