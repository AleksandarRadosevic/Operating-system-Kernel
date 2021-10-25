/*
 * semaphor.cpp
 *
 *  Created on: May 10, 2020
 *      Author: OS1
 */
#include"semaphor.h"
#include"KernelS.h"




Semaphore::Semaphore(int init)
{
	myImpl=new KernelSem(init,this);

}


int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}


int Semaphore::signal(int n){
	return myImpl->signal(n);
}




int Semaphore::val()const{
	return myImpl->val();
}

Semaphore::~Semaphore(){
	delete myImpl;

}
