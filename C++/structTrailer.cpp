#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
struct Trailer;

struct Lider
{
	int key;//  Номер вершины
	int count;//   ( 	)Число предшественников число входящих дуг
	Lider* nx;//         	(Lider)Указатель на следующий элемент в списке вершин
	Trailer* tr;// 	RAILER (   	,   указатель на Т список указателей на вершины к	)которым ведут исходящие дуги
};
struct Trailer
{
	Lider* ld;//       	LEADER, 	указатель на вершину в списке к которой ведет
            	//    	,   	TRAILERисходящая дуга из вершины к которой прикреплен
	Trailer* nx;//  	указатель на следующий элемент
};
/*
 * AddEdge ­     	добавление ребра в граф
 *	:Входные параметры
 * vertexes ­ 	список вершин графа
 *      	(vertexA,vertexB)Ребро графа определяется парой вершин
  */
void AddEdge(Lider*& vertexes, int vertexA, int vertexB) {
	Lider* ptrA = NULL, * ptrB = NULL;
	for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->nx);
	for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->nx);
	bool setEdge = true;
	if (ptrA && ptrB) {
    	Trailer* trTmp = ptrA->tr;
    	for (Trailer* trTmp = ptrA->tr; trTmp && trTmp->ld != ptrB; trTmp = trTmp->nx);
    	setEdge = !(bool)trTmp;
	}
	else {
    	//Создаем вершину А, добавляем в голову списка vertexes
    	if (!ptrA) {
        	ptrA = new Lider;
        	ptrA->key = vertexA;
        	ptrA->count = 0;
        	ptrA->nx = NULL;
        	ptrA->tr = NULL;

        	ptrA->nx = vertexes;
        	vertexes = ptrA;
    	}
    	//	B ,     	vertexes Создаем вершину добавляем в голову списка
    	if (!ptrB) {
        	ptrB = new Lider;
        	ptrB->key = vertexB;
        	ptrB->count = 0;
        	ptrB->nx = NULL;
        	ptrB->tr = NULL;
        	ptrB->nx = vertexes;
        	vertexes = ptrB;
    	}

	}
	// 	Устанавливаем ребро
	if (setEdge) {
    	ptrB->count++;
    	Trailer* elemTr = new Trailer;
    	elemTr->ld = ptrB;
    	elemTr->nx = ptrA->tr;
    	ptrA->tr = elemTr;
	}
}
/*
 * PrintGraph ­   Вывод графа
 *	:Входные параметры
 * vertexes ­ 	список вершин графа
 *
 */
void PrintGraph(Lider* vertexes) {
	if (vertexes == NULL)
    	printf("Graph is Empty");
	else
    	while (vertexes) {
        	Trailer* trTmp = vertexes->tr;
        	while (trTmp) {
            	printf("(%d,%d)\n", vertexes->key, trTmp->ld->key);
            	trTmp = trTmp->nx;
        	}
        	vertexes = vertexes->nx;
    	}
}
/*
 * DeleteGraph ­   удаление графа
 *	:Входные параметры
 * vertexes ­ 	список вершин графа
 *
 */
void DeleteGraph(Lider*& vertexes) {
	while (vertexes) {
    	while (vertexes->tr) {
        	Trailer* tmp = vertexes->tr;
        	vertexes->tr = vertexes->tr->nx;
        	delete tmp;
    	}
    	Lider* tmp = vertexes;
    	vertexes = vertexes->nx;
    	delete tmp;
	}

}
/*
 * ExcludeEdge ­     	Функция удаления ребра из графа
 *	:Входные пареметры
 * vertexes ­ 	список вершин графа
 *      	(vertexA,vertexB)Ребро графа определяется парой вершин
 *	:Возвращаемое значение
 * true  ­   ребро удалено
 * false ­   	ребро не удалено
 *
 */
bool ExcludeEdge(Lider*& vertexes, int vertexA, int vertexB) {
	Lider* ptrA = NULL, *ptrB = NULL;
	for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->nx);
	for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->nx);
	if (ptrA && ptrB && ptrA->tr)
	{
    	if (ptrA->tr->ld == ptrB) {
            	Trailer* t = ptrA->tr;
            	ptrA->tr = ptrA->tr->nx;
            	delete t;
            	ptrB->count--;

    	}
    	else {
        	Trailer* trTmp;
        	for (trTmp = ptrA->tr; trTmp->nx && trTmp->nx->ld != ptrB; trTmp = trTmp->nx);
        	if (trTmp->nx) {
        	Trailer* t = trTmp->nx;
        	trTmp->nx = trTmp->nx->nx;
        	delete t;
        	ptrB->count--;
        	}
    	}
    	return true;
	}
	return false;
}
// Функция удаления вершины из графа
// Входные значения: список вершин графа (vertexes) и номер/название вершины(vertex)
// Выходящие значения: вершина удалена(true) и вершина не удалена(false)
bool DeleteVertex(Lider*& vertexes, int vertex)
{
	Lider* ptrA = NULL, *ptrA_p = NULL, *ptrB = NULL, *ptrC = NULL;
	for (ptrA = vertexes; ptrA && ptrA->key != vertex; ptrA = ptrA->nx); // Цикл ищет адрес на требуемую вершину
    
	if (!ptrA) return false; // Если по требуемому номеру вершина не найдена, то выходим из функции

	Trailer* trlA = ptrA->tr; bool flg = true;
	while (trlA && flg) // Цикл удаления потомков вершины
	{
    	ptrA_p = trlA->ld; // Адрес на потомка
    	flg = ExcludeEdge(vertexes, vertex, ptrA_p->key);
    	trlA = ptrA->tr;
	}

	Trailer* trlC = NULL; bool flag = true;
	for (ptrB = vertexes; ptrB && flag; ptrB = ptrB->nx) // Цикл удаления предков вершины. Проходим по всему графу и ищем предков по потомкам рассмотриваемой вершины
	{
    	trlC = ptrB->tr;
    	while (trlC && flag) // Проверяем всех потомков и ищем заданую вершину
    	{
        	if (trlC->ld == ptrA) // Если мы нашли нашу вершину, то удаляем соединяющее ребро
        	{
            	flag = ExcludeEdge(vertexes, ptrB->key, vertex);
            	trlC = NULL;
        	}
        	else trlC = trlC->nx;
    	}
	}
	if (!flag) return false; // Если хотя бы одно ребро, связанное с заданной вершиной, не удалилось, то выходим с отрицательным результатом

	if (vertexes == ptrA) // Удаляем саму вершину
	{ // Если она оказалась во главе списка
    	vertexes = vertexes->nx;
    	delete(ptrA);
    	ptrA = NULL;
	}
	else
	{ // И если она в любом другом месте списка
    	for (ptrC = vertexes; ptrC && ptrC->nx && ptrC->nx->key != ptrA->key; ptrC = ptrC->nx);
    	if (ptrA->nx && ptrC) ptrC->nx = ptrA->nx;
    	else if(ptrC) ptrC->nx = NULL;
    	delete(ptrA);
    	ptrA = NULL;
	}
	return true;
}
/*
 *       	graph.datИсходный граф хранится в файле
 *	,  : списком ребер например
 * 1 2
 * 1 3
 * 1 5
 * 4 5
 * 2 4
 *
 */
int main(int argc, char** argv) {

	FILE* graphInFile = fopen("C:\\Users\\elena\\source\\repos\\zachet ASID\\graph.dat.txt", "r");
	Lider* graph = NULL;
	int vertexA = 0, vertexB = 0;
	int k = 0;
	if (graphInFile)
	{
    	while (k = fscanf(graphInFile, "%d%d", &vertexA, &vertexB) > 0) {
        	printf("(%d,%d)\n", vertexA, vertexB);
        	AddEdge(graph, vertexA, vertexB);
    	}

    	fclose(graphInFile);
    	puts("Print Graph");
    	PrintGraph(graph);
    	//ExcludeEdge(graph, 1, 5);
    	//ExcludeEdge(graph, 4, 5);
    	//ExcludeEdge(graph, 5, 3);
    	//PrintGraph(graph);
    	DeleteVertex(graph, 1);
    	cout << "__________________________\n";
    	PrintGraph(graph);
    	cout << "__________________________\n";
    	//DeleteGraph(graph);
    	//PrintGraph(graph);

	}
	else
    	puts("File is not open");
	return 0;
}
