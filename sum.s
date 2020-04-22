.section .data
    array:
        .word 1, 19, 16, 18, -15, 17, 14, 13, -12, 11
    pos_sum_str:
        .ascii "                         \n"
    neg_sum_str:
        .ascii "-                        \n"
    first_str:
        .ascii "...0....\n"
    second_str:
        .ascii "...1....\n"

.section .text
.global _start
_start:
    mov $0, %esp
    mov $array, %esi
    mov $10, %cx
    xor %eax, %eax
    xor %edi, %edi

    #Ищем положительное число
    find_pos:
        mov (%esi), %ebp
        test $0b1000000000000000, %bp #смотрим знаковый бит
        jz get_pos_sum #если там ноль, то число положительное
        jnz get_neg_sum #если не ноль - отрицательное

    #Добавляем число в сумму положительных
    get_pos_sum:
        add (%esi), %eax
        jmp end

    #Переводим в прямой код и добавляем в сумму отрицательных
    get_neg_sum:
        mov (%esi), %ebx
        xor $0xffff, %bx
        inc %ebx
        add %bx, %di

    end:
    add $2, %esi
    loop find_pos

    mov $pos_sum_str, %esi #в буферный регистр для вывода
    mov $pos_sum_str, %ebp #в регистр для конвертирования суммы в строку
    add $20, %ebp #смещаемся в конец строки для конвертирования
    
    #Конвертируем полученную сумму в строку
    mov $10, %cx
    convert:
        mov $10, %ebx
        xor %edx, %edx
        div %bx
        add $48, %dl
        mov %dl, (%ebp)
        sub $1, %ebp
        cmp $0, %ax
        je print
    loop convert

    #Готовим отрицательную сумму к выводу
    negative:
    mov $1, %esp #ставим флаг, что отрицательная сумма выведена
    mov $neg_sum_str, %esi #в буферный регистр для вывода
    xor %eax, %eax
    mov $10, %cx
    mov %di, %ax
    mov $neg_sum_str, %ebp #в регистр для конвертирования суммы в строку
    add $20, %ebp #смещаемся в конец строки для конвертирования
    jmp convert

    print:
        mov $4, %eax
        mov $1, %ebx
        mov %esi, %ecx
        mov $26, %edx
        int $0x80
        test $1, %esp #если сумма отрицательных еще не была выведена
        je negative #то готовим ее к выводу

    mov %di, %ax #сумму отрицательных в ах
    #Переводим сумму в доп код
    xor $0xffff, %ax
    inc %ax

    test $0b00010000, %ax #получаем значение 5 бита и выбираем нужную строку
    #cmp $0, %ax
    je print_first
    jmp print_second

    #Помещаем адрес соответствующей строки в есх для вывода
    print_first:
        mov $first_str, %ecx
        jmp print_bits

    print_second:
        mov $second_str, %ecx
        jmp print_bits

    #Выводим строку с битами
    print_bits:
        mov $4, %eax
        mov $1, %ebx
        mov $9, %edx
        int $0x80
        jmp exit

    
    exit:
        mov $1, %eax
        mov $0, %ebx

        int $0x80
