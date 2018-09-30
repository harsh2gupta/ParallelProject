//
// Created by Shweta Sahu on 4/5/18.
//

#ifndef PROJECT_TASK_H
#define PROJECT_TASK_H

#include "ConcurrentDeque.h"
#include <atomic>
#include <iostream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

using namespace std;

int maxCores;
bool ready;
bool terminated;

class Task{
public:
    virtual void execute() = 0;
    void spawn(Task *task);
    int sync();
    Task* parentTask;
    atomic<int> childTasksCount;

};

ConcurrentDeque <Task*> *centralizedTaskQueue;

 void getAndExecuteNextTask(){
     Task* task = NULL;
     while(ready && task == NULL && centralizedTaskQueue->size()>0){
         task = centralizedTaskQueue->pop_front();
     }
     if(task != NULL){
         task->execute();
         task->parentTask->childTasksCount--;
     }
 }
void Task::spawn(Task *task) {
    task->parentTask = this;
    this->childTasksCount++;
    centralizedTaskQueue->push_back(task);
}

int Task::sync(){
    while(this->childTasksCount >0){
        getAndExecuteNextTask();
    }
    return 0;
}



void start(){
    Task * task = NULL;
    cout<<"Thread created\n";
    while(!terminated ){//|| centralizedTaskQueue->size()>0){
        getAndExecuteNextTask();
    }
}

void initScheduler(int maxCores_,Task *task) {
    cout<<"inside init \n";
    maxCores = maxCores_;
    ready = false;
    terminated = false;
    centralizedTaskQueue = new ConcurrentDeque<Task *>();

    int id = 0;
    while (id < maxCores - 1) {
        cilk_spawn start();
        id++;
    }
    ready = true;
    time_t start = time(0);
    task->execute();
    terminated = true;
    cilk_sync;
    time_t end = time(0);
    cout<<"time:"<<end-start<<endl;
    cout<<"init completed\n";
}




#endif //PROJECT_TASK_H
