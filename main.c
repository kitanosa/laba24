#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack.h"

typedef struct _Node
{
	char _varOp;
	double _num;
	struct _Node *_left;
	struct _Node *_right;
} Node;


Node *treeNodeCreate(void);
Node *treeCopy(Node **node);
Node *Zamena1(Node **node);
int treeIsMinusNode(Node **node);
int treeIsMinusNodeNum(Node **node);
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
	Node *tmpNode = (Node *)malloc(sizeof(Node));

	tmpNode->_varOp = '\0';
	tmpNode->_num = 0.0;
	tmpNode->_left = NULL;
	tmpNode->_right = NULL;

	return tmpNode;
}

Node *treeCopy(Node **node){
	Node *tmpNode = NULL;

	if (*node == NULL)
		return NULL;

	tmpNode = treeNodeCreate();
	tmpNode->_varOp = (*node)->_varOp;
	tmpNode->_num = (*node)->_num;
	tmpNode->_left = treeCopy(&(*node)->_left);
	tmpNode->_right = treeCopy(&(*node)->_right);
	return tmpNode;
}

int treeIsMinusNode(Node **node){
	if (*node == NULL)
		return 0;

	if ((*node)->_left == NULL || (*node)->_right == NULL)
		return 0;

	return ((*node)->_varOp == '-' && (*node)->_left->_varOp == '\0' && (*node)->_left->_num == 0.0);
}

void treeBuild(Node **node, Stack *st){
	Token token;

	if (stackEmpty(st))
		return;

	token = stackTop(st);

	stackPop(st);

	(*node) = treeNodeCreate();
	(*node)->_varOp = token._varOp;
	(*node)->_num = token._num;

	if (isOp((*node)->_varOp)){
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

	free(*node);

	*node = NULL;
}

void PKL(Node **node, const int level){
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

void LKP(Node **node){
	if (*node == NULL)
		return;

	if ((*node)->_left != NULL && !treeIsMinusNode(node)){
		if ((*node)->_left->_left != NULL)
			printf("(");
		
		LKP(&(*node)->_left);
		
		if ((*node)->_left->_left != NULL)
			printf(")");
	}

	if ((*node)->_varOp != '\0')
		printf("%c", (*node)->_varOp);
	else
		printf("%f", (*node)->_num);

	if ((*node)->_right != NULL){
		if ((*node)->_right->_left != NULL)
			printf("(");

		LKP(&(*node)->_right);
		
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

void postOrder(const char *str, Stack *st){
    
	int i = 0, step = -1, isBracket = 0, isDot = 0;
	char tmpCh;
	Token tk;
	Stack stOp;

	stackCreate(&stOp);

	tk._varOp = '\0';
	tk._num = 0.0;

	while (str[i] != '\0'){
		if (str[i] == '.')
			isDot = 1;
		else if (isLetter(str[i])){
			tk._varOp = str[i];

			stackPush(st, tk);
		}
		else if (isNumber(str[i])){
			tk._varOp = '\0';

			if (!isDot)
				tk._num = tk._num * 10.0 + str[i] - '0';
			else{
				tk._num = tk._num + pow(10.0, step) * (str[i] - '0');
				step--;
			}

			if (str[i + 1] != '.' && !isNumber(str[i + 1])){
				stackPush(st, tk);

				tk._num = 0.0;
				step = -1;
				isDot = 0;
			}
		}
		else if (isOp(str[i])){
			tk._varOp = str[i];

			if (str[i] == ')')
				isBracket = 1;
			else if (str[i] == '-' && (i == 0 || str[i - 1] == '(')){
				tmpCh = tk._varOp;
				tk._varOp = '\0';
				tk._num = 0.0;

				stackPush(st, tk);

				tk._varOp = tmpCh;
			}

			while (!stackEmpty(&stOp) && (isOpHigh(stackTop(&stOp)._varOp, str[i]) || isBracket)){
				if (stackTop(&stOp)._varOp == '(')
					isBracket = 0;
				else
					stackPush(st, stackTop(&stOp));

				stackPop(&stOp);
			}

			if (str[i] != ')')
				stackPush(&stOp, tk);
		}

		i++;
	}

	while (!stackEmpty(&stOp)){
		stackPush(st, stackTop(&stOp));
		stackPop(&stOp);
	}

	stackDestroy(&stOp);
}
Node *Zamena1(Node **node){
	if ((*node)!= NULL){
		
		
		if (((*node)->_left==NULL)  && ((*node)->_right==NULL) && ((*node)->_varOp =='a')){
			(*node)->_varOp ='+';
			
			Node *tmpNode = NULL;
			tmpNode = treeNodeCreate();
			tmpNode->_varOp = 'i';
			(*node)->_left=tmpNode;
			
			Node *tmpNode1 = NULL;
			tmpNode1 = treeNodeCreate();
			tmpNode1->_num = 4;
			(*node)->_right=tmpNode1;
		}
		else{
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
				treeDestroy(&root);
				treeDestroy(&root2);
				stackCreate(&stPost);
				postOrder(expr, &stPost);
				treeBuild(&root, &stPost);
				stackDestroy(&stPost);
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
