#include "Task.h"
#include <iostream>

class testTask: public Task {
public:
    testTask(int n_, long *result_): n(n_), result(result_) {
        childTasksCount = 0;
    }

    void execute() {
        if (n <= 2) {
            *result = 1;
            return;
        }

        long x, y;

        testTask *a = new testTask(n-1, &x);
        testTask *b = new testTask(n-2, &y);
        spawn(a);
        b->execute();

        sync();

        delete a;
        delete b;

        *result = x + y;
        printf("value of %d = %d",n,*result);
        return;
    }

private:
    int n;
    long *result;
};

//int main(int argc, char *argv[]) {
//    int n = 15;
//    long result;
//
//    testTask *root = new testTask(n, &result);
//    initScheduler(64,root);
//
//
//    cout << "fib(" << n << ") = " << result << "\n";
//
//    return 0;
//}