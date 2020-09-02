// gcc main.c stack.c -o laba24 -lm && ./laba24
// как надо читать этот код? Сначала идешь в main и там по строчке разбираешь. Смотришь в main какие он функции вызывает и бежишь к функции глядишь че она там творит
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack.h"

typedef struct _Node
{
	char _varOp;	// _var0p это символ
	double _num;	// _num это число видимо
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
	// Node * это возвращаемый тип данных
	Node *tmpNode = (Node *)malloc(sizeof(Node));

	tmpNode->_varOp = '\0';
	// '\0' это нуль-символ Строковая константа — это последовательность символов, заключенная в кавычки, например: «Это строковая константа». В конце каждой строковой константы компилятор помещает ‘\0’ (нуль-символ), чтобы программе было возможно определить конец строки.
	tmpNode->_num = 0.0;
	tmpNode->_left = NULL;
	tmpNode->_right = NULL;

	return tmpNode;
}

Node *treeCopy(Node **node){ // root2 = treeCopy(&root); а root мы взяли из treeBuild(&root, &stPost);
	// что за node? Мы потом в case1 вызываем treecopy($root) то есть вместо node у нас буит root
	Node *tmpNode = NULL;

	if (*node == NULL)
		return NULL;

	tmpNode = treeNodeCreate();
	tmpNode->_varOp = (*node)->_varOp;
	tmpNode->_num = (*node)->_num;
	tmpNode->_left = treeCopy(&((*node)->_left));  //мы должны сначала рызыменовать node конструкцией *node тем самым получив адрес node (два раза разыменуешь и получишь само node. Ведь у нас **node). После первого разыменования мы сможем обратиться к left (не забываем что node-> тож самое что (*node). это значит что стрелочкой мы еще раз разыменуем и наконец дойдем до самого значения node) А после обращения мы опять берем адрес, только уже от (*node->left) чтобы поместить в функцию. Ведь аргумент данной функции требует двойной адресации. Или как это назвать. Там де Node **node
	tmpNode->_right = treeCopy(&((*node)->_right));
	return tmpNode; // а как это раюотает? когда дойдет до return
	/*
	tmpNode->_left = treeCopy(&((*node)->_left));
	как работает эта штука и с следующей строчкой
	лучше всего это понять когда сам увидишь через debug
	сначала мы перешли от корня к левой ноде. Потом мы смотрим есть ли у нее сосед слева. Если есть. То снова идем к левой ноде поддерева. Если нет. То идем к правой ноде поддерева. И то же самое. Сначала чекаем левую, у левой_1 еще раз левую_2 правую_2 потом правую_1. Потом. Когда закончим работать с изначальным левым поддеревом, мы перейдем от корня к правому поддереву и также будет все работать
	Вопрос. А что же в конце программы вернет return?
	Ответ. Это как то хитро работает. И в конце работы treeCope возвращается КОРЕНЬ дерева. Хз как это работает. Но вот
	Вопрос. Куда мы сохраняем дерево?
	Ответ. Никуда скорее всего. Мы оставляем указатели от каждой ноды к следующей ноде. И вот так все сохраняется. Чтобы как в массиве числа сохранялось дерево. Вроде такого нет

	опять же. Чтобы лучше понять, стоит самому програть этот цикл через debug
	*/
}

int treeIsMinusNode(Node **node){// эта функци проверяет если в ноде '-' и чтобы в левом ее потомке не было ни буквы ни числа ни знака
	if (*node == NULL)	// мы вызываем ее из LKP treeIsMinusNode(node) где node = &root2
		return 0;

	if ((*node)->_left == NULL || (*node)->_right == NULL)
		return 0;

	return ((*node)->_varOp == '-' && (*node)->_left->_varOp == '\0' && (*node)->_left->_num == 0.0);
}

void treeBuild(Node **node, Stack *st){// построение дерева ( http://algolist.manual.ru/syntax/revpn.php )
	Token token;

	if (stackEmpty(st)) // если стек st пустой 
		return; // завершить работу функции treeBuild

	token = stackTop(st); // token это верхний элемент стека

	stackPop(st);// удаление верхнего элемента стека st

	(*node) = treeNodeCreate(); // создание одной ноды
	(*node)->_varOp = token._varOp; // запись в нолу сивола
	(*node)->_num = token._num; // запись в ноду числа

	if (isOp((*node)->_varOp)){ // если в _varOp у нас операция была, то мы создаем справа и слева ноды. Вдруг так букавки к в ( http://algolist.manual.ru/syntax/revpn.php ) или еще операции
		treeBuild(&(*node)->_right, st); // возвращаем эту же функцию. Но в лей будет следующий элемент в стэке, ведь предыдущий мы удалили
		treeBuild(&(*node)->_left, st); // получается node это двойной адрес? зачем
	} // мы вызывали treeBuild(&root, &stPost); значит в качестве node у нас будет &root
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
// вопрос к LKP. Как она работает. А именно. Почему когда цикл LKP(&(*node)->_left); у нас заканчивается то якобы нода левая у нас удаляется. Хотя нигде она к нулю не приравнивается
// страшный цикл. Без дебага и рисунка сюда не лезть ни в коем случае. Сожрет нахуй
void LKP(Node **node){ // печать преобразованного выражения. Из уже готового дерева
	if (*node == NULL) // LKP(&root2); где root2 это корень преобразованного дерева
		return;

	if ((*node)->_left != NULL && !treeIsMinusNode(node)){
	// if выше. Если левый потомок ненулевой(нахуя?) и в ноде НЕ "-" 
		if ((*node)->_left->_left != NULL) // это по логике. Надо нарисовать дерево чтобы понять
			printf("(");
		/*для уравнения a+4*a
		          +
		    +          *  
		  i   4     4     +
				        i   4
		Если левый подпотомой ненулевой. А это i то мы пишем скобочку. Если подпотомок ненулевой то потомок это точно операция. Операция между двумя числами. Значи там точно нужна скобочка в начале
		*/

		
		LKP(&(*node)->_left); //  спускаемся в левый низ пока не дойдем до нуля
		/* LKP работает хитро. Он как бы рекурсивно вызывается. Как матрешка. Самая большая матрешка это самая последняя. То есть в данном случае это i самое левое. После того как с ней мы разобрались, для нее функция перестает работать и мы возвращаемся к поменьше матрешке. Это к +. Это очень хорошо видно в дебаге
		LKP(+)
			LKP(+)
				LKP(i)
		как только закончил работу LKP(i) мы возвращаемся к LKP(+) что на уровень выше. То есть к ноде с + на втором уровне. Вот так хитро 
		*/
		if ((*node)->_left->_left != NULL) // после того как мы спустились в самый левый низ то данный if пропускается потому что (*node)->_left == NULL. Мы пробегаемся по другим if, а после возвращаемся к LKP(+) уровнем выше и бежим к этому if что я сейчас комменчу
			printf(")");
	}

	if ((*node)->_varOp != '\0') // спустились в самый левый низ. Пишем буковку
		printf("%c", (*node)->_varOp);
	else // а если не буковка то пишем циферку. Смотря че тама
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

void postOrder(const char *str, Stack *st){
// postOrder(expr, &stPost);
	// где expr это выражение с клавиатуры а stPost это структура
    
	int i = 0, step = -1, isBracket = 0, isDot = 0;	// isBracket это скобка; is dot точка; step степень; i данный читаемый элемент

	char tmpCh;
	// Token tk это структура с числом и букавками
	Token tk; // tk это имя переменной структуры. По которой можно обращаться к структуре по точке
	Stack stOp; // у нас два стека. st и st0p. st это стек вывода уже. А st0p он собирает элементы и сравнивает

	stackCreate(&stOp);

	tk._varOp = '\0'; 	// var0p это символ
	tk._num = 0.0;

	while (str[i] != '\0'){	// пока строка не дошла до конца 
		// что такое конструкция else if. if(key=2) else if(key = 3) else{}...  звучит так. Если ключ равно 2... если ключ равно 3... и во всех остальных случаях, то есть когда на 2 ни 3
		if (str[i] == '.')
			isDot = 1;
		else if (isLetter(str[i])){ // если условие выполнено
			// записываем в токен(var0p) значение: букву
			// функция isLetter: return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
			tk._varOp = str[i];

			stackPush(st, tk); 			// добавление элемента tk в стэк st
		}
		else if (isNumber(str[i])){
			// записываем в токен(num) значение: число
			tk._varOp = '\0';

			if (!isDot)
				// str[i] - '0'преобразует символ в позиции i в числовую цифру. Подробнее тут https://stackoverflow.com/questions/36942227/what-is-the-purpose-of-using-stri-0-where-str-is-a-string
				tk._num = tk._num * 10.0 + str[i] - '0';


			if (str[i + 1] != '.' && !isNumber(str[i + 1])){ // !isNumber(str[i + 1] в данном случае означанет что функиця должна выдать нулик. Тогда сработает if. И еще! Тут баг в этой программе и если что, она работает ТОЛЬКО с целочисленнными значениями. Если попытаешься 5.5 например ввести то выйдет жопа
				stackPush(st, tk);

				tk._num = 0.0;
				step = -1;
				isDot = 0;
			}
		}
		else if (isOp(str[i])){// Пока непонятно. для операций + - ... Кажись это пошла Дейкстра
			tk._varOp = str[i];

			if (str[i] == ')')
				isBracket = 1; // is Bracket переводится как скобочка
			/*
			else if (str[i] == '-' && (i == 0 || str[i - 1] == '(')){// бесполезный цикл
				tmpCh = tk._varOp;
				tk._varOp = '\0';
				tk._num = 0.0;

				stackPush(st, tk);

				tk._varOp = tmpCh;
				
			} */

			while (!stackEmpty(&stOp) && (isOpHigh(stackTop(&stOp)._varOp, str[i]) || isBracket)){// пока( стек не пустой && ( по приоритету элемент в стэке stOp больше элемента что считываем || b != 0) )
				// !stackEmpty(&stOp) в данном случае ! распространяется только на эту функцию. !stackEmpty(&stOp) озачаем, что пока эта функция не выполняется. Эта функция проверяет пустой ли стек. Получается с ! функция проверяет невыполнение этого условия
				// теперь про || isBracket. Почему мы вкинули просто переменную в цикл ни к чемц не приравняв? Если кидаешь просто переменную в цикл, например, if(b){...}, то на нее накладывается условие !=0. То есть конструкция if(b) равноправна if(b != 0)
				if (stackTop(&stOp)._varOp == '(') // бесполезный цикл потому что никто не ставил ( и сразу )
					isBracket = 0;
				else
					stackPush(st, stackTop(&stOp));// добавление в вывод элемента с приоритетом большим чем в элемента строчке

				stackPop(&stOp); // удаление верхнего элемента
			}

			if (str[i] != ')')  // вот это не совсем понял зачем
				stackPush(&stOp, tk);
		}

		i++;
	}

	while (!stackEmpty(&stOp)){ // если строчка закончилась но элементы в стеке st0p еще остались (данный вариант описан внизу в картинке в http://algolist.manual.ru/syntax/revpn.php) то мы добавляем этот элемент в выходную строку
		stackPush(st, stackTop(&stOp));
		stackPop(&stOp);
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
				// если мы только начали работу программы. Вводим первое выражение. То следующие две функции закончатся сразу же т.к. root = root2 = NULL
				// две функции ниже предназначены если ты одно выражение преобразовал и решил преобразовать еще одно другое выражение не выходя из программы
				treeDestroy(&root);
				treeDestroy(&root2);
				// stackCreate находится в подпрограмме stack.c
				stackCreate(&stPost);
				postOrder(expr, &stPost);
				treeBuild(&root, &stPost); // node в качестве &root
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
