/*
 * event.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */
#include"event.h"
#include"KernelEv.h"

Event::Event(IVTNo ivtNo){
myImpl=new KernelEv(ivtNo);
myImpl->event_=this;

}

void Event::wait(){
	myImpl->wait();
}

Event::~Event(){
	delete myImpl;
}

void Event::signal(){
	myImpl->signal();
}
