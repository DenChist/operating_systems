#ifndef __TOPOLOGY_TREE_HPP__
#define __TOPOLOGY_TREE_HPP__

#include "TopologyNode.hpp"

class TopologyTree
{
public:
    TopologyNode* root;

    TopologyTree()
    {
        root = new TopologyNode(nullptr, -1);
    }

    TopologyNode* findNode(TopologyNode* curNode, int id)
    {
        if(curNode != nullptr)
        {
            TopologyNode* ans = nullptr;
            if(!curNode->removed)
            {
                ans = findNode(curNode->oldestChild, id);
            }
            if(ans != nullptr && !ans->removed)
            {
                return ans;
            }
            if(curNode->id == id && !curNode->removed)
            {
                ans = curNode;
                return ans;
            }
            return findNode(curNode->rightBro, id);
        }
        else
        {
            return nullptr;
        }
    }

    TopologyNode* addNode(int nodeId, int parentId)
    {
        TopologyNode* parent = findNode(root, parentId);
        TopologyNode* node = new TopologyNode(parent, nodeId);
        if(parent->oldestChild == nullptr)
        {
            parent->oldestChild = node;
        }
        else
        {
            TopologyNode* leftBro = parent->oldestChild;
            while(leftBro->rightBro != nullptr)
            {
                leftBro = leftBro->rightBro;
            }
            leftBro->rightBro = node;
        }
        return node;
    }
    
    void _recursClear(TopologyNode* curNode)
    {
        if(curNode != nullptr)
        {
            _recursClear(curNode->oldestChild);
            _recursClear(curNode->rightBro);
            curNode->socket.close();
            delete curNode;
        }
    }
    
    void clear()
    {
        _recursClear(root->oldestChild);
        root->oldestChild = nullptr;
    }

    void removeNode(int nodeId)
    {
        TopologyNode* deletedNode = findNode(root, nodeId);
        deletedNode->socket.close();
        deletedNode->removed = true;
    }

    void recursivePrint(TopologyNode* curItem)
    {
        if (curItem != nullptr)
        {
            std::cout << curItem->id;
            if(curItem->removed)
            {
                std::cout << "d";
            }
            
            if (curItem->oldestChild != nullptr)
            {
                std::cout << " : [";
            }

            recursivePrint(curItem->oldestChild);

            if (curItem->rightBro != nullptr)
            {
                std::cout << ", ";
            }
            recursivePrint(curItem->rightBro);
            if (curItem->rightBro == nullptr)
                std::cout << "]";
        }
    }

    void print()
    {
        std::cout << root->id;
        std::cout << " : [";
        recursivePrint(root->oldestChild);
    }
};

#endif
