#include <iostream>
#include <string>
#include <cstring>

using namespace std;

struct nodes
{
    string data;
    nodes *next = NULL;
    int index = -1;
    //tuplas
};

class LinkedList
{
private:
    nodes *head = NULL;
    int size = 0;

public:
    void addData(string data,int index)
    {
        if (head == NULL)
        {
            head = new nodes;
            head->data = data;
            head->index = index;
       
        }
        else if (head->next == NULL)
        {
            head->next = new nodes;
            head->next->data = data;
            head->next->index = index;
          
        }
        else
        {
            nodes *tmp = head;
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            };
            tmp->next = new nodes;
            tmp->next->data = data;
            tmp->next->index = index;
        }
        this->size++;
    }

    string getData(int index)
    {
        if (index < size)
        {
            nodes *tmp = head;
            for (int i = 0; i < index; i++)
            {
                tmp = tmp->next;
            }
            return tmp->data;
        }
        else
        {
            return "Index error";
        }
    }

    int contains(string data){
        nodes *tmp = head;
        int i = 0;
            while (tmp != NULL)
            {
                if(tmp->data == data){
                    return tmp->index;
                }
                tmp = tmp->next;
                i++;
            }
            return -1;
    }

    int getIndex(int index){
        if (index < size)
        {
            nodes *tmp = head;
            for (int i = 0; i < index; i++)
            {
                tmp = tmp->next;
            }
            return tmp->index;
        }
        else
        {
            return -1;
        }
    }

    int getSize()
    {
        return size;
    }
};
