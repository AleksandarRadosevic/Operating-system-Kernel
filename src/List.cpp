/*
 * List.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#include"pcb.h"
#include"semaphor.h"
extern volatile unsigned lockFlag;
//LIST PCB
//LIST PCB
//LIST PCB

List::List()
{
	front_=0;
	rear_=0;
	size=0;
}

void List::push_back(PCB *p){
#ifndef BCC_BLOCK_IGNORE
	HARD_LOCK
	node *nn=new node(p);
	HARD_UNLOCK
	if (nn!=0){
		size++;

	if (front_==0)
	{
		nn->left=0;
		nn->right=0;
		front_=nn;
		rear_=nn;

	}
	else {
		rear_->right=nn;
		nn->left=rear_;
		rear_=nn;
	}
	}
#endif

}
void List::pop_back()
{
	if (front_!=0)
		{
		--size;
		if (front_!=rear_)
		{
			node *old=rear_;
			rear_=rear_->left;
			rear_->right=0;
			old=0;
		}
		else {

			front_=0;
			rear_=0;
			size=0;
		}
	}

}

void List::pop_front()
{
	if (front_!=0)
			{
			--size;
			if (front_!=rear_)
			{
				node *old=front_;
				front_=front_->right;
				front_->left=0;
				old=0;
			}
			else {
				rear_=0;
				front_=0;
				size=0;
			}
		}

}

void List::remove(node *old){
if (size>0){
	if (old==front_)
	{
		if (front_==rear_)
		{
			front_=rear_=old=0;
		}
		else
		{
			front_=front_->right;
			front_->left=0;
			old=0;
		}
	}
	else if (old==rear_)
	{
		if (front_==rear_)
				{
					front_=rear_=old=0;
				}
		else
		{
			rear_=rear_->left;
			rear_->right=0;
			old=0;
		}

	}
	else
	{
		node *levi=old->left;
		node *desni=old->right;
		if(levi!=0)
		levi->right=desni;
		if (desni!=0)
		desni->left=levi;
		old=0;
	}
size--;
}
}

PCB* List::front(){
	if(front_!=0)
	return front_->myPCB;
	return 0;
}
PCB* List::rear(){
	if (rear_!=0)
	return rear_->myPCB;
	return 0;
}

PCB* List::operator [](int n){
	node *nn=front_;
	int i=0;
	while (nn!=0 && i<n){
		nn=nn->right;
		i++;
	}
	if (nn)
	return nn->myPCB;
	else return 0;
}


int List::empty(){
	if (size==0) return 1;
	else return 0;
}


void List::delete_list(){
	node *nn=front_;
		while (nn!=0)
		{
			node *old=nn;
			nn=nn->right;
			nn->left=0;
			old=0;
		}
size=0;
}

List::~List()
{
	node *nn=front_;
	while (nn!=0)
	{
			node *old=nn;
			nn=nn->right;
			nn->left=0;
			old=0;
	}
size=0;
}

//LIST SEMAPHORE
//LIST SEMAPHORE

LIST_SEMAPHORE::LIST_SEMAPHORE()
{
	front_=0;
	rear_=0;
	size=0;
}

void LIST_SEMAPHORE::push_back(KernelSem *p){
#ifndef BCC_BLOCK_IGNORE
	HARD_LOCK
	SEM *nn=new SEM(p);
	HARD_UNLOCK
	if (nn!=0){
	if (front_==0)
	{
		nn->left=0;
		nn->right=0;
		front_=nn;
		rear_=nn;

	}
	else {
		rear_->right=nn;
		nn->left=rear_;
		rear_=nn;
	}
	size++;
	}
#endif
}
void LIST_SEMAPHORE::pop_front()
{
	if (front_!=0)
		{
		--size;
		if (front_!=rear_)
		{
			front_=front_->right;
			front_->left=0;
		}
		else {
			rear_=0;
			front_=0;
			size=0;
		}
	}

}
KernelSem* LIST_SEMAPHORE::front(){
	if (front_!=0)
	return front_->mySEM;
	else return 0;
}
KernelSem* LIST_SEMAPHORE::rear(){
	if (rear_!=0)
	return rear_->mySEM;
	else return 0;
}

KernelSem* LIST_SEMAPHORE::operator [](int n){
	SEM *nn=front_;
	int i=0;
	while (nn!=0 && i<n){
		nn=nn->right;
		i++;
	}
	if (nn)
	return nn->mySEM;
	else return 0;
}

int LIST_SEMAPHORE::empty(){
	if (size==0) return 1;
	else return 0;
}

LIST_SEMAPHORE::~LIST_SEMAPHORE()
{
	SEM *nn=front_;
		while (nn!=0)
		{
			SEM *old=nn;
				nn=nn->right;
				nn->left=0;
				old=0;
		}
	size=0;
}


//LIST SIGNAL
//LIST SIGNAL


LIST_SIGNAL::LIST_SIGNAL(){
	front=0;
	rear=0;
	size=0;
	this->state=Signal_READY;
}


void LIST_SIGNAL::Execute_all_functions(){

	Signal *temp=front;
	if (state==Signal_READY && size>0)
	while (temp!=0){
	temp->function();
	temp=temp->next;
	}
}


void LIST_SIGNAL::Add_signal_hendler(SignalHandler s){

	Signal *ss=new Signal(s);

	if (ss!=0){
	if (size==0)
	{
		front=rear=ss;

	}
	else
	{
		rear->next=ss;
		ss->prev=rear;
		rear=ss;
	}
	size++;
}

}

void LIST_SIGNAL::Pop_all(){
	Signal *temp=front;
	while (temp!=0){
		Signal *old=temp;
		temp=temp->next;
		LOCK
		delete old;
		UNLOCK
	}
	size=0;
}

void LIST_SIGNAL::Swap(SignalHandler first,SignalHandler second)
{
		int val=0;
		Signal *temp=front;
		Signal *first_signal;
		Signal *second_signal;
		while (temp!=0 && val!=2){
			if (temp->function==first){
				val++;
				first_signal=temp;
			}
			if (temp->function==second){
				val++;
				second_signal=temp;
				break;
			}
			temp=temp->next;
		}

		if (val==2)
		{
			SignalHandler temp_signal_handler=first_signal->function;
			first_signal->function=second_signal->function;
			second_signal->function=temp_signal_handler;
		}


	}

void LIST_SIGNAL::Set_state(Signal_State s)
{

	this->state=s;
}

LIST_SIGNAL& LIST_SIGNAL::operator =(const LIST_SIGNAL &s){
	Signal *temp_signal=s.front;
	while (temp_signal!=0){
		Add_signal_hendler(temp_signal->function);
		temp_signal=temp_signal->next;
	}

	return *this;

}


List_flags& List_flags::operator =(const List_flags &lista)
{

			Flag *temp_signal=lista.front;

			while (temp_signal!=0){
				Add_flag(temp_signal->val);
				temp_signal=temp_signal->next;
			}

			return *this;


}

void List_flags::Remove_flag(int i){
	Flag *f=front;
	while (f->val!=i){
		f=f->next;
	}

	if (f==front && f==rear){
		front=0;
		rear=0;
	}
	else {

	if (f==front){
		front=front->next;
		front->prev=0;

	}
	else if (f==rear){
		rear=rear->prev;
		rear->next=0;

	}
	else {
		Flag *left=f->prev;
		Flag *right=f->next;
		left->next=right;
		right->prev=left;

	}

	}
	delete f;
	size--;
}




void List_flags::Add_flag(int i){

	LOCK
	Flag *f=new Flag(i);
	UNLOCK
	if (f!=0){
	if (size==0){
		front=rear=f;

	}
	else {
		rear->next=f;
		f->prev=rear;
		rear=f;
	}


	size++;
	}
}







