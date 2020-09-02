#ifndef STACK_H
#define STACK_H
// в массиве ты обращаешься к элементам по индексу
// очередь ты обращаешься только к самому нижнему элементу. Ты его достаешь и следующий приходит. Самый первый пришел и самый первый ушел ты можешь пополнять но в конец очереди
// стек стопка тарелок.  Сначала достаешь достаешь верхнюю тарелку

#include <stdlib.h>
// описание структуры стека
// Обычно все члены структуры связаны друг с другом. Например, информация об имени и адресе, находящаяся в списке рассылки, обычно представляется в виде структуры.
typedef struct _Token // токен содержин либо букву либо число соответственно данным ниже
{
    char _varOp;
    double _num;
} Token;

typedef Token STACK_TYPE;

typedef struct _ItemStack
// элемент стека
{
    STACK_TYPE _data;
    // какие то данные которые записываются в каждый элемент стека. Как в дереве мы записываем в узел ноду, так и тут 
    // STACK_TYPE это тип
    struct _ItemStack *_prev;
    // ссылка на предыдущий элемент prev с перевода предыдущий
} ItemStack;

typedef struct _Stack
// сам стек
{
    int _size;
    // size – это количество элементов
    struct _ItemStack *_top;
    // ссылка на верхний элемент стека
} Stack;

void stackCreate(Stack *s);
int stackEmpty(const Stack *s);
int stackSize(const Stack *s);
int stackPush(Stack *s, const STACK_TYPE value);
int stackPop(Stack *s);
STACK_TYPE stackTop(const Stack *s);
void stackDestroy(Stack *s);

#endif
