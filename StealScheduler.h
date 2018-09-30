//
// Created by Shweta Sahu on 4/5/18.
//

#ifndef STEAL_SCHEDULER_H
#define STEAL_SCHEDULER_H

#include "ConcurrentDeque.h"
#include <atomic>
#include <iostream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <random>
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>

using namespace std;

int maxCores;
bool ready;
bool terminated;
int MAX_ATTEMPTS;


class Task{
public:
    virtual void execute() = 0;
    void spawn(Task *task);
    int sync();
    Task* parentTask;
    atomic<int> childTasksCount=0;
    string id;
};

unordered_map<int,ConcurrentDeque<Task*>*> processQueue;
vector<int> processIds;

int  getRandom(){
    std::random_device rd;
    std::mt19937 rng(rd());
    thread_local std::uniform_int_distribution<int> uid(1,processIds.size()); // random dice
    int x =  uid(rng)-1; // use rng as a generator
    //printf("%d \n",x);
    return x;
}

int getAndExecuteNextTask(int max_attempts){
    Task* task = NULL;
    int processId = __cilkrts_get_worker_number();
    if(ready) {
        //cout<<"GETTING new task "<<processId;
        task = processQueue[processId]->pop_back();
        int attempt_count = 1;
        while (task == NULL && !terminated && attempt_count <= max_attempts) {
            //printf("STEALING  task %d attempt%d\n",processId,attempt_count);
            attempt_count++;
            int rand = getRandom();
            int pId = processIds[rand];
            if(pId == processId){
                continue;
            }
            ConcurrentDeque<Task *> *queue = processQueue[pId];

            if (queue != NULL) {

                //printf("%d -> %d size of queue %d \n",processId,pId,queue->size());
                task = queue->pop_front();
                //printf("%d -> %d size of queue %d --------\n",processId,pId,queue->size());
            }

        }

        if (task != NULL) {
            task->execute();
            //printf("BEFORE DECREE %d ...... %s\n",task->parentTask->childTasksCount,task->id.c_str());
            task->parentTask->childTasksCount--;

        }
        if (attempt_count > max_attempts) {
            //cout<<"ATTEMPT OUT ";
            return -1;
        }
    }
    return 1;
}

void Task::spawn(Task *task) {
    task->parentTask = this;
    childTasksCount++;
    int processId = __cilkrts_get_worker_number();
    ConcurrentDeque<Task*>* queue = processQueue[processId];
    queue->push_back(task);
}

int Task::sync(){
    while(childTasksCount >0){
        //printf("SYNC %d ...... %s\n",childTasksCount,id);
        getAndExecuteNextTask(MAX_ATTEMPTS/100);
    }
    return 0;
}



void start(){
    int processId = __cilkrts_get_worker_number();
    processIds.push_back(processId);
    //printf("start: %d\n",processId);
    processQueue[processId] = new ConcurrentDeque<Task*>();
    Task * task = NULL;
    int status =1;
    while(!terminated && status==1){//|| centralizedTaskQueue->size()>0){
        status = getAndExecuteNextTask(MAX_ATTEMPTS);
    }
    //printf(" %d status=%d\n",processId,status);
    delete processQueue[processId];
    processQueue[processId] = NULL;
    //processIds.erase(std::remove(processIds.begin(), processIds.end(), processId), processIds.end());
}

void schedule(int maxCores_,Task *task) {
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    cout<<"inside StealScheduler ---------------\n";
    maxCores = maxCores_;
    MAX_ATTEMPTS = 100*maxCores*log2(maxCores);
    ready = false;
    terminated = false;
    int id = 0;
    while (id < maxCores - 1) {
        cilk_spawn start();
        id++;
    }
    int processId = __cilkrts_get_worker_number();
    processIds.push_back(processId);
    processQueue[processId] = new ConcurrentDeque<Task*>();
    maxCores = processIds.size();
    //printf("total...... %d\n",maxCores);
    ready = true;
    auto start = Time::now();
    task->execute();
    terminated = true;
    cilk_sync;
    delete processQueue[processId];
    auto end = Time::now();
    auto d = end - start;
    auto diff = std::chrono::duration_cast<ms>(d);
    cout<<"time:"<<diff.count()<<endl;
    cout<<"StealScheduler completed--------------\n";
}



#endif //STEAL_SCHEDULER_H
