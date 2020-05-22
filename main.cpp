#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>
#include <windows.h>

#define INF 99999

using namespace std;
struct point
{
    int x;
    int y;
};
vector < vector < pair<char,int> > > graph;

void setcolor(unsigned short color);
void gotoxy(int x, int y);

void InitializeWin(int* n, int* m, point* start, point* finish);
void PrintField(int n, int m);
void PrintDijkstraMap(int n, int m);

void CalculateDistance(int n, int m, point start);
void SetNextNode(point next, point current);
void FindPath(int n, int m, point finish);

int main()
{

    int n, m;
    point start{9, 7};
    point finish{5,5};

    InitializeWin(&n, &m, &start, &finish);
    PrintField(n, m);
    CalculateDistance(n, m, start);
    FindPath(n, m, finish);

    PrintField(n, m);
}

void CalculateDistance(int n, int m, point start)
{
    queue<point> frontier;
    frontier.push(start);
    graph[start.y][start.x].second = 0;
    point temp;

    while(!frontier.empty())
    {
        temp = frontier.front();
        frontier.pop();
        gotoxy(5*(temp.x+1),(temp.y+1));
        setcolor(2);
        cout << graph[temp.y][temp.x].second;

        if(temp.y-1 >= 0
                && graph[temp.y-1][temp.x].second == INF
                && graph[temp.y-1][temp.x].first != '#')
        {
            frontier.push({temp.x, temp.y-1});
            SetNextNode({temp.x, temp.y-1}, temp);
        }
        if(temp.x-1 >= 0
                && graph[temp.y][temp.x-1].second == INF
                && graph[temp.y][temp.x-1].first != '#')
        {
            frontier.push({temp.x-1, temp.y});
            SetNextNode({temp.x-1, temp.y}, temp);
        }
        if(temp.y+1 < n
                && graph[temp.y+1][temp.x].second == INF
                && graph[temp.y+1][temp.x].first != '#')
        {
            frontier.push({temp.x, temp.y+1});
            SetNextNode({temp.x, temp.y+1}, temp);
        }
        if(temp.x+1 < m
                && graph[temp.y][temp.x+1].second == INF
                && graph[temp.y][temp.x+1].first != '#')
        {
            frontier.push({temp.x+1, temp.y});
            SetNextNode({temp.x+1, temp.y}, temp);
        }
        Sleep(10);
    }
}

void SetNextNode(point next, point current)
{
    graph[next.y][next.x].second = graph[current.y][current.x].second + 1;
    graph[next.y][next.x].first = '.';

    gotoxy(5*(next.x+1), (next.y+1));
    setcolor(1);
    cout << graph[next.y][next.x].second;

}

void FindPath(int n, int m, point finish)
{
    point temp{finish.x, finish.y};
    graph[temp.y][temp.x].first = '@';
    while(graph[temp.y][temp.x].second != 0)
    {
        if(graph[temp.y-1][temp.x].second <= graph[temp.y][temp.x].second)
            temp.y--;
        if(graph[temp.y+1][temp.x].second <= graph[temp.y][temp.x].second)
            temp.y++;
        if(graph[temp.y][temp.x-1].second <= graph[temp.y][temp.x].second)
            temp.x--;
        if(graph[temp.y][temp.x+1].second <= graph[temp.y][temp.x].second)
            temp.x++;
        graph[temp.y][temp.x].first = '@';
    }
}

void PrintField(int n, int m)
{
    system("cls");
    setcolor(7);
    for(int i = 0; i <= m; i++)
        cout << setw(5) << left << i;
    cout << endl;
    for(int i = 0; i < n; i++)
    {
        cout << setw(5) << left << i+1;
        for(int j = 0; j < m; j++)
        {
            if(graph[i][j].first == '.')
                setcolor(2);
            else if(graph[i][j].first == '#')
                setcolor(3);
            cout << setw(5) << left << graph[i][j].first;
            setcolor(7);
        }
        cout << endl;
    }
    cout << endl;
}

void PrintDijkstraMap(int n, int m)
{
    system("cls");
    setcolor(7);
    for(int i = 0; i <= m; i++)
        cout << setw(5) << left << i;
    cout << endl;
    for(int i = 0; i < n; i++)
    {
        cout << setw(5) << left << i+1;
        for(int j = 0; j < m; j++)
        {
            if(graph[i][j].first == '.')
                setcolor(2);
            else if(graph[i][j].first == '#')
                setcolor(3);
            cout << setw(5) << left << graph[i][j].second;
            setcolor(7);
        }
        cout << endl;
    }
    cout << endl;
}

void InitializeWin(int* n, int* m, point* start, point* finish)
{
    cout << "sizes(x, y):";
    cin >> *m >> *n;
    graph.resize(*n,vector< pair<char,int> >(*m,make_pair(' ', INF)));
    do
    {
        cout << "start(x, y) : ";
        cin >> start->x >> start->y;
        cout << "finish(x, y): ";
        cin >> finish->x >> finish->y;
    }
    while((start->x<=0 || start->y  <=0)||
            (finish->x <=0 || finish->y <=0)||
            (start->x  >*m || start->y  >*n)||
            (finish->x >*m || finish->y >*n));
    start->x--;
    start->y--;
    finish->y--;
    finish->x--;
}

void setcolor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,color);
}

void gotoxy(int x, int y)
{
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}
