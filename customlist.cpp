#include "customlist.hpp"

CustomList::Node::Node(): content(NULL), next(NULL) {}

CustomList::Node::Node(MyJob* mj): content(mj), next(NULL){}

CustomList::Node::~Node()
{
    delete content;
}

CustomList::CustomList(): head(NULL), tail(NULL), items(0) {}

CustomList::~CustomList()
{
    Node* temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
}

bool CustomList::isEmpty()
{
    if(head == NULL)
        return true;
    else
        return false;
}

void CustomList::insertEntry(MyJob* mj)
{
    Node* n = new Node(mj);
    if(isEmpty())
    {
        head = n;
        tail = n;
    }
    else
    {
        tail->next = n;
        tail = n;
    }
    items++;
}

MyJob* CustomList::getTop()
{
    return head->content;
}

void CustomList::removeTopEntry()
{
    if(head != NULL)
    {
        Node* temp = head;
        head = head->next;
        items--;
        delete temp;
    }
}

int CustomList::length()
{
    return items;
}

MyJob* CustomList::itemAt(int i)
{
    if(i <= items)
    {
        Node* n = head;
        for(int j = 0; j < i; j++)
            n = n->next;
        return n->content;
    }
    return NULL;
}

MyJob* CustomList::popTop()
{
    if(head != NULL)
    {
        Node* temp = head;
        head = head->next;
        temp->next = NULL;
        items--;
        return temp->content;
    }
    return NULL;
}

void CustomList::removeEntryByJobId(int l)
{
    if(head != NULL)
    {
        Node* temp = head;
        if(head->content->getId() == l)
        {
            if(head == tail)
                tail = NULL;
            head = head->next;
            delete temp;
            items--;
        }
        else
        {
            while(temp->next != NULL)
            {
                if(temp->next->content->getId() == l)
                    break;
                temp = temp->next;
            }
            if(temp->next != NULL)
            {
                Node* toBeDeleted = temp->next;
                temp->next = temp->next->next;
                if(toBeDeleted == tail)
                    tail = temp;
                delete toBeDeleted;
                items--;
            }
        }
    }
}

void CustomList::removeEntryByPid(int l)
{
    if(head != NULL)
    {
        Node* temp = head;
        if(head->content->getPid() == l)
        {
            if(head == tail)
                tail = NULL;
            head = head->next;
            delete temp;
            items--;
        }
        else
        {
            while(temp->next != NULL)
            {
                if(temp->next->content->getPid() == l)
                    break;
                temp = temp->next;
            }
            if(temp->next != NULL)
            {
                Node* toBeDeleted = temp->next;
                temp->next = temp->next->next;
                if(toBeDeleted == tail)
                    tail = temp;
                delete toBeDeleted;
                items--;
            }
        }
    }
}

void CustomList::print()
{
    Node* temp = head;
    while(temp != NULL)
    {
        std::cout << "pid: " << temp->content->getPid() << std::endl;
        temp = temp->next;
    }
}

bool CustomList::pidExists(int i)
{
    Node* temp = head;
    while(temp != NULL)
    {
        if(temp->content->getPid() == i)
            return true;
        temp = temp->next;
    }
    return false;
}

bool CustomList::jobIdExists(int i)
{
    Node* temp = head;
    while(temp != NULL)
    {
        if(temp->content->getId() == i)
            return true;
        temp = temp->next;
    }
    return false;
}

int CustomList::getPidByJobId(int i)
{
    Node* temp = head;
    while(temp != NULL)
    {
        if(temp->content->getId() == i)
            return temp->content->getPid();
        temp = temp->next;
    }
    return -1;
}

void CustomList::printToFifo(FILE* fp)
{
    Node* temp = head;
    if(head == NULL)
        fprintf(fp, "No jobs currently in the queue\n");
    while(temp != NULL)
    {
        temp->content->printPollToFile(fp);
        temp = temp->next;
    }
}
