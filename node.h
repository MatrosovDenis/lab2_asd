#pragma once

#include <memory>
#include <variant>
#include <string>

class HList;

class Node
{
public:
    typedef std::shared_ptr<Node> SPtr;
    typedef std::weak_ptr<Node> WPtr;

private:
    std::string name_ = "";
    int number_ = 0;
    bool hasText_ = true;
    std::variant<std::string, std::shared_ptr<HList>> content_ = "";

    SPtr next_ = nullptr;
    WPtr prev_;

public:
    Node() = default;
    Node(const std::string& name);
    Node(const std::string&, std::string text);

    Node(const Node& node);
    Node& operator=(const Node& node);

    const std::string& name() const;
    void rename(const std::string& name);

    int number() const;
    void setNumber(int number);

    bool hasText() const;
    const std::string& text() const;

    const std::shared_ptr<HList> subList() const;

    void setNext(SPtr node);
    void setPrev(SPtr node);
    const SPtr next() const;
    const SPtr prev() const;

    std::string toStr() const;
};
