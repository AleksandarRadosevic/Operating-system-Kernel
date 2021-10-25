/*
 * KernelEv.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#include"KernelEv.h"
#include"IVTEntry.h"
extern volatile unsigned int lockFlag;

KernelEv::KernelEv(IVTNo ivtNo){

	IVTEntry::table[ivtNo]->kernel_event=this;
	myPCB=PCB::running;
	value=1;
}

void KernelEv::signal(){

		if (myPCB->state_==BLOCKED){
		LOCK
		myPCB->state_=READY;
		value=1;
		Scheduler::put(myPCB);
		UNLOCK
		}
}

void KernelEv::wait(){

if (PCB::running->myId==myPCB->myId )
{
	if (PCB::running->state_==RUNNING){
	value=0;
	PCB::running->state_=BLOCKED;
	myPCB->state_=BLOCKED;
	dispatch();
	}
}

}
KernelEv::~KernelEv(){
myPCB=0;
event_=0;
}


