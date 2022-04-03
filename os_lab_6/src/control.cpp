#include <iostream>
#include <vector>
#include <zmq.hpp>
#include <unistd.h>
#include "TopologyTree.hpp"
#include <set>

zmq::context_t context;

int main()
{
    TopologyTree* topologyTree = new TopologyTree();
    std::set<int> existingNodes;

    std::string command;
    int id;

    std::cout << "> ";
    while(std::cin >> command){
        if(command == "ping")
        {
            std::cin >> id;
            TopologyNode* node = topologyTree->findNode(topologyTree->root, id);
            if(node == nullptr && existingNodes.find(id) == existingNodes.end())
            {
                std::cout << "Error: Not found\n";
            }
            else if(node == nullptr && existingNodes.find(id) != existingNodes.end())
            {
                std::cout << "Ok: 0\n";
            }
            else
            {
                std::string msgOut = command + "|" + std::to_string(id);
                zmq::message_t zOut(msgOut);
                node->socket.send(zOut, zmq::send_flags::none);

                zmq::message_t zIn;
                zmq::recv_result_t rc = node->socket.recv(zIn);
                if(rc.value() == -1ul)
                {
                    return 1;
                }
                else if(!rc.has_value())
                {
                    existingNodes.erase(id);
                    topologyTree->removeNode(id);
                }
                else
                {
                    std::cout << zIn.to_string() << std::endl;
                }
            }
        }
        else if(command == "create")
        {
            std::cin >> id;
            int parentId;
            std::cin >> parentId;
            if(existingNodes.find(id) != existingNodes.end())
            {
                std::cout << "Error: Already exists\n";
            }
            else if(topologyTree->findNode(topologyTree->root, parentId) == nullptr
                    && existingNodes.find(parentId) == existingNodes.end())
            {
                std::cout << "Error: Parent not found\n";
            }
            else if(topologyTree->findNode(topologyTree->root, parentId) == nullptr
                    && existingNodes.find(parentId) != existingNodes.end())
            {
                std::cout << "Error: Parent is unavailable\n";
            }
            else
            {
                TopologyNode* node = topologyTree->addNode(id, parentId);

                node->socket = zmq::socket_t(context, zmq::socket_type::req);
                node->socket.setsockopt(ZMQ_RCVTIMEO, 2000);
                const std::string addr = "tcp://127.0.0.1:" + std::to_string(5555 + id);
                node->socket.connect(addr);
                existingNodes.insert(id);
                topologyTree->print();
                std::cout << std::endl;
                int pid = fork();
                if(pid == 0)
                {
                    execl("calc.out", addr.c_str(), NULL);
                }
                else if(pid > 0)
                {
                    std::cout << "Ok: " << pid << "\n";
                }
            }

        }
        else if(command == "exec")
        {
            std::cin >> id;
            TopologyNode* node = topologyTree->findNode(topologyTree->root, id);
            if(node == nullptr && existingNodes.find(id) == existingNodes.end())
            {
                std::cout << "Error: Not found\n";
            }
            else if(node == nullptr && existingNodes.find(id) != existingNodes.end())
            {
                std::cout << "Error: Node is unavailable\n";
            }
            else
            {
                std::string key;
                std::cin >> key;
                int value;
                std::string msgOut;
                if(getchar() == ' ')
                {
                    std::cin >> value;
                    msgOut = command + "|" + std::to_string(id) + "|" + key + "|" + std::to_string(value);
                }
                else
                {
                    msgOut = command + "|" + std::to_string(id) + "|" + key;
                }

                zmq::message_t zOut(msgOut);
                node->socket.send(zOut, zmq::send_flags::none);

                zmq::message_t zIn;
                if(node->socket.recv(zIn).value() == -1ul)
                {
                    return 1;
                }
                std::cout << zIn.to_string() << std::endl;

            }
        }
        else if(command == "kill")
        {
            std::cin >> id;
            TopologyNode* node = topologyTree->findNode(topologyTree->root, id);
            std::string msgOut = command + "|" + std::to_string(id);
            zmq::message_t zOut(msgOut);
            node->socket.send(zOut, zmq::send_flags::none);
            existingNodes.erase(id);
            topologyTree->removeNode(id);
        }
        else if(command == "exit")
        {
            break;
        }
        else
        {
            std::cout << "Error: Unknown command\n";
        }
        std::cout << "> ";
    }
    topologyTree->clear();
    delete topologyTree;
    return 0;
}
