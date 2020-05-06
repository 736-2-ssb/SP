#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

using namespace std;

int to_int(string str)
{
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
    return res;
}

float M_func(int* array, int size)
{
    float M = 0;
    for (int i = 0; i < size; i++)
    {
        M += array[i];
    }
    M /= size;
    return M;
}

float D_func(int* array, int size)
{
    float D = 0;
    float M2x = M_func(array, size) * M_func(array, size);
    float Mx2;
    for (int i = 0; i < size; i++)
    {
        array[i] = array[i]*array[i];
    }
    Mx2 = M_func(array, size);
    D = Mx2 - M2x;
    return D;
}

int main (int argc, char* argv[])
{
    float M, D;
    double start;
    pid_t pid = getpid(), parent_pid = getppid();
    int size = argc - 1;
    int* array = (int*)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        array[i] = to_int(argv[i+1]);
    }
    cout << "Child:\n";
	cout << "My pid: " << pid << "\n";
	cout << "My parent's pid: " << parent_pid << "\n";
	cout << "My numers: ";
    for (int i = 0; i < size; i++)
    {
        cout << array[i] << " ";
    }
    M = M_func(array, size);
    D = D_func(array, size);
    cout << "\n    M(x) = " << M;
    cout << "\n    D(x) = " << D << "\n";
    free(array);
    printf("Process will be terminated in 3 seconds\n\n");
    start = clock();
    while(1)
    {
        if (((clock() - start) / CLOCKS_PER_SEC) > 3)
            break;
    }
    return 0;
}