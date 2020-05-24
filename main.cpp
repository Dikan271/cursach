#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>
#include <windows.h>
#include <fstream>
#include <string>

#include "Field.h"

#define INF 99999

using namespace std;
//перечисление для управления ходом порграммы
enum selection
{
    printDMap,
    printField,
    endPogram,
    file,
    rectangle
};
struct point
{
    int x;
    int y;
};
//граф как глобальная переменная
vector < vector < pair<char,int> > > graph;
int sizeCell; //размер клетки в ширину

// декоративные функции
void setcolor(unsigned short color);
void gotoxy(int x, int y);

//функции управления
selection Menu();
void ImportFromFile(int* n, int* m, point* start, point* finish);
void InitializeWin(int* n, int* m, point* start, point* finish);
void SetControlPoint(int* n, int* m, point* start, point* finish);

//функции вывода
void PrintField(int n, int m);
void PrintDijkstraMap(int n, int m);

// ункции поиска
void StartAlgorithms(int &n, int &m, point start, point finish, selection mode);
void CalculateDistance(int n, int m, point start, point finish);
bool SetNextNode(point next, point current, point finish);
void FindPath(int n, int m, point finish);

int main()
{
    int n, m;
    point start;    //точки стартовой и конечной позиции поиска пуит
    point finish;
    selection chose = rectangle;
    while(chose != endPogram)
    {
        switch(chose)
        {
        case file:
        case rectangle:
            StartAlgorithms(n, m, start, finish, chose);
        case printField:
            PrintField(n, m);
            break;
        case printDMap:
            if(sizeCell >= 5)
                PrintDijkstraMap(n, m);
            else //если ячейка будет менньше 5 единиц, то веса вершин, не поместятся в ячейку
                cout << "impossible to print DM, because a very small cell size" << endl;
            break;
        case endPogram:
            return 0;
        }
        chose = Menu(); //запрос на дальнейшие дуйствия
    }
}

void StartAlgorithms(int &n, int &m, point start, point finish, selection mode)
{
    /* по умолчанию режим программы включен на прямоугольник,
    но по завершению первного прохода, пользователь выбирает сам*/
    if(mode == file)
        ImportFromFile(&n, &m, &start, &finish);
    if(mode == rectangle)
        InitializeWin(&n, &m, &start, &finish);
    PrintField(n, m);
    CalculateDistance(n, m, start, finish);
    /*после того как посчитаны дистанции до каждой точки, можно начинать поиск пути*/
    FindPath(n, m, finish);
}

//создание пустого от препядствий поля, для демогстрации процесса поиска
void InitializeWin(int* n, int* m, point* start, point* finish)
{
    system("cls");
    sizeCell = 5;   //стандартный размер ячейки
    cout << "create rectangle\n";
    do
    {
        cout << "sizes(x <=40, y <= 40):";
        cin >> *m >> *n;
    }//проверка на границы поля
    while(*m<=0 ||*n <=0||
         (*m>40 ||*n >40));
    //если граф ранее был использован, очищаем его
    graph.clear();
    //заполняем граф новыми значениями, все веса по умолчанию бесконечны
    graph.resize(*n,vector< pair<char,int> >(*m,make_pair(' ', INF)));
    //задаём контрольные точки
    SetControlPoint(n, m, start, finish);
}

//Загружает поле из заранее подготовленного файла
void ImportFromFile(int* n, int* m, point* start, point* finish)
{
    //ячейка единичная, так как поле слишком большое
    sizeCell = 1;
    Field file; //используем класс, который выгружжает поле из файла
    file.InitializationMap(&graph, *n, *m);
    SetControlPoint(n, m, start, finish);
}

//функция устанавливает точки поиска пути
void SetControlPoint(int* n, int* m, point* start, point* finish)
{
    PrintField(*n, *m);
    cout << "size(x, y): " << *m << ' ' << *n << endl;
    do
    {
        cout << "start(x, y) : ";
        cin >> start->x >> start->y;
        cout << "finish(x, y): ";
        cin >> finish->x >> finish->y;
    } //проверка на границы поля и премядствия на поле
    while((start->x<=0 || start->y  <=0)||
        (finish->x <=0 || finish->y <=0)||
        (start->x  >*m || start->y  >*n)||
        (graph[start->y-1][start->x-1].first  =='#')||
        (graph[finish->y-1][finish->x-1].first=='#'));
    start->x--;
    start->y--;
    finish->y--;
    finish->x--;
}

//функция Меню пользователя
selection Menu()
{
    int t;
    cout<< "Choose action:" << endl
        << "1: New Example" << endl
        << "2: Print Field" << endl
        << "3: Print Dijkstra Map" << endl
        << "4: Exit" << endl;
    do
    {
        cin >> t;
        switch(t)
        {
        case 2:
            return printField;
        case 3:
            return printDMap;
        case 4:
            return endPogram;
        case 1: //пользователь выбирает следующий пример,
            {   //стоит выбор источника поля.
                cout<< "Choose source:" << endl
                    << "1: Import from file" <<endl
                    << "2: Create clear rectangle" << endl;
                cin >> t;
                if(t == 1)
                    return file;
                if(t == 2)
                    return rectangle;
            }
        default: //при неверном значении, сообщается об ошибке
            cout << "Error. Please, repeat:";
        }
    }
    while(true);
}

//функция подсчёта дистанций
void CalculateDistance(int n, int m, point start, point finish)
{
    queue<point> frontier;  //очередь из вершин
    frontier.push(start);   //помещаем в очередь стартовую позицию
    graph[start.y][start.x].second = 0;
    point temp;  //временная переменная, для хранения координат нынешней вершины
    /*необходимая задержка, для илюстрации процесса распростраения подсчета дистанций
    при больший полях, не нужна, так как принесёт большую задержку производительности*/
    bool toSleep = n+m <= 50;
    bool isEnd = false; //индикатор того, что точка найдена
    while(!frontier.empty())
    {
        temp = frontier.front();
        frontier.pop();
        /*переход каретки ввода на определённую координату консольного окна
        нужен для того чтобы пометить пройденное проле*/
        gotoxy(sizeCell*(temp.x+1),(temp.y+1));
        setcolor(2);
        cout << graph[temp.y][temp.x].first;

        if(temp.y-1 >= 0
                && graph[temp.y-1][temp.x].second == INF
                && graph[temp.y-1][temp.x].first != '#')
        {
            frontier.push({temp.x, temp.y-1});
            isEnd = SetNextNode({temp.x, temp.y-1}, temp, finish);
        }
        if(temp.x-1 >= 0
                && graph[temp.y][temp.x-1].second == INF
                && graph[temp.y][temp.x-1].first != '#')
        {
            frontier.push({temp.x-1, temp.y});
            isEnd = SetNextNode({temp.x-1, temp.y}, temp, finish);
        }
        if(temp.y+1 < n
                && graph[temp.y+1][temp.x].second == INF
                && graph[temp.y+1][temp.x].first != '#')
        {
            frontier.push({temp.x, temp.y+1});
            isEnd = SetNextNode({temp.x, temp.y+1}, temp, finish);
        }
        if(temp.x+1 < m
                && graph[temp.y][temp.x+1].second == INF
                && graph[temp.y][temp.x+1].first != '#')
        {
            frontier.push({temp.x+1, temp.y});
            isEnd = SetNextNode({temp.x+1, temp.y}, temp, finish);
        }
        if(isEnd)
            return;
        if(toSleep)
            Sleep(10);
    }
}

//функция изменяет веса в новой вершине и ставит специальный символ, который обозначает проверенную область
bool SetNextNode(point next, point current, point finish)
{
    //новые значения
    graph[next.y][next.x].second = graph[current.y][current.x].second + 1;
    graph[next.y][next.x].first = '.';
    //выводи значения на поле, синим цветом
    // синий цвет сигнализирует о том, что тоачна находится в очереди
    gotoxy(sizeCell*(next.x+1), (next.y+1));
    setcolor(1);
    cout << graph[next.y][next.x].first;
    //проверка на совпадение с искомой точкой
    return (next.x == finish.x && next.y == finish.y);
}

//функция которая ищет путь
void FindPath(int n, int m, point finish)
{
    //для найденного пути устанавливается белый цвет, чтобы можно бло отличить от поля
    setcolor(7);
    point temp{finish.x, finish.y};
    graph[temp.y][temp.x].first = '@';
    while(graph[temp.y][temp.x].second != 0)
    {
        if(temp.y >0 && graph[temp.y-1][temp.x].second <= graph[temp.y][temp.x].second)
            temp.y--;
        else if(temp.y <n-1 && graph[temp.y+1][temp.x].second <= graph[temp.y][temp.x].second)
            temp.y++;
        else if(temp.x >0 && graph[temp.y][temp.x-1].second <= graph[temp.y][temp.x].second)
            temp.x--;
        else if(temp.x <m-1 && graph[temp.y][temp.x+1].second <= graph[temp.y][temp.x].second)
            temp.x++;
        /* устанавливается специально обозначение пути
        и с задержкой выводиться за консоль*/
        graph[temp.y][temp.x].first = '@';
        gotoxy(sizeCell*(temp.x+1), (temp.y+1));
        cout << graph[temp.y][temp.x].first;
        Sleep(20);
    }
}

//функция вывода поля с обозначениями проверенной територии
void PrintField(int n, int m)
{
    system("cls");
    setcolor(7);
    //выводится нумерация по границцым поля
    for(int i = 0; i <= m; i++)
        cout << setw(sizeCell) << left << (i)%10;
    cout << endl;
    for(int i = 0; i < n; i++)
    {
        cout << setw(sizeCell) << left << (i+1)%10;
        for(int j = 0; j < m; j++)
        {
            //каждый символ имеет свой цвет
            if(graph[i][j].first == '.')
                setcolor(2);
            else if(graph[i][j].first == '#')
                setcolor(3);
            //если поле загружено не из файла, то создаётся отступ в пять единиц между вершинами
            cout << setw(sizeCell) << left << graph[i][j].first;
            setcolor(7);
        }
        cout << endl;
    }
    cout << endl;
}

//функция выводит на консоль, карту весов
void PrintDijkstraMap(int n, int m)
{
    system("cls");
    setcolor(7);
    for(int i = 0; i <= m; i++)
        cout << setw(sizeCell) << left << (i)%10;
    cout << endl;
    for(int i = 0; i < n; i++)
    {
        cout << setw(sizeCell) << left << (i+1)%10;
        for(int j = 0; j < m; j++)
        {
            if(graph[i][j].first == '.')
                setcolor(2);
            else if(graph[i][j].first == '#')
                setcolor(3);
            cout << setw(sizeCell) << left << graph[i][j].second;
            setcolor(7);
        }
        cout << endl;
    }
    cout << endl;
}

//функция изменения цвета вывода
void setcolor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,color);
}

//функция перемещения каретки на координаты консоли
void gotoxy(int x, int y)
{
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}
