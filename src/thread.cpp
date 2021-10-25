/*
 * thread.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */
#include"pcb.h"
#include"KernelS.h"
int context_switch_on_demand=0;
unsigned oldTimerOFF, oldTimerSEG;
volatile unsigned tsp,tss,btp;
extern volatile unsigned int lockFlag;

class IdleThread: public Thread {
public:
	IdleThread(){}
	void run() {
    while (1)
    {    }
    }
	PCB *get_pcb(){return myPCB;}
};
IdleThread *unlimited_thread=new IdleThread();



void signal0(){

	PCB::running->state_=FINISHED;
	if (PCB::running->parent!=0)
		PCB::running->parent->myThread->signal(1);
		PCB::running->myThread->signal(2);
		delete PCB::running->myThread;

}

void Thread::registerHandler(SignalId signal,SignalHandler handler){
	LOCK
	myPCB->signals[signal].Add_signal_hendler(handler);
	UNLOCK
}


Thread::Thread (StackSize stackSize,Time timeSlice){
	myPCB=new PCB(stackSize,timeSlice,this);

}


Thread::~Thread()
{
waitToComplete();
#ifndef BCC_BLOCK_IGNORE
HARD_LOCK
	delete myPCB;
HARD_UNLOCK
#endif
}

void Thread::start()
{
	//nit je moguce pokrenuti kad se napravi
	if (myPCB->state_==INIT){
	myPCB->state_=READY;
	Scheduler::put(myPCB);
	}

}

ID Thread::getId(){
	if (myPCB!=0)
	return myPCB->myId;
	return 0;
}

ID Thread::getRunningId(){
	if (PCB::running!=0)
	return PCB::running->myId;
	return 0;
}

Thread* Thread::getThreadById(ID a){
	if (PCB::list_[a]!=0)
	return PCB::list_[a]->myThread;
	return 0;
}

void Thread::waitToComplete(){

	//situacija kada se izvrsava t1 i pozove se t2.waitTOcomplete
	if (PCB::running->state_==RUNNING && PCB::running->myId!=myPCB->myId && myPCB->state_!=FINISHED){
	LOCK
	PCB::running->state_=BLOCKED;
	myPCB->blocked_list.push_back(PCB::running);
	UNLOCK
	dispatch();
	}
	//situacija kada se nit zavrsila i treba da se odblokiraju sve niti koje su bile blokirane
	if (PCB::running->state_==FINISHED)
	{
		LOCK
		while (!PCB::running->blocked_list.empty())
		{
			PCB *temp=PCB::running->blocked_list.rear();
			PCB::running->blocked_list.pop_back();
			temp->state_=READY;
			Scheduler::put(temp);

		}
		UNLOCK
	dispatch();
	}
}


void interrupt timer(){
	if (!context_switch_on_demand)
		{
		if (PCB::running->timeSlice!=0)
		PCB::running->time_2--;
		}
		if ((PCB::running->timeSlice!=0 && PCB::running->time_2 == 0) || context_switch_on_demand) {
			if (lockFlag==0){
#ifndef BCC_BLOCK_IGNORE

				asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov btp, bp
			}
#endif
			if (PCB::running->time_2==0 && PCB::running->timeSlice!=0)
				PCB::running->time_2=PCB::running->timeSlice;

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = btp;
			// scheduler
			if (PCB::running->state_!=BLOCKED && PCB::running->state_!=FINISHED){
			PCB::running->state_=READY;
			if (PCB::running->myId!=unlimited_thread->get_pcb()->myId)
			Scheduler::put(PCB::running);

			}
			PCB::running =Scheduler::get();
			if (PCB::running==0)
			{
			PCB::running=unlimited_thread->get_pcb();
			}
			PCB::running->state_=RUNNING;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			btp = PCB::running->bp;
#ifndef BCC_BLOCK_IGNORE

			asm {
				// restaurira sp
				mov sp, tsp
				mov ss, tss
				mov bp,btp
			}
#endif
}
		}
		if(!context_switch_on_demand)
				{
#ifndef BCC_BLOCK_IGNORE

				asm int 60h;
#endif
				if(KernelSem::semaphores.size>0)
				for (int i=0;i<KernelSem::semaphores.size;i++)
				{
					node *rem=0;
					KernelSem *temp=KernelSem::semaphores[i];
					node *t_sem=temp->list_wait.get_first();
					while (t_sem!=0){
					if (--t_sem->myPCB->Semaphore_timeSlice==0)
					{
						rem=t_sem;
					}
					if (rem!=0)
						{
							rem->myPCB->state_=READY;
							Scheduler::put(rem->myPCB);
							temp->list_wait.remove(rem);
							rem=0;
							temp->value++;
						}

					t_sem=t_sem->right;
				}
				}


				tick();
				}
		else
		context_switch_on_demand = 0;
		PCB::running->Execute_signals();



}

void init(){
#ifndef BCC_BLOCK_IGNORE

	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax // es = 0

		// pamti staru rutinu
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		// postavlja novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		// postavlja staru rutinu na int 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
	LOCK
	PCB::running->myThread->registerHandler(0,signal0);
	UNLOCK
}
// vraca staru prekidnu rutinu

void restore(){
#ifndef BCC_BLOCK_IGNORE

	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	HARD_LOCK
	context_switch_on_demand=1;
	timer();
	HARD_UNLOCK
#endif
}

//SIGNALI
//SIGNALI




void Thread::unregisterAllHandlers(SignalId signal){
	LOCK
	myPCB->signals[signal].Pop_all();
	UNLOCK
}



void Thread::swap(SignalId id,SignalHandler hand1,SignalHandler hand2){
	LOCK
	myPCB->signals[id].Swap(hand1,hand2);
	UNLOCK
}



void Thread::blockSignal(SignalId signal){
	LOCK
	myPCB->signals[signal].state=Signal_BLOCKED;
	UNLOCK
}

void Thread::blockSignalGlobally(SignalId signal)
{
	LOCK
	PCB::BlockSignalGlobally(signal,0);
	UNLOCK
}



void Thread::unblockSignal(SignalId signal){
	LOCK
	myPCB->signals[signal].state=Signal_READY;
	UNLOCK
}

void Thread::unblockSignalGlobally(SignalId signal)
{
	LOCK
	PCB::BlockSignalGlobally(signal,1);
	UNLOCK
}


void Thread::signal(SignalId signal){
	LOCK
	if (signal>=3 && signal<=15){
	myPCB->flags_signals.Add_flag(signal);
	}
	if (signal==0)
	{
	myPCB->flags_signals.Add_flag(signal);

	}
	if (signal==1)
	{
		if (PCB::running->state_==FINISHED && myPCB==PCB::running->parent)
		{
			myPCB->flags_signals.Add_flag(signal);
		}
	}
	if (signal==2)
	{
		if (PCB::running->state_==FINISHED && myPCB==PCB::running)
		{
			myPCB->flags_signals.Add_flag(signal);

		}
	}
	UNLOCK
}







