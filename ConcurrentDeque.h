//
// Created by Shweta Sahu on 3/13/18.
//

#ifndef HW1_CONCURRENTDEQUE_H
#define HW1_CONCURRENTDEQUE_H

#include <deque>
#include <mutex>
#include <atomic>

//template<class T>
//struct Node {
//    Node<T>* next;
//    Node<T>* prev;
//    T data;
//};
//
//
//template <class T>
//class myDeque{
//    Node<T> *head = NULL;
//    Node<T> *tail = NULL;
//    std::atomic<int> len = 0;
//public:
//    myDeque(){}
//
//    bool empty(){
//        return len==0;
//    }
//
//    T pop_front(){
//        if(empty()){
//            return NULL;
//        }
//        Node<T> *temp = head;
//        head = head->next;
//        if(head!=NULL){
//            head->prev = NULL;
//        }
//        len--;
//        return temp->data;
//    }
//
//    T pop_back(){
//        if(empty()){
//            return NULL;
//        }
//        Node<T> *temp = tail;
//        tail = tail->prev;
//        if(tail!=NULL){
//            tail->next = NULL;
//        }
//        len--;
//        return temp->data;
//
//    }
//
//    T push_back(T &val){
//        Node<T> *temp = new Node<T>();
//        temp->data = val;
//        temp->next = NULL;
//        temp->prev = tail;
//        if(empty()){
//            head = temp;
//            tail = temp;
//        }else{
//            tail->next = temp;
//            tail = temp;
//        }
//        len++;
//    }
//
//    int size(){
//        return len;
//    }
//
//
//};



template <class T>
class ConcurrentDeque{
private:
    std::deque<T> myQueue;
    std::mutex mtx;
    void lock(){
        mtx.lock();
    }
    void unlock(){
        mtx.unlock();
    }

public:
    long work;
    //ConcurrentDeque():mtx(){}
    T pop_front(){
        T val = NULL;
        //Previous was lock() need to check for centralised queue.
        if(mtx.try_lock()) {
            if (!myQueue.empty()) {
                val = myQueue.front();
                myQueue.pop_front();
            }
            unlock();
        }
        return val;
    }

    //Only used by stealing thread
    T pop_back(){
        T val = NULL;
        if(mtx.try_lock()) {
            if (!myQueue.empty()) {
                val = myQueue.back();
                myQueue.pop_back();
            }
            unlock();
        }
        return val;
    }
    void push_back(T val){
        lock();
        myQueue.push_back(val);
        unlock();
    }
    int size(){
        return myQueue.size();
    }

    T peek_front(){
        T val = NULL;
        if (!myQueue.empty()) {
            val = myQueue.front();
        }
        return val;
    }

    void push_back_Lfree(T val){
        myQueue.push_back(val);
    }

    T pop_front_Lfree() {
        T val = NULL;
        if (!myQueue.empty()) {
            val = myQueue.front();
            myQueue.pop_front();
        }
        return val;
    }

    T pop_back_Lfree() {
        T val = NULL;
        if (!myQueue.empty()) {
            val = myQueue.back();
            myQueue.pop_back();
        }
        return val;
    }

};

//template <class T>
//class ConcurrentDeque{
//private:
//    myDeque<T> *myQueue = new myDeque<T>();
//    std::mutex mtx;
//    void lock(){
//        mtx.lock();
//    }
//    void unlock(){
//        mtx.unlock();
//    }
//
//public:
//    long work;
//    //ConcurrentDeque():mtx(){}
//    T pop_front(){
//        T val = NULL;
//        if(mtx.try_lock()) {
//            if (!myQueue->empty()) {
//                //val = myQueue.front();
//                val = myQueue->pop_front();
//            }
//            unlock();
//        }
//        return val;
//    }
//    T pop_back(){
//        T val = NULL;
//        lock();
//        if(!myQueue->empty()) {
//            //myQueue.back();
//            val = myQueue->pop_back();
//        }
//        unlock();
//        return val;
//    }
//    void push_back(T val){
//        lock();
//        myQueue->push_back(val);
//        unlock();
//    }
//    int size(){
//        return myQueue->size();
//    }
//};

#endif //HW1_CONCURRENTDEQUE_H
