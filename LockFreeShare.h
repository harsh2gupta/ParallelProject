#ifndef PARALLELPROJECT_LOCKFREESHARE_H
#define PARALLELPROJECT_LOCKFREESHARE_H


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
unordered_map<int,Task *> received;
vector<int> processIds;
Task* DUMMY = (Task*)1;

int  getRandom(){
    std::random_device rd;
    std::mt19937 rng(rd());
    thread_local std::uniform_int_distribution<int> uid(1,processIds.size()); // random dice
    int x =  uid(rng)-1; // use rng as a generator
    //printf("%d \n",x);
    return x;
}
//void updateStatus(int processId){
//    bool newStatus = (processQueue[processId]->size()>0);
//    if(status[processId]!=newStatus) {
//        status[processId] = newStatus;
//    }
//}

void communicate(int processId){

    if (processQueue[processId]->size()==0){
        return;
    }
    int rand = getRandom();
    int pId = processIds[rand];
    while(pId == processId){
        rand = getRandom();
        pId = processIds[rand];
    }
    if(received[pId] != NULL && received[pId]!= DUMMY){
        return;
    }
    Task* temp = processQueue[processId]->peek_front();
    if(__sync_bool_compare_and_swap(&received[pId],DUMMY,temp)){
        processQueue[processId]->pop_front_Lfree();
    }

//    if (processQueue[processId]->size()==0){
//        return;
//    }
//    int rand = getRandom();
//    int pId = processIds[rand];
//    int attempts = maxCores*log2(maxCores);
//    while(true){
//        attempts--;
//        rand = getRandom();
//        pId = processIds[rand];
//        if(pId == processId || received[pId]!= DUMMY){
//            continue;
//        }
//        Task* temp = processQueue[processId]->peek_front();
//        if(__sync_bool_compare_and_swap(&received[pId],DUMMY,temp)){
//            processQueue[processId]->pop_front_Lfree();
//            break;
//        }
//        if(attempts==0){
//            break;
//        }
//    }



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
            if(received[processId]==DUMMY){
                continue;
            }else{
                task = received[processId];
                received[processId] = DUMMY;
            }

        }
        if (task != NULL) {
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
    //printf("start: %d\n",processId);
    processQueue[processId] = new ConcurrentDeque<Task*>();
    received[processId] = DUMMY;
    Task * task = NULL;
    int status =1;
    while(!terminated){ //&& status==1){//|| centralizedTaskQueue->size()>0){
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
    cout<<"inside LockFreeShare ---------------\n";
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
    //cout<<"mainprocessID "<<processId<<endl;
    processIds[maxCores_-1]=processId;
    processQueue[processId] = new ConcurrentDeque<Task*>();
    received[processId] = DUMMY;
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
    cout<<"LockFreeShare completed--------------\n";
}


#endif //PARALLELPROJECT_LOCKFREESHARE_H
