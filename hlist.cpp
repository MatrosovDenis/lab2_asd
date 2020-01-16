#include "hlist.h"

HList::HList()
{

}

HList::HList(const HList& list)
{
    if(list.head_ == nullptr)
    {
        head_ = nullptr;
        return;
    }
    head_ = std::make_shared<Node>(*(list.head_));
    Node::SPtr curNode = head_;
    Node::SPtr buf = list.head_->next();

    while(buf)
    {
        Node::SPtr newNode = std::make_shared<Node>(*buf);
        newNode->setPrev(curNode);
        curNode->setNext(newNode);
        curNode = newNode;
        buf = buf->next();
    }
}

HList& HList::operator=(const HList& list)
{
    if(head_ == nullptr)
    {
        return *this;
    }

    head_ = std::make_shared<Node>(*(list.head_));
    Node::SPtr curNode = head_;

    Node::SPtr buf = list.head_->next();

    while(buf)
    {
        Node::SPtr newNode = std::make_shared<Node>(*buf);
        newNode->setPrev(curNode);
        curNode->setNext(newNode);
        curNode = newNode;
        buf = buf->next();
    }

    return *this;
}

bool HList::isContain(const std::string& adress) const
{
    if(adress == "")
    {
        return false;
    }

    std::stringstream adressStream(adress);

    int num = 0;
    if(adressStream.peek() == '.')
    {
        adressStream.ignore(1, EOF);
    }
    adressStream >> num;

    Node::SPtr buf = head_;

    while(buf)
    {
        if(buf->number() == num)
        {
            if(adressStream.peek() == EOF)
            {
                return true;
            }
            else
            {
                if(buf->hasText())
                {
                    return false;
                }

                std::string newAdress;
                adressStream >> newAdress;

                return buf->subList()->isContain(newAdress);
            }
        }
        else if (num > buf->number())
        {
            buf = buf->next();
        }
        else
        {
            return false;
        }
    }

    return false;
}

std::string HList::getAdress(const std::string& name) const
{
    std::stringstream adress;

    Node::SPtr buf = head_;

    while(buf)
    {
        if(buf->name() == name)
        {
            adress << buf->number();

            return adress.str();
        }
        else if(!buf->hasText())
        {
            std::string adressBuf = buf->subList()->getAdress(name);
            if(adressBuf != "")
            {
                adress << buf->number() << "." << adressBuf;

                return adress.str();
            }
        }

        buf = buf->next();
    }

    return "";
}

Node HList::get(const std::string& adress) const
{
        std::stringstream adressStream(adress);
        Node::SPtr buf = head_;
        int num = 0;
        if(adressStream.peek() == '.')
        {
            adressStream.ignore(1, EOF);
        }
        adressStream >> num;

        while(buf)
        {
            if(buf->number() == num)
            {
                if(adressStream.peek() == EOF)
                {
                    return *buf;
                }
                else
                {
                    std::string newAdress;
                    adressStream >> newAdress;

                    return buf->subList()->get(newAdress);
                }
            }
            else if (num > buf->number())
            {
                buf = buf->next();
            }
            else
            {
                break;
            }
        }

    std::cout << "Cann't get. Adress didn't exist!" << std::endl;

    return Node();
}

bool HList::insert(const std::string& adress, const Node& node)
{
    if(adress == "")
    {
        std::cout << "Can't insert. Wrong adress!" << std::endl;
        return false;
    }
    if(isContain(adress))
    {
        std::cout << "Can't insert. Adress already taken!" << std::endl;
        return false;
    }

    std::stringstream adressStream(adress);
    int num = 0;
    if(adressStream.peek() == '.')
    {
        adressStream.ignore(1, EOF);
    }
    adressStream >> num;

    Node::SPtr buf = head_;

    if(buf == nullptr && adressStream.peek() == EOF)
    {
        Node::SPtr newNode = std::make_shared<Node>(node);
        newNode->setNumber(num);

        head_ = newNode;
        return true;
    }

    if(num < head_->number())
    {
        Node::SPtr newNode = std::make_shared<Node>(node);
        newNode->setNumber(num);

        newNode->setNext(buf);

        buf->setPrev(newNode);

        head_ = newNode;

        return true;
    }

    while(buf)
    {
        if(adressStream.peek() == EOF)
        {
            if(buf->number() > num || buf->next() == nullptr)
            {
                Node::SPtr newNode = std::make_shared<Node>(node);
                newNode->setNumber(num);

                if(buf->next() != nullptr)
                {
                    newNode->setPrev(buf->prev());
                    newNode->setNext(buf);

                    if(buf->prev() != nullptr)
                    {
                        buf->prev()->setNext(newNode);
                    }
                    buf->setPrev(newNode);
                }
                else
                {
                    newNode->setPrev(buf);
                    newNode->setNext(buf->next());

                    if(buf->next())
                    {
                        buf->next()->setPrev(newNode);
                    }
                    buf->setNext(newNode);
                }

                return true;
            }
            else if (num > buf->number())
            {
                buf = buf->next();
            }
            else
            {
                std::cout << "Can't insert. Wrong adress!" << std::endl;
                return false;
            }
        }
        else {
            if(buf->number() == num)
            {
                if(buf->hasText())
                {
                    std::cout << "Can't insert. Wrong adress!" << std::endl;
                    return false;
                }

                std::string newAdress;
                adressStream >> newAdress;

                return buf->subList()->insert(newAdress, node);
            }
            else if (num > buf->number())
            {
                buf = buf->next();
            }
            else
            {
                std::cout << "Can't insert. Wrong adress!" << std::endl;
                return false;
            }
        }
    }

    std::cout << "Can't insert. Wrong adress!" << std::endl;
    return false;
}

bool HList::moveTo(const std::string& adress, const std::string& newAdress)
{
    if(!isContain(adress))
    {
         std::cout << "Can't Move. Old adress didn't exist!" << std::endl;
         return false;
    }
    if(isContain(newAdress))
    {
        std::cout << "Can't Move. New adress already taken." << std::endl;
        return false;
    }

    Node buf = get(adress);
    if(insert(newAdress, buf))
    {
        remove(adress);
        return true;
    }
    else
    {
        std::cout << "Can't Move. Wrong new adress.." << std::endl;
        return false;
    }
}

bool HList::remove(const std::string& adress)
{
    if(adress == "")
    {
        std::cout << "Can't remove. Wrong adress!" << std::endl;
        return false;
    }
    if(!isContain(adress))
    {
        std::cout << "Can't remove. Wrong adress!" << std::endl;
        return false;
    }

    std::stringstream adressStream(adress);
    int num = 0;
    if(adressStream.peek() == '.')
    {
        adressStream.ignore(1, EOF);
    }
    adressStream >> num;

    Node::SPtr buf = head_;

    while(buf)
    {
        if(adressStream.peek() == EOF)
        {
            if(buf->number() == num)
            {
                if(buf->prev())
                {
                    buf->prev()->setNext(buf->next());
                }
                if(buf->next())
                {
                    buf->next()->setPrev(buf->prev());
                }
                if(buf == head_)
                {
                    if(buf->prev())
                    {
                        head_ = buf->prev();
                    }
                    else if (buf->next())
                    {
                        head_ = buf->next();
                    }
                    else
                    {
                        head_ = nullptr;
                    }
                }

                return true;
            }
            else if (num > buf->number())
            {
                buf = buf->next();
            }
            else
            {
                std::cout << "Can't remove. Wrong adress!" << std::endl;
                return false;
            }
        }
        else {
            if(buf->number() == num)
            {
                if(buf->hasText())
                {
                    std::cout << "Can't remove. Wrong adress!" << std::endl;
                    return false;
                }

                std::string newAdress;
                adressStream >> newAdress;

                return buf->subList()->remove(newAdress);
            }
            else if (num > buf->number())
            {
                buf = buf->next();
            }
            else
            {
                std::cout << "Can't remove. Wrong adress!" << std::endl;
                return false;
            }
        }
    }

    std::cout << "Can't remove. Wrong adress!" << std::endl;
    return false;
}

bool HList::rename(const std::string& adress, const std::string& newName)
{
    if(adress == "")
    {
        std::cout << "Can't rename. Wrong adress!" << std::endl;
        return false;
    }
    if(!isContain(adress))
    {
        std::cout << "Can't rename. Wrong adress!" << std::endl;
        return false;
    }

    std::stringstream adressStream(adress);
    int num = 0;
    if(adressStream.peek() == '.')
    {
        adressStream.ignore(1, EOF);
    }
    adressStream >> num;

    Node::SPtr buf = head_;

    while(buf)
    {
        if(adressStream.peek() == EOF)
        {
            if(buf->number() == num)
            {
                buf->rename(newName);
                return true;
            }
            else if (num > buf->number())
            {
                buf = buf->next();
            }
            else
            {
                std::cout << "Can't rename. Wrong adress!" << std::endl;
                return false;
            }
        }
        else {
            if(buf->number() == num)
            {
                if(buf->hasText())
                {
                    std::cout << "Can't rename. Wrong adress!" << std::endl;
                    return false;
                }

                std::string newAdress;
                adressStream >> newAdress;

                return buf->subList()->rename(newAdress, newName);
            }
            else if (num > buf->number())
            {
                buf = buf->next();
            }
            else
            {
                std::cout << "Can't rename. Wrong adress!" << std::endl;
                return false;
            }
        }
    }

    std::cout << "Can't rename. Wrong adress!" << std::endl;
    return false;
}

std::string HList::toStr(std::string prefix) const
{
    std::stringstream result;

    Node::SPtr buf = head_;

    while(buf)
    {
        if(prefix == "")
        {
            result << buf->number();
        }
        else
        {
            result << prefix <<  "." << buf->number();
        }
        result << " " << buf->toStr() << std::endl;

        if(!buf->hasText())
        {
            std::string newPrefix;
            if(prefix == "")
            {
                newPrefix = std::to_string(buf->number());
            }
            else
            {
                newPrefix = prefix + "." + std::to_string(buf->number());
            }
            result << buf->subList()->toStr(newPrefix);
        }

        buf = buf->next();
    }

    return result.str();
}

void HList::clear()
{
    head_ = nullptr;
}
