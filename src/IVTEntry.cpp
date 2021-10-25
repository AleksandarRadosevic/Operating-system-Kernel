/*
 * IVTEntry.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#include"IVTEntry.h"
#include<DOS.H>


IVTEntry* IVTEntry::table[256]={0};


IVTEntry::IVTEntry(int numEntry,pInterrupt p){

#ifndef BCC_BLOCK_IGNORE
	interruptRoutine=p;
	old_routine=getvect(numEntry);
	setvect(numEntry,interruptRoutine);
#endif
	table[numEntry]=this;

this->numEntry=numEntry;



}

void IVTEntry::signal()
{
	kernel_event->signal();
}


IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	setvect(numEntry,old_routine);
	table[numEntry]=0;
	#endif
}
