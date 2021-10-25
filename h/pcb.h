/*
 * pcb.h
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */
#include<DOS.H>
#include"thread.h"
#include"List.h"
#include"SCHEDULE.H"
#include<IOSTREAM.H>
#ifndef PCB_H_
#define PCB_H_

enum State{INIT,READY,RUNNING,FINISHED,BLOCKED};
enum Signal_State{Signal_READY,Signal_BLOCKED,Signal_FINISHED};


class SignalThread:public Thread{
public:

	SignalThread(PCB *m){
		myPCB=m;
	}
virtual ~SignalThread(){
	waitToComplete();
}
};



class PCB{
public:

	PCB(unsigned long stackSize = 4096, unsigned int timeSlice =2,Thread *t=0);
	static void wrapper();


	PCB *parent;


	LIST_SIGNAL signals[16];
	List_flags flags_signals;
	void Execute_signals();

	static PCB *running;
	static ID id;
	static List list_;
	static void BlockSignalGlobally(SignalId id,int val);


	List blocked_list;
	Thread *myThread;
	ID myId;

	unsigned *stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;

	unsigned timeSlice;
	unsigned time_2;
	//time_2 je zapravo timeSlice
	unsigned int Semaphore_timeSlice;


	State state_;

	virtual ~PCB();
};
#endif /* PCB_H_ */


#ifndef HARD_LOCK
#define HARD_LOCK asm{\
pushf;\
cli;\
}
#endif
#ifndef HARD_UNLOCK
#define HARD_UNLOCK asm popf
#endif


#ifndef LOCK
#define LOCK lockFlag=1;
#endif
#ifndef UNLOCK
#define UNLOCK lockFlag=0;
#endif
