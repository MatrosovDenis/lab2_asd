#include "node.h"
#include "hlist.h"

Node::Node(const std::string& name)
{
    name_ = name;
    number_ = 0;
    hasText_ = false;
    content_ = HList::SPtr(new HList);
}

Node::Node(const std::string& name,  std::string text)
{
    name_ = name;
    number_ = 0;
    hasText_ = true;
    content_ = text;
}

Node::Node(const Node& node)
{
    name_ = node.name_;
    number_ = node.number_;
    hasText_ = node.hasText_;

    if(hasText_)
    {
        content_ = node.content_;
    }
    else
    {
        content_ = std::make_shared<HList>(*(node.subList()));
    }
}

Node& Node::operator=(const Node& node)
{
    name_ = node.name_;
    number_ = node.number_;
    hasText_ = node.hasText_;

    if(hasText_)
    {
        content_ = node.content_;
    }
    else
    {
        content_ = std::make_shared<HList>(*(node.subList()));
    }

    return *this;
}

const std::string& Node::name() const
{
    return name_;
}

int Node::number() const
{
    return number_;
}

bool Node::hasText() const
{
    return hasText_;
}

const std::string& Node::text() const
{
        return std::get<std::string>(content_);
}

const std::shared_ptr<HList> Node::subList() const
{
    return std::get<HList::SPtr>(content_);
}

void Node::setNext(SPtr node)
{
    next_ = node;
}

void Node::setPrev(SPtr node)
{
    prev_ = node;
}

const Node::SPtr Node::next() const
{
    return next_;
}

const Node::SPtr Node::prev() const
{
    if(prev_.expired())
    {
        return nullptr;
    }
    else
    {
        return SPtr(prev_);
    }
}

void Node::rename(const std::string& name)
{
    name_ = name;
}

void Node::setNumber(int number)
{
    number_ = number;
}

std::string Node::toStr() const
{
    std::stringstream result;

    result << name_;
    if(hasText_)
    {
        result << " [ " << text() << " ]";
    }

    return result.str();
}
