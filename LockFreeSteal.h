#ifndef PARALLELPROJECT_LOCKFREESTEAL_H
#define PARALLELPROJECT_LOCKFREESTEAL_H


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
unordered_map<int,bool> status;
unordered_map<int,Task *> transfer;
unordered_map<int,int> request;
vector<int> processIds;
Task* NO_RES = (Task*)1;
int NO_REQ = -1;

int  getRandom(){
    std::random_device rd;
    std::mt19937 rng(rd());
    thread_local std::uniform_int_distribution<int> uid(1,processIds.size()); // random dice
    int x =  uid(rng)-1; // use rng as a generator
    //printf("%d \n",x);
    return x;
}
void updateStatus(int processId){
    bool newStatus = (processQueue[processId]->size()>0);
    if(status[processId]!=newStatus) {
        status[processId] = newStatus;
    }
}

void communicate(int processId){
    int req = request[processId];
    if(req == NO_REQ){
        return;
    }
    if(processQueue[processId]->size()==0){
        transfer[req]=NULL;
    }else{
        transfer[req]=processQueue[processId]->pop_front_Lfree();
    }
    request[processId] = NO_REQ;

}

int getAndExecuteNextTask(int max_attempts){
    Task* task = NULL;
    int processId = __cilkrts_get_worker_number();
    if(ready) {
        //cout<<"GETTING new task "<<processId;
        task = processQueue[processId]->pop_back_Lfree();
        int attempt_count = 1;
        while (task == NULL && !terminated && attempt_count <= max_attempts) {
            //printf("STEALING  task %d attempt%d\n",processId,attempt_count);
            attempt_count++;
            transfer[processId] = NO_RES;
            int rand = getRandom();
            int pId = processIds[rand];
            if (pId == processId) {
                continue;
            }
            //int initialRequest = request[pId]
            if (status[pId] && __sync_bool_compare_and_swap(&request[pId],NO_REQ,processId)){
                while(!terminated && transfer[processId]==NO_RES){
                    communicate(processId);
                }
                if(transfer[processId] != NULL && transfer[processId] != NO_RES){
                    task = transfer[processId];
                    //Need to check this
                    //request[processId] = NO_REQ;
                    break;
                }
            }
            communicate(processId);

        }
        communicate(processId);

        if (task != NULL) {
            updateStatus(processId);
            communicate(processId);
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
    if(queue == NULL){
        cout<<"SPAWN ERRRRRORRRRR:"<<processId;
    }else{
        queue->push_back_Lfree(task);
        if(queue->size()>1){
            updateStatus(processId);
        }
        if(request[processId]==-1){
            return;
        }
        communicate(processId);
    }

}

int Task::sync(){
    while(childTasksCount >0){
        //printf("SYNC %d ...... %s\n",childTasksCount,id);
        getAndExecuteNextTask(MAX_ATTEMPTS/100);
    }
    return 0;
}



void start(int id){
    int processId = __cilkrts_get_worker_number();
    processIds[id]=processId;
    //printf("start: %d\n",processId);
    processQueue[processId] = new ConcurrentDeque<Task*>();
    request[processId] = NO_REQ;
    transfer[processId] = NO_RES;
    status[processId] = false;
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
    cout<<"inside LockFreeSteal ---------------\n";
    maxCores = maxCores_;
    MAX_ATTEMPTS = 100*maxCores*log2(maxCores);
    ready = false;
    terminated = false;
    int id = 0;
    processIds.resize(maxCores_);
    while (id < maxCores - 1) {
        cilk_spawn start(id);
        id++;
    }
    int processId = __cilkrts_get_worker_number();
    processIds[maxCores_-1]=processId;
    processQueue[processId] = new ConcurrentDeque<Task*>();
    request[processId] = NO_REQ;
    transfer[processId] = NO_RES;
    status[processId] = false;
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
    cout<<"LockFreeSteal completed--------------\n";
}

#endif //PARALLELPROJECT_LOCKFREESTEAL_H
