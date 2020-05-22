#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>
#include <windows.h>
#include <fstream>
#include <string>

#define INF 200

using namespace std;
struct point
{
    int x;
    int y;
};
vector < vector < pair<char,int> > > graph;

void setcolor(unsigned short color);
void gotoxy(int x, int y) ;
void Print(int n, int m);
void FindPath(int n, int m, point finish);
void ExportFromFile();

int main()
{

    int n, m;
    cin >> n >> m;
    graph.resize(n,vector< pair<char,int> >(m,make_pair('/', INF)));
    point start{9, 7};
    point finish{5,5};
//    do
//    {
//        cout << "start: ";
//        cin >> start.x >> start.y;
//        cout << "finish: ";
//        cin >> finish.x >> finish.y;
//    }
//    while(start.x >= m || start.y >= n);
    start.x--;
    start.y--;
    finish.y--;
    finish.x--;
    Print(n, m);
    queue<point> frontier;
    frontier.push(start);
    graph[start.y][start.x].second = 0;
    point temp;
    while(!frontier.empty())
    {
        temp = frontier.front();
        frontier.pop();
        graph[temp.y][temp.x].first = '.';
        if(temp.y-1 >= 0
                && graph[temp.y-1][temp.x].second == INF
                && graph[temp.y-1][temp.x].first != '#')
        {
            frontier.push({temp.x, temp.y-1});
            graph[temp.y-1][temp.x].second = graph[temp.y][temp.x].second + 1;
            graph[temp.y-1][temp.x].first = '+';
        }
        if(temp.x-1 >= 0
                && graph[temp.y][temp.x-1].second == INF
                && graph[temp.y][temp.x-1].first != '#')
        {
            frontier.push({temp.x-1, temp.y});
            graph[temp.y][temp.x-1].second = graph[temp.y][temp.x].second + 1;
            graph[temp.y][temp.x-1].first = '+';
        }
        if(temp.y+1 < n
                && graph[temp.y+1][temp.x].second == INF
                && graph[temp.y+1][temp.x].first != '#')
        {
            frontier.push({temp.x, temp.y+1});
            graph[temp.y+1][temp.x].second = graph[temp.y][temp.x].second + 1;
            graph[temp.y+1][temp.x].first = '+';
        }
        if(temp.x+1 < m
                && graph[temp.y][temp.x+1].second == INF
                && graph[temp.y][temp.x+1].first != '#')
        {
            frontier.push({temp.x+1, temp.y});
            graph[temp.y][temp.x+1].second = graph[temp.y][temp.x].second + 1;
            graph[temp.y][temp.x+1].first = '+';
        }
    }
    system("pause");

    Print(n, m);

    system("pause");
    FindPath(n, m, finish);
    Print(n, m);
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

void Print(int n, int m)
{
    system("cls");

    for(int i = 0; i <= m; i++)
        cout << setw(5) << left << i;
    cout << endl;
    for(int i = 0; i < n; i++)
    {
        cout << setw(5) << left << i+1;
        for(int j = 0; j < m; j++)
        {
//            if(graph[i][j].first == '+')
//                setcolor(1);
//            else
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

    for(int i = 0; i <= n; i++)
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
            cout << setw(5) << left << graph[i][j].second ;
            setcolor(7);
        }
        cout << endl;
    }
    cout << endl;
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
