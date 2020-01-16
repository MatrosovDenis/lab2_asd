#include <iostream>
#include <fstream>
#include "hlist.h"

using namespace std;

void menu(HList& list);
bool readFromFile(HList& list, std::string filePath);
bool readNewNote(HList& list, istream& input);

int main()
{
    setlocale(LC_ALL, "Russian");
    HList list;
    while(true)
    {
        menu(list);
    }
}

void menu(HList& list)
{
    std::string path;
    std::string adress;
    std::string newAdress;
    std::string name;
    std::string newName;
    std::string flushStr;

    unsigned adressNameFlag = 0;

    cout << "Enter command." << endl;
    cout << "1. Read from file." << endl;
    cout << "2. Print list." << endl;
    cout << "3. Insert new note." << endl;
    cout << "4. Remove note." << endl;
    cout << "5. Move note." << endl;
    cout << "6. Rename note." << endl;
    cout << "7. Exit." << endl;

    unsigned command;
    cin >> command;
    getline(cin, flushStr);

    switch (command) {
    case 1:
        cout << "Enter file path." << endl;
        getline(cin, path);
        if(readFromFile(list, path))
        {
            cout << "File successfully readed." << endl;
        }
        else
        {
            cout << "Can't read file." << endl;
        }
        break;
    case 2:
        cout << list.toStr("") << endl;
        break;
    case 3:
        cout << "Enter new note." << endl;

        if(readNewNote(list, cin))
        {
            cout << "Note successfully readed." << endl;
        }
        else
        {
            cout << "Can't read note." << endl;
        }
        break;
    case 4:
        cout << "Enter mode." << endl;
        cout << "1. By adress." << endl;
        cout << "2. By name." << endl;
        cin >> adressNameFlag;
        getline(cin, flushStr);

        if(adressNameFlag == 1)
        {
            cout << "Enter adress." << endl;
            cin >> adress;
            getline(cin, flushStr);
        }
        else if(adressNameFlag == 2)
        {
            cout << "Enter name" << endl;
            cin >> name;
            getline(cin, flushStr);
            adress = list.getAdress(name);
        }

        list.remove(adress);
        break;
    case 5:
        cout << "Enter mode." << endl;
        cout << "1. By adress." << endl;
        cout << "2. By name." << endl;
        cin >> adressNameFlag;
        getline(cin, flushStr);

        if(adressNameFlag == 1)
        {
            cout << "Enter adress." << endl;
            cin >> adress;
            getline(cin, flushStr);
        }
        else if(adressNameFlag == 2)
        {
            cout << "Enter name" << endl;
            cin >> name;
            getline(cin, flushStr);
            adress = list.getAdress(name);
        }

        cout << "Enter new adress" << endl;
        cin >> newAdress;
        getline(cin, flushStr);

        list.moveTo(adress, newAdress);

        break;
    case 6:
        cout << "Enter mode." << endl;
        cout << "1. By adress." << endl;
        cout << "2. By name." << endl;
        cin >> adressNameFlag;
        getline(cin, flushStr);

        if(adressNameFlag == 1)
        {
            cout << "Enter adress." << endl;
            cin >> adress;
            getline(cin, flushStr);
        }
        else if(adressNameFlag == 2)
        {
            cout << "Enter name" << endl;
            cin >> name;
            getline(cin, flushStr);
            adress = list.getAdress(name);
        }

        cout << "Enter new name" << endl;
        cin >> newName;
        getline(cin, flushStr);

        list.rename(adress, newName);

        break;
    case 7:
        exit(0);
    }
}

bool readFromFile(HList& list, std::string filePath)
{
    ifstream file(filePath);
    if(file.is_open())
    {
        list.clear();
        while(file.peek() != EOF)
        {
            readNewNote(list, file);
        }

        return true;
    }
    else
    {
        cout << "Can't open file" << endl;
        return false;
    }
}

bool readNewNote(HList& list, istream& input)
{
    std::string buf;
    std::string adress = "";
    std::string name = "";
    std::string text = "";

    getline(input, buf);
    stringstream lineStream(buf);

    lineStream >> adress;
    while(lineStream.peek() != EOF)
    {
        lineStream >> buf;
        if(buf != "[ ")
        {
            if(name == "")
            {
                name += buf;
            }
            else
            {
                name += " " + buf;
            }
        }
        else
        {
            break;
        }
    }

    if(lineStream.peek() == EOF)
    {
        Node newNode(name);
        return list.insert(adress, newNode);
    }

    while(lineStream.peek() != EOF)
    {
        lineStream >> buf;
        if(buf != "] ")
        {
            if(text == "")
            {
                text += buf;
            }
            else
            {
                text += " " + buf;
            }
        }
        else
        {
            break;
        }
    }

    Node newNode(name, text);

    return list.insert(adress, newNode);
}
