//
// Created by Shweta Sahu on 4/19/18.
//

#ifndef PROJECT_CSCHEDULER_H
#define PROJECT_CSCHEDULER_H
#include "ConcurrentDeque.h"
#include <atomic>
#include <iostream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <ctime>
#include <chrono>

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
    string id;
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
    childTasksCount++;
    centralizedTaskQueue->push_back(task);
}

int Task::sync(){
    while(childTasksCount >0){
        getAndExecuteNextTask();
    }
    return 0;
}



void start(){
    Task * task = NULL;
    while(!terminated ){//|| centralizedTaskQueue->size()>0){
        getAndExecuteNextTask();
    }
}

void schedule(int maxCores_,Task *task) {
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    cout<<"inside Centralised ---------------\n";
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
    auto start = Time::now();
    task->execute();
    terminated = true;
    cilk_sync;
    auto end = Time::now();
    auto d = end - start;
    auto diff = std::chrono::duration_cast<ms>(d);
    cout<<"time:"<<diff.count()<<endl;
    cout<<"Centralised completed--------------\n";
}


#endif //PROJECT_CSCHEDULER_H
