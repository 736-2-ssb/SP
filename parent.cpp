#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

using namespace std;

int main()
{

    unsigned int common, ammount, length = 10;
    pid_t pid;
    int rv;
    cout << "Enter ammount of numbers\n";
    cin >> common;
    cout << "Generated array:\n";
    int* array = (int*)malloc(sizeof(int)*common);
    srand(time(NULL));
    for (int i = 0; i < common; i++)
    {
        array[i] = rand()%10;
        cout << array[i] << " ";
    }
    cout << "\n";

    ammount = common / length;
    if ((common % length) != 0)
        ammount++;
    
    for (unsigned int i = 0; i < ammount; i++)
    {
        unsigned int first = i * length;
        unsigned int second = i * length + length;
        if (second > common)
            second = common;
        unsigned int size = (second - first);
        char** argv = (char**)malloc(sizeof(char*)*(size+2));
        argv[0] = "children";
        char buf [3];
        for ( int j = 1; j < size+1; j++)
        {
            argv[j] = (char*)malloc(sizeof(char*));
            sprintf(argv[j], "%d", array[(first + j - 1)]);
        }
        argv[size+1] = NULL;
        cout << "\nParent:\n" << "Trying to start child number " << i+1 << "\n";
        switch (pid = fork())
		{
			case -1:
				perror("fork");
				exit(1);
			case 0:
				cout << "Success!\n\n";
				execv("children", argv);
                free(argv);
				exit(rv);
			default:
				wait(&rv);
				cout << "Parent:\n";
				cout << "Child number " << i+1 << " terminated wth status " << rv << "\n";
		}
    }
    free(array);
    cout << "Nothing left. Exit\n";
    return 0;
}