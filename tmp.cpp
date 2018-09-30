//
// Created by Shweta Sahu on 4/6/18.
//


#include <iostream>

using namespace std;

void sync(){
    cout <<"inside a\n";
    printf("%p\n", __builtin_extract_return_addr(__builtin_return_address(0)));
    void * addr = __builtin_extract_return_addr(__builtin_return_address(0));
    //goto &addr;
    uint64_t temp = (uint64_t )addr;
    //__asm__ __volatile__("jmpq %0 ":: "g"(temp):"memory");
    __asm__ __volatile__("pushq %0; ret":: "g"(temp):"memory");

    cout<<"sync end\n";
    return;

}
void main(){
    cout<<"start\n";
    sync();
    cout<<"after\n";

}

