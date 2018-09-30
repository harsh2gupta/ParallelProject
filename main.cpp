#include "LockFreeSteal.h"
int** Z;
int** X;
int** Y;

using namespace std;
class testTask: public Task {
public:
    testTask(int Zr,int Zc,
             int Xr,int Xc,
             int Yr,int Yc,int n): n(n),Zr(Zr),Zc(Zc),Xr(Xr),Xc(Xc),Yr(Yr),Yc(Yc) {
        childTasksCount = 0;
        //id = to_string(n)+":"+to_string(Zr)+","+to_string(Zc)+":"+to_string(Xr)+","+to_string(Xc)+":"+to_string(Yr)+","+to_string(Yc);
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
    int N = myAtoi(argv[2]);
    int cores = myAtoi(argv[1]);
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
    schedule(cores,root);


    cout << "Matrix(" << N << ") = " << Z[N-1][N-3] << "\n";
    delete X;
    delete Y;
    delete Z;
    return 0;
}