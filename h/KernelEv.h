/*
 * KernelEv.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_
#include"event.h"
#include"pcb.h"

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
Event *event_;
PCB *myPCB;
void wait();
void signal();
int value;
virtual ~KernelEv();

};

typedef void interrupt (*pInterrupt)(...);


#endif /* KERNELEV_H_ */
