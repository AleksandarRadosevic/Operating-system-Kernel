/*
 * pcb.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#include "pcb.h"
#include"STDIO.H"
PCB * PCB::running=0;
ID PCB::id=0;
List PCB::list_;

volatile unsigned int lockFlag=0;


PCB::PCB(unsigned long stackSize, unsigned int timeSlice,Thread *t){

#ifndef BCC_BLOCK_IGNORE
	HARD_LOCK
	stackSize=stackSize/sizeof(unsigned);
	stack=new unsigned[stackSize];
	HARD_UNLOCK
	if (stack!=0){

	stack[stackSize-1]=0x200;

	stack[stackSize-2]=FP_SEG(wrapper);
	stack[stackSize-3]=FP_OFF(wrapper);
	}
	else
	{
		LOCK
		printf("NEUSPESNA ALOKACIJA!\n");
		UNLOCK
	}
	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
#endif
	parent=PCB::running;
	bp = sp;
	state_=INIT;
	myThread=t;
	this->timeSlice=timeSlice;
	myId=++id;
	list_.push_back(this);
	this->time_2=timeSlice;
	Semaphore_timeSlice=0;
	LOCK
	if (parent!=0)

	{
		for (int i=0;i<16;i++)
		{

			this->signals[i]=parent->signals[i];

		}
		flags_signals=parent->flags_signals;

	}
	UNLOCK
}

void PCB::wrapper(){

	running->state_=RUNNING;
	running->myThread->run();
	running->state_=FINISHED;
	LOCK
	if (running->parent!=0)
	running->parent->myThread->signal(1);
	running->myThread->signal(2);
	UNLOCK
	running->myThread->waitToComplete();
}



PCB::~PCB(){
	#ifndef BCC_BLOCK_IGNORE
	HARD_LOCK
	delete [] stack;
	HARD_UNLOCK
#endif
}


void PCB::BlockSignalGlobally(SignalId id,int val)
{
	Signal_State state_;
	if (val==0)
		state_=Signal_BLOCKED;
	else state_=Signal_READY;
	for (int i=0;i<PCB::list_.size;i++){
		PCB *tek=list_.operator [](i);
		tek->signals[id].state=state_;
	}


}

void PCB::Execute_signals(){
#ifndef BCC_BLOCK_IGNORE
	asm sti
#endif
	Flag *f=flags_signals.front;
	int val=0;
	while (f!=0)
	{
		val=f->val;
		LOCK
		if (signals[val].state==Signal_READY){

			signals[val].Execute_all_functions();
			f=f->next;
			flags_signals.Remove_flag(val);
		}
		else
		{
			f=f->next;
		}
		UNLOCK
	}


}








