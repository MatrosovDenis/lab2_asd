#pragma once

#include <memory>
#include <sstream>
#include <iostream>
#include "node.h"

class HList
{
public:
    typedef std::shared_ptr<HList> SPtr;

private:
    Node::SPtr head_ = nullptr;

public:
    HList();

    HList(const HList& list);
    HList& operator=(const HList& list);

    bool isContain(const std::string& adress) const;
    std::string getAdress(const std::string& name) const;
    Node get(const std::string& adress) const;

    bool insert(const std::string& adress, const Node& node);
    bool moveTo(const std::string& adress, const std::string& newAdress);

    bool remove(const std::string& adress);
    bool rename(const std::string& adress, const std::string& newName);

    void clear();

    std::string toStr(std::string prefix) const;
};
