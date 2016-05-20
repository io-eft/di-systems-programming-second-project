#ifndef __CUSTOMLIST__HPP
#define __CUSTOMLIST__HPP
#include <iostream>
#include "myjob.hpp"

class CustomList{
    struct Node{
        MyJob* content;
        Node* next;

        Node(MyJob* mj);
        Node();
	~Node();
    };
    Node* head;
    Node* tail;
    unsigned int items;

    public:
        CustomList();
        ~CustomList();
        bool isEmpty();
        int length();

        void insertEntry(MyJob* mj);
        MyJob* getTop();
        void removeTopEntry();
	MyJob* itemAt(int i);
	void removeEntryByJobId(int l);
	void removeEntryByPid(int l);
	bool jobIdExists(int i);
	bool pidExists(int i);
	int getPidByJobId(int i);	
	MyJob* popTop();

        void print();
	void printToFifo(FILE* fp);
};

#endif

