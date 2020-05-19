#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <fstream>

const char PATH[] = "results.txt";

pthread_mutex_t file_mutex;//объявляем мьютекс
typedef struct args_d//структура для передачи аргументов в поток
{
    int* array;//указатель на первый элемент блока
    int ammount;//количество элементов в блоке
    int number;//номер блока (для записи в файл)
    float* M;//адрес в массиве М(х) для i-ого блока
    float* D;//адрес в массиве D(х) для i-ого блока
}args_d;

float M_funk(int* array, int size)
{
    float M = 0;
    for (int i = 0; i < size; i++)
    {
        M += array[i];
    }
    M /= size;
    return M;
}

float D_funk(int* array, int size)
{
    float D = 0;
    float M2x = M_funk(array, size) * M_funk(array, size);
    float Mx2;
    for (int i = 0; i < size; i++)
    {
        array[i] = array[i]*array[i];
    }
    Mx2 = M_funk(array, size);
    D = Mx2 - M2x;
    return D;
}

void* calcStats (void* arguments)
{
    args_d* args = (args_d*)arguments;
    *(args->M) = M_funk(args->array, args->ammount);
    *(args->D) = D_funk(args->array, args->ammount);

    pthread_mutex_lock(&file_mutex);//блокируем мьютекс перед записью в файл
    std::ofstream file;
    file.open(PATH, std::ios::app);
    file << args->number << ": M(x) = " << *(args->M) << ", D(x) = " << *(args->D) << "\n";
    file.close();
    pthread_mutex_unlock(&file_mutex);//освобождаем мьютекс после завершения записи
    return NULL;
}

int main()
{
    pthread_mutex_init (&file_mutex, NULL);//инициализируем мьютекс
    unsigned int common, ammount, length = 10;
    pid_t pid = getpid();
    int status;

    //чистим файл
    std::ofstream file (PATH);
    file.clear();
    file.close();
    
    std::cout << "Enter common ammount of numbers\n";
    std::cin >> common;
    int* array = (int*)malloc(sizeof(int)*common);
    srand(time(NULL));
    std::cout << "Generated array:\n";
    for (int i = 0; i < common; i++)
    {
        array[i] = rand()%10;
        std::cout << array[i] << " ";
    }
    std::cout << "\n";

    ammount = common / length;
    if ((common % length) != 0)
        ammount++;
    
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t)*ammount);
    float* M_arr = (float*)malloc(sizeof(float)*ammount);//массив для мат.ожиданий
    float* D_arr = (float*)malloc(sizeof(float)*ammount);//массив для дисперсий
    for (unsigned int i = 0; i < ammount; i++)
    {
        unsigned int first = i * length;
        unsigned int second = i * length + length;
        if (second > common)
            second = common;
        unsigned int size = (second - first);


        args_d* arguments = (args_d*)malloc(sizeof(args_d));//создаем структуру для передачи аргументов
        arguments->array = &array[first];
        arguments->ammount = size;
        arguments->number = i + 1;
        arguments->M = &M_arr[i];
        arguments->D = &D_arr[i];

        std::cout << "\nPARENT:\n" << "Trying to start child thread " << i+1 << "\n";
        status = pthread_create(&tid[i], NULL, calcStats, (void*)arguments);//создаем потоки
        switch (status)
        {
        case 0:
            std::cout << "PID: " << pid << "\n" << "TID: " << tid[i] << "\n";
            break;
        case -1:
            perror("pthread_create");
            exit(1);
        default:
        free(arguments);//удаляем структуру аргументов
            break;
        }
		
    }
    for (unsigned int i = 0; i < ammount; i++)
    {
        pthread_join( tid[ i ], NULL);//ждем завершения всех потоков
    }

    for (unsigned int i = 0; i < ammount; i++)
    {
        std::cout << "M" << i+1 << " = " << M_arr[i] << "\n";
        std::cout << "D" << i+1 << " = " << D_arr[i] << "\n\n";
    }

    std::cout << "Nothing left. Exit\n";
    pthread_mutex_destroy(&file_mutex);//уничтожаем мьютекс
    free(array);//чистим память
    free(M_arr);
    free(D_arr);
    free(tid);
    return 0;
}