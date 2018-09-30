
//
// Created by Shweta Sahu on 4/5/18.
//

#include "ConcurrentDeque.h"
#include <atomic>
#include <iostream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <random>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

int maxCores;
bool ready;
bool terminated;
int MAX_ATTEMPTS;


int** Z;
int** X;
int** Y;


class Task{
public:
    virtual void execute() = 0;
    void spawn(Task *task);
    int sync();
    Task* parentTask;
    atomic<int> childTasksCount=0;
    vector<bool> childCount;
    int count=0;
    int updateParentIndex = -1;
    string id;
};

unordered_map<int,ConcurrentDeque<Task*>*> processQueue;
vector<int> processIds;

int  getRandom(int processId){
    std::random_device rd;
    std::mt19937 rng(rd());
    thread_local std::uniform_int_distribution<int> uid(1,processIds.size()); // random dice
    int x =  uid(rng)-1; // use rng as a generator
    //printf("%d \n",x);
    return x;
//    subtract_with_carry_engine<uint_fast32_t, 9, 10, 24> randGenerator(processId);
//    return (int)(randGenerator()%maxCores);
}

int getAndExecuteNextTask(int max_attempts){
    Task* task = NULL;
    int processId = __cilkrts_get_worker_number();
    if(ready) {
        //cout<<"GETTING new task "<<processId;
        task = processQueue[processId]->pop_front();
        int attempt_count = 1;
        while (task == NULL && !terminated && attempt_count <= max_attempts) {
            //printf("STEALING  task %d attempt%d\n",processId,attempt_count);
            int rand = getRandom(processId);
            int pId = processIds[rand];
            if(pId == processId){
                continue;
            }
            ConcurrentDeque<Task *> *queue = processQueue[pId];

            if (queue != NULL) {
                attempt_count++;
                printf("%d -> %d size of queue %d \n",processId,pId,queue->size());
                task = queue->pop_front();
                printf("%d -> %d size of queue %d --------\n",processId,pId,queue->size());
            }

        }
        if (task != NULL) {
            task->execute();
            //printf("BEFORE DECREE %d ...... %s\n",task->parentTask->childTasksCount,task->id.c_str());
            task->parentTask->childTasksCount--;
            task->parentTask->childCount[task->updateParentIndex]=true;

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


    //CHecking
    childCount.push_back(false);
    task->updateParentIndex = count;
    count++;

    int processId = __cilkrts_get_worker_number();
    ConcurrentDeque<Task*>* queue = processQueue[processId];
    childTasksCount++;
    queue->push_back(task);

}

int Task::sync(){
    bool flag=true;
//    while(flag){
//        flag=false;
//        for(bool var :childCount){
//            if(!var){
//                flag=true;
//            }
//        }
//        if(flag){
//            getAndExecuteNextTask(MAX_ATTEMPTS/100);
//            printf("Value -> %d\n",Z[1022][987]);
//        }

    //printf("SYNC %d ...... %s\n",childTasksCount,id);
//    }
    while(childTasksCount >0){
        //printf("SYNC %d ...... %s\n",childTasksCount,id);
        getAndExecuteNextTask(MAX_ATTEMPTS/100);
    }
    return 0;
}



void start(){
    int processId = __cilkrts_get_worker_number();
    printf("ProcessId :%d\n",processId);
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
    cout<<"inside init \n";
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
    cout<<"mainprocessID "<<processId<<endl;
    processIds.push_back(processId);
    processQueue[processId] = new ConcurrentDeque<Task*>();
    maxCores = processIds.size();
    printf("total...... %d\n",maxCores);
    ready = true;
    time_t start = time(0);
    task->execute();
    terminated = true;
    cilk_sync;
    delete processQueue[processId];
    time_t end = time(0);
    cout<<"time:"<<end-start<<endl;
    cout<<"init completed\n";
}


class testTask: public Task {
public:
    testTask(int Zr,int Zc,
             int Xr,int Xc,
             int Yr,int Yc,int n): n(n),Zr(Zr),Zc(Zc),Xr(Xr),Xc(Xc),Yr(Yr),Yc(Yc) {
        childTasksCount = 0;
        id = to_string(n)+":"+to_string(Zr)+","+to_string(Zc)+":"+to_string(Xr)+","+to_string(Xc)+":"+to_string(Yr)+","+to_string(Yc);
        //cout<<id<<endl;
    }

    void execute() {
        if( n <= 32) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    for (int k = 0; k < n; k++) {
                        Z[Zr + i][Zc + j] += X[Xr + i][Xc + k] * Y[Yr + k][Yc + j];

                    }
                }
            }

            return;
        }

        n =n/2;
        testTask *a = new testTask(Zr, Zc,
                                   Xr, Xc,
                                   Yr, Yc, n);
        testTask *b = new testTask(Zr, Zc+n,
                                   Xr, Xc,
                                   Yr, Yc + n , n);
        testTask *c = new testTask(Zr + n , Zc,
                                   Xr + n , Xc,
                                   Yr, Yc , n);
        testTask *d = new testTask(Zr+n,Zc+n,
                                   Xr+n,Xc,
                                   Yr,Yc+n, n);

        spawn(a);
        spawn(b);
        spawn(c);
        d->execute();
        sync();

        after_sync:
        delete a;
        delete b;
        delete c;
        delete d;


        testTask *a1 = new testTask(Zr, Zc,
                                    Xr, Xc + n,
                                    Yr + n, Yc, n);
        testTask *b1 = new testTask(Zr, Zc + n ,
                                    Xr, Xc + n ,
                                    Yr + n , Yc + n , n);
        testTask *c1 = new testTask(Zr + n , Zc,
                                    Xr + n , Xc + n ,
                                    Yr + n , Yc, n);
        testTask *d1 = new testTask(Zr+n,Zc+n,
                                    Xr+n,Xc+n,
                                    Yr+n,Yc+n, n);

        spawn(a1);
        spawn(b1);
        spawn(c1);
        d1->execute();
        sync();

        delete a1;
        delete b1;
        delete c1;
        delete d1;


        //printf("value of %d = %d\n",n,Z[n-1][n-4]);
        return;
    }

private:
    int n;
    int Zr,Zc,Xr,Xc,Yr,Yc;
};

void fillArray(int** A, int n){
    for(int i =0; i<n ; i++){
        for(int j =0 ; j<n ; j++){
            A[i][j]= 1;
        }
    }
}
void fillZero(int** A, int n){
    for(int i =0; i<n ; i++){
        for(int j =0 ; j<n ; j++){
            A[i][j]= 0;
        }
    }
}

int myAtoi(char *str)
{
    int res = 0; // Initialize result

    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';

    // return result.
    return res;
}
int main(int argc, char *argv[]) {
    int N = myAtoi(argv[1]);
    int P = myAtoi(argv[2]);
    Z = new int*[N];
    X = new int*[N];
    Y = new int*[N];
    for(int i = 0; i <N; i++){
        X[i] = new int[N];
        Y[i] = new int[N];
        Z[i] = new int[N];

    }
    //_count = n*n*n;
    fillArray(X,N);
    fillArray(Y,N);
    fillZero(Z,N);

    testTask *root = new testTask(0,0,0,0,0,0,N);
    schedule(P,root);


    cout << "Matrix(" << N << ") = " << Z[N-1][N-3] << "\n";
    delete X;
    delete Y;
    delete Z;
    return 0;
}