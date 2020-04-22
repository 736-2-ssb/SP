#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "time.h"

void inicializeMatrix(uint8_t* matrix, int size)
{
    srand(time(NULL));
    uint8_t row, col;
    for (row = 0; row < size; row++)
    {
        for (col = 0; col < size; col++)
        {
            matrix[row * size + col] = rand()%10;
        }
    }
}

void printMatrix(uint8_t* matrix, int size)
{
    uint8_t row, col;
    for (row = 0; row < size; row++)
    {
        for (col = 0; col < size; col++)
        {
            printf("%d ", matrix[row * size + col]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int size;
    printf("Enter ammount of elevents in array:\n");
    scanf("%d", &size);
    uint8_t* source_matrix = (uint8_t*)malloc(sizeof(uint8_t)*size*size);
    uint8_t* T_matrix = (uint8_t*)malloc(sizeof(uint8_t)*size*size);
    inicializeMatrix(source_matrix, size);
    printMatrix(source_matrix, size);
    asm volatile(
        "mov %%rdi, %%rcx\n\t"
        "mov %%rdi, %%rax\n\t"
        "sub:\n\t"
        "push %%rbx\n\t"
        "mov (%%rbx), %%rbx\n\t"
        "mov %%rbx, (%%rdx)\n\t"
        "pop %%rbx\n\t"
        "inc %%rdx\n\t"
        "add %%rdi, %%rbx\n\t"
        "loop sub\n\t"
        "mov %%rdi, %%rcx\n\t"
        "restore:\n\t"
        "sub %%rdi, %%rbx\n\t"
        "dec %%rdx\n\t"
        "loop restore\n\t"
        "inc %%rbx\n\t"
        "add %%rdi, %%rdx\n\t"
        "mov %%rdi, %%rcx\n\t"
        "dec %%rax\n\t"
        "cmp $0, %%rax\n\t"
        "jne sub\n\t"
        :
        :"b"(source_matrix), "d"(T_matrix), "D"(size)
        :
    );
    printMatrix(T_matrix, size);
    return 0;
}