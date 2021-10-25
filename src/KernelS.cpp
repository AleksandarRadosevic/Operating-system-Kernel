/*
 * KernelSem.cpp
 *
 *  Created on: May 10, 2020
 *      Author: OS1
 */
#include "KernelS.h"
#include"STDIO.H"
LIST_SEMAPHORE KernelSem::semaphores;

extern volatile unsigned int lockFlag;
KernelSem::KernelSem(int init,Semaphore *s){
	LOCK
	value=init;
	mySem=s;
	semaphores.push_back(this);
	UNLOCK
}

int KernelSem::wait(Time maxTimeToWait)
{
	value--;
if (maxTimeToWait==0)
{

	if (value<0)
	{
		LOCK
		PCB::running->state_=BLOCKED;
		list_val.push_back(PCB::running);
		UNLOCK
		dispatch();
	}

}

else if(maxTimeToWait>0){

	LOCK
	PCB::running->state_=BLOCKED;
	PCB::running->Semaphore_timeSlice=maxTimeToWait;
	list_wait.push_back(PCB::running);
	UNLOCK
	dispatch();
	if (PCB::running->Semaphore_timeSlice==0)
	return 0;

}
return 1;
}

int KernelSem::signal(int n)
{
	if (n>0)
	{
		value=value+n;
		if (n<=list_wait.size){
			LOCK
			for (int i=0;i<n;i++)
			{
				PCB *temp=list_wait.front();
				list_wait.pop_front();
				temp->state_=READY;
				Scheduler::put(temp);

			}
			UNLOCK
		return n;
		}

		else {
			int i=0;
			LOCK
			while (!list_wait.empty()){
			PCB *temp=list_wait.front();
			list_wait.pop_front();
			temp->state_=READY;
			Scheduler::put(temp);
			i++;
			}


			while (!list_val.empty())
			{
				PCB *temp=list_val.front();
				list_val.pop_front();
				temp->state_=READY;
				Scheduler::put(temp);
				i++;
				if (i==n) break;
			}

			UNLOCK
			return i;
		}


	}
	else if (n==0)
	{
		value++;
		if (value<=0)
		{
			LOCK
			PCB *temp=list_val.front();
			list_val.pop_front();
			temp->state_=READY;
			UNLOCK
			Scheduler::put(temp);
		}
		return 0;
	}
	else
	{
		return n;
	}

}


KernelSem::~KernelSem(){
	mySem=0;
	value=0;
}



