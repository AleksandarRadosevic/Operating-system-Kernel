/*
 * List
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */


#ifndef LIST_
#define LIST_

#include"thread.h"

typedef struct node{
public:
	PCB *myPCB;
	struct node *right;
	struct node *left;
	node(PCB *p,node*l=0,node*r=0){
		myPCB=p;
		left=l;
		right=r;
	}
	~node()
	{
		left=0;
		right=0;
		myPCB=0;
	}

};

class List
{
public:
	List();
	void push_back(PCB *p);
	void pop_back();
	void pop_front();
	node *get_first(){return front_;}
	void remove(node *);
	PCB *front();
	PCB *rear();
	PCB *operator[](int);
	int empty();
	void delete_list();
	virtual ~List();



int size;
private:
	node *front_;
	node *rear_;
};




class KernelSem;

typedef struct SEM{
public:
	KernelSem *mySEM;
	struct SEM *right;
	struct SEM *left;
	SEM(KernelSem *p,SEM*l=0,SEM*r=0){
		mySEM=p;
		left=l;
		right=r;
	}
	~SEM()
	{
		left=0;
		right=0;
		mySEM=0;
	}

};

class LIST_SEMAPHORE
{
public:
	LIST_SEMAPHORE();
	void push_back(KernelSem *p);
	void pop_front();
	KernelSem *front();
	KernelSem *rear();
	KernelSem *operator[](int);
	int empty();
	virtual ~LIST_SEMAPHORE();


int size;
private:
	SEM *front_;
	SEM *rear_;
};

enum Signal_State;
typedef struct Signal{
	SignalHandler function;
	Signal *prev;
	Signal *next;
	Signal (SignalHandler s)
	{
		function=s;
		prev=next=0;

	}
	~Signal(){
		function=0;
		prev=0;
		next=0;

	}

};




class LIST_SIGNAL{
public:
	Signal_State state;
	Signal *front;
	Signal *rear;
	int size;

	LIST_SIGNAL();
	void Add_signal_hendler(SignalHandler s);
	void Pop_all();

	void Swap(SignalHandler first,SignalHandler second);
	void Set_state(Signal_State s);

	void Execute_all_functions();
	LIST_SIGNAL& operator=(const LIST_SIGNAL &s);

	virtual ~LIST_SIGNAL(){
		Pop_all();
	}
};

typedef struct Flag{
	int val;
	Flag *next;
	Flag *prev;
	Flag(int i){
		val=i;
		next=0;
		prev=0;
	}
	~Flag(){
		val=0;
		next=0;
		prev=0;
	}
};
class List_flags{
public:
	int size;
	Flag *front;
	Flag *rear;
	List_flags(){
		size=0;
		front=0;
		rear=0;
	}
	void Add_flag(int i);

	void Remove_flag(int i);
	List_flags& operator=(const List_flags& lista);
};
#endif /* LIST_ */
