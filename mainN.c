// gcc main.c stack.c -o laba24 -lm && ./laba24
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack.h"

typedef struct _Node
{
	char _varOp;	// _var0p это символ
	double _num;	// _num это число 
	struct _Node *_left;
	struct _Node *_right;
} Node;


Node *treeNodeCreate(void);
Node *treeCopy(Node **node);
Node *Zamena1(Node **node);
int CheckLeftNode(Node **node);
void treeBuild(Node **node, Stack *st);
void treeDestroy(Node **node);
void PKL(Node **node, const int level);
void LKP(Node **node);

int isLetter(const char ch);
int isNumber(const char ch);
int isOp(const char ch);
int isOpHigh(const char op1, const char op2);
void postOrder(const char *str, Stack *st);

Node *treeNodeCreate(void){
	Node *tmpNode = malloc(sizeof(Node)); // ВНИМАНИЕ

	tmpNode->_varOp = '\0';//‘\0’ (нуль-символ), чтобы программе было возможно определить конец строки.
	tmpNode->_num = 0.0;
	tmpNode->_left = NULL;
	tmpNode->_right = NULL;

	return tmpNode;
}

Node *treeCopy(Node **node){ // root2 = treeCopy(&root) копирование дерева для вывода измененного дерева
	Node *tmpNode = NULL;

	if (*node == NULL)
		return NULL;

	tmpNode = treeNodeCreate();
	tmpNode->_varOp = (*node)->_varOp;
	tmpNode->_num = (*node)->_num;
	tmpNode->_left = treeCopy(&((*node)->_left)); 
	tmpNode->_right = treeCopy(&((*node)->_right));
	return tmpNode; // возвращает корень дерева скопированного
}

int CheckLeftNode(Node **node){// проверяет пустой ли левый потомок
	if (*node == NULL)	// ВНИМАНИЕ
		return 0;

	if ((*node)->_left == NULL || (*node)->_right == NULL)
		return 0;

	return ((*node)->_left->_varOp == '\0' && (*node)->_left->_num == 0.0);
}

void treeBuild(Node **node, Stack *st){// построение дерева 
	Token token;

	if (stackEmpty(st)) // если стек st пустой 
		return;

	token = stackTop(st); // token это верхний элемент стека

	stackDelTop(st);// удаление верхнего элемента стека st

	(*node) = treeNodeCreate(); // создание одной ноды
	(*node)->_varOp = token._varOp; 
	(*node)->_num = token._num; 

	if (isOp((*node)->_varOp)){ // если в _varOp у нас операция была, то мы создаем справа и слева ноды и заполняем их из стэка st
		treeBuild(&(*node)->_right, st);
		treeBuild(&(*node)->_left, st); 
	} 
}

void treeDestroy(Node **node){
	if (*node == NULL)
		return;

	if ((*node)->_left != NULL)
		treeDestroy(&(*node)->_left);

	if ((*node)->_right != NULL)
		treeDestroy(&(*node)->_right);

	free(*node); // ВНИМАНИЕ. СТРОКА СНИЗУ
}

void PKL(Node **node, const int level){ // печать исходного дерева
	if (*node == NULL)
		return;

	if ((*node)->_right != NULL)
		PKL(&(*node)->_right, level + 1);

	if ((*node)->_varOp != '\0')
		printf("%*s%c\n", level * 4, "", (*node)->_varOp);
	else
		printf("%*s%f\n", level * 4, "", (*node)->_num);

	if ((*node)->_left != NULL)
		PKL(&(*node)->_left, level + 1);
}

void LKP(Node **node){ // печать преобразованного выражения. Из уже преобразованного дерева где корень у нас root2 LKP(&root2);
	if (*node == NULL) 
		return;

	if ((*node)->_left != NULL && !CheckLeftNode(node)){

		if ((*node)->_left->_left != NULL)
			printf("(");

		
		LKP(&(*node)->_left);
		if ((*node)->_left->_left != NULL) // если мы поднялись на два уровня выше
			printf(")");
	}

	if ((*node)->_varOp != '\0') // спустились в самый левый низ. Пишем букавку
		printf("%c", (*node)->_varOp);
	else 
		printf("%f", (*node)->_num);

	if ((*node)->_right != NULL){
		if ((*node)->_right->_left != NULL)
			printf("(");

		LKP(&(*node)->_right);// ищем левый лист
		
		if ((*node)->_right->_left != NULL)
			printf(")");
	}

}
int isLetter(const char ch){
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

int isNumber(const char ch){
	return (ch >= '0' && ch <= '9');
}

int isOp(const char ch){
	return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')');
}

int opPrior(const char op){
	if (op == '^')
		return 4;

	if (op == '*' || op == '/')
		return 3;

	if (op == '+' || op == '-')
		return 2;

	return 1;
}
int isOpHigh(const char op1, const char op2){
	if (op1 == '(' || op2 == '(' || op2 == ')')
		return 0;

	if (op1 == op2 && op2 == '^')
		return 0;
	
	return (opPrior(op1) >= opPrior(op2));
}

void postOrder(const char *str, Stack *st){// postOrder(expr, &stPost);
    
	int i = 0, isBracket = 0;	// isBracket это скобка; is dot точка; step степень; i данный читаемый элемент

	Token tk; 
	Stack stOp; // у нас два стека. st и st0p. st это стек вывода уже. А st0p он собирает элементы и сравнивает

	stackCreate(&stOp);

	tk._varOp = '\0'; 
	tk._num = 0.0;

	while (str[i] != '\0'){	// ВНИМАНИЕ  isDot ваще бесполезна КАК И if ниже
		if (isLetter(str[i])){ // если условие выполнено
			tk._varOp = str[i];
			stackPush(st, tk); 	// добавление элемента tk в стэк st
		}
		else if (isNumber(str[i])){
			tk._varOp = '\0'; // очищаем если вдруг там шо было ВНИМАНИЕ ниже if не нужен
			tk._num = tk._num * 10.0 + str[i] - '0'; // str[i] - '0'преобразует символ в позиции i в числовую цифру. ВНИМАНИЕ бредовую дичь с tk.num можн удалить

			if (!isNumber(str[i + 1])){ // ВНИМАНИЕ
				stackPush(st, tk);
				tk._num = 0.0; // ВНИМАНИЕ МОЖНО УДАЛИТЬ БРЕД СНИЗУ ДВЕ СТРОЧКИ
			}
		}
		else if (isOp(str[i])){
			tk._varOp = str[i];

			if (str[i] == ')')
				isBracket = 1; 

			while (!stackEmpty(&stOp) && (isOpHigh(stackTop(&stOp)._varOp, str[i]) || isBracket)){// пока( стек не пустой && ( по приоритету элемент в стэке stOp больше элемента что считываем || b != 0) )
				if (stackTop(&stOp)._varOp == '(') // на случай если ты с скобки вводишь выражение
					isBracket = 0;
				else
					stackPush(st, stackTop(&stOp));// добавление в вывод элемента с приоритетом большим чем в элемента строчке

				stackDelTop(&stOp); // удаление верхнего элемента
			}

			if (str[i] != ')') 
				stackPush(&stOp, tk);
		}

		i++;
	}

	while (!stackEmpty(&stOp)){ // если строчка закончилась но элементы в стеке st0p еще остались то выводим их
		stackPush(st, stackTop(&stOp));
		stackDelTop(&stOp);
	}

	stackDestroy(&stOp);
}
Node *Zamena1(Node **node){
	if ((*node)!= NULL){
		
		
		if (((*node)->_left==NULL)  && ((*node)->_right==NULL) && ((*node)->_varOp =='a')){
			(*node)->_varOp ='+'; // так как у нас замена a = i + 4
			
			Node *tmpNode = NULL;
			tmpNode = treeNodeCreate();// пустой узел дерева
			tmpNode->_varOp = 'i';
			(*node)->_left=tmpNode;
			
			Node *tmpNode1 = NULL;
			tmpNode1 = treeNodeCreate();
			tmpNode1->_num = 4;
			(*node)->_right=tmpNode1;
		}
		else{ // зочем? Неясно. Наверное на случай если вдруг справа и слева не нул
			 Zamena1(&(*node)->_left);
			 Zamena1(&(*node)->_right);
		}
	}
}


int main(void)
{
	int action;
	char expr[255];
	Node *root = NULL, *root2 = NULL;
	Stack stPost;
	while (1)
	{
		printf("Меню:\n");
		printf("1) Ввести выражение\n");
		printf("2) Печать исходного выражения\n");
		printf("3) Печать преобразованного выражения\n");
		printf("4) Печать исходного дерева\n");
		printf("5) Печать преобразованного дерева\n");
		printf("6) Выход\n");
		printf("Выберите действие: ");
		scanf("%d", &action);

		switch (action)
		{
			case 1:
			{
				printf("Введите выражение: ");
				scanf("%s", expr);
				treeDestroy(&root); // для введения нового выражения нужно сначала удалить старое
				treeDestroy(&root2);
				stackCreate(&stPost);
				postOrder(expr, &stPost);
				treeBuild(&root, &stPost); 
				stackDestroy(&stPost);  // удаляем  пустой стек чтобы не занимал место. Пустой он потому что мы из него построили дерево
				root2 = treeCopy(&root);
                Zamena1(&root2);
				break;
			}

			case 2:
			{
				printf("Исходное выражение: %s\n", expr);

				break;
			}

			case 3:
			{
				LKP(&root2);
				
				printf("\n");

				break;
			}

			case 4:
			{
				if (root != NULL)
				{
					printf("Дерево исходного выражения\n");
					PKL(&root, 0);
				}
				else
					printf("Дерево исходного выражения пусто\n");

				break;
			}

			case 5:
			{
				if (root2 != NULL)
				{
					printf("Дерево преобразованного выражения\n");
					PKL(&root2, 0);
				}
				else
					printf("Дерево преобразованного выражения пусто\n");

				break;
			}

			case 6: break;

			default:
			{
				printf("Ошибка. Такого пункта меню не существует\n");

				break;
			}
		}

		if (action == 6)
			break;
	}

	treeDestroy(&root);
	treeDestroy(&root2);

	return 0;	
}
