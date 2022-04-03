#ifndef __TOPOLOGY_NODE_HPP__
#define __TOPOLOGY_NODE_HPP__

#include <zmq.hpp>

class TopologyNode
{
public:
    int id;
    TopologyNode* parent;
    TopologyNode* oldestChild;
    TopologyNode* rightBro;
    bool removed;
    zmq::socket_t socket;

    TopologyNode(TopologyNode* parent, int id)
    {
        this->parent = parent;
        this->oldestChild = nullptr;
        this->rightBro = nullptr;
        this->id = id;
        removed = false;
    }
};

#endif
