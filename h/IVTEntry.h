/*
 * IVTEntry.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include"KernelEv.h"


class IVTEntry{
public:
	IVTEntry(int numEntry,pInterrupt p);
	static IVTEntry* table[256];
	KernelEv *kernel_event;
	pInterrupt old_routine;
	pInterrupt interruptRoutine;
	void signal();
	void call_old(){old_routine();}
	virtual ~IVTEntry();
	int numEntry;
};



#define PREPAREENTRY(number_entry,flag)\
void interrupt inter##number_entry(...);\
IVTEntry newEntry##number_entry(number_entry,inter##number_entry);\
void interrupt inter##number_entry(...){\
newEntry##number_entry.signal();\
if (flag == 1)\
newEntry##number_entry.call_old();\
}

#endif /* IVTENTRY_H_ */
