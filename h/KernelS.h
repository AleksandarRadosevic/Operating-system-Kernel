/*
 * KernelSem.h
 *
 *  Created on: May 10, 2020
 *      Author: OS1
 */

#ifndef KERNELS_H_
#define KERNELS_H_

#include"semaphor.h"
#include "pcb.h"

class KernelSem{
public:
KernelSem(int init=1,Semaphore *);
 virtual int wait(Time maxTimeToWait);
 virtual int signal(int n=0);
 int val () const {return value;}
virtual ~KernelSem();
static LIST_SEMAPHORE semaphores;

int value;
List list_val;
List list_wait;
Semaphore *mySem;
};

#endif /* KERNELS_H_ */
