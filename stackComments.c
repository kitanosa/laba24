#include "stack.h"
// описание функций работы со стеком
/*
теория по стэку
Изначально стек пуст. Вершина стека – указатель на первый элемент, никуда не указывает. В случае си она может быть равна NULL.
Push 3
Теперь стек состоит из одного элемента, числа 3. Вершина стека указывает на число 3.
Push 5
Стек состоит из двух элементов, 5 и 3, при этом вершина стека указывает на 5.
Push 7
Стек состоит из трёх элементов, вершина стека указывает на 7.
Pop
Вернёт значение 7, в стеке останется 5 и 3. Вершина будет указывать на следующий элемент – 5.
Pop
Вернёт 5, в стеке останется всего один элемент, 3, на который будет указывать вершина стека.
Pop
Вернёт 3, стек станет пуст.
*/
void stackCreate(Stack *s)
// s -  стэк. А Stack * это тип. Почему у нас стэк в качестве адреса? Так как самой переменной s у нас не в функции. Мы ее не объявляли. А чтобы менять ее не объйявляя по правилу ее нужно менять по адресу
{
	s->_size = 0;
	s->_top = NULL;
}

int stackEmpty(const Stack *s)
// проверяет пустой ли стак
// в верхнем элементе стэка есть указатель на нижний лемент первый. Если сломать указатель на верхний то стек поломается
{
	return s->_top == NULL;
}

int stackSize(const Stack *s)
{
	return s->_size;
}

int stackPush(Stack *s, const STACK_TYPE value)
// добавление элемента в стек
// в качестве аргументов функции у нас выступает адрес стека s и value - данные внутри каждого элемента стека
{
	ItemStack *item = (ItemStack *)malloc(sizeof(ItemStack));

	if (!item)
		// if (!a)можно переписать как if (a==0)
		return 0;
	// записываем значения value введенные с клавиатуры в элемент стека item. value имеет структурный тип включая _varOp и _num
	item->_data = value;
	// у стека есть ссылка на верхний элемент
	// item это элемент что только содлаи и поместили наверх
	// item указывает на бывший топовый элемент то есть на предпоследний
	// стек с указателями сверху вниз
	// что делается в следующих двух строках
	// мы добавили наверх стэка новый элемент item. Нужно сказать, что теперь это топовый элемент и поставить туда указатели
	// s->top это указатель на старый топовый элемент
	// item->_prev это указатель на предыдущий элемент. item это новый элемент который мы положили наверх
	// то есть мы item->_prev = s->_top; мы сказали что указатель на топовый элемент неактуален. И мы поменяем его с указателем на предтоповый элемент
	item->_prev = s->_top;
	// стек теперь указывает на новый топовый элемент item
	s->_top = item;
	// размер стека увеличиваетмя
	s->_size++;

	return 1;
}

int stackPop(Stack *s)
// удаление верзнего элемента
{
	ItemStack *item = NULL;

	if (!s->_size)
		return 0;

	item = s->_top; // присваиваем топовому элементу имя item
	// меняем ссылку с топового элемента ведь у нас удаление идет
	s->_top = s->_top->_prev;
	s->_size--;

	free(item);

	return 1;
}

STACK_TYPE stackTop(const Stack *s)
{
	// произвольные данные верхнего элемета. Мы можем поместить какие то данные внутри каждого элемента. Это как с нодами в деревьях где мы числа вставляли Но тут данные могут быть не только числа но любые. Даже массивы
	return s->_top->_data;
}

void stackDestroy(Stack *s)
{
	ItemStack *item = NULL;

	while (s->_top)
	{
		item = s->_top;
		s->_top = s->_top->_prev;

		free(item);
	}

	s->_size = 0;
	s->_top = NULL;
}