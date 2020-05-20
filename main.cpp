#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>
#include <windows.h>

#define INF 200

using namespace std;
struct point
{
    int x;
    int y;
};
vector < vector < pair<char,int> > > graph;

void setcolor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,color);
}

void Print(int n, int m)
{
    system("cls");
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            if(graph[i][j].first == '+')
                setcolor(1);
            else if(graph[i][j].first == '.')
                setcolor(2);
            else if(graph[i][j].first == '#')
                setcolor(3);
            cout << setw(5) << left << graph[i][j].first;
            setcolor(7);
        }
        cout << endl;
    }
    cout << endl;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
             if(graph[i][j].first == '+')
                setcolor(1);
            else if(graph[i][j].first == '.')
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

int main()
{
    int n, m;
    cin >> n >> m;
    graph.resize(n,vector< pair<char,int> >(m,make_pair('.', INF)));
    point start;
    do
    {
       cin >> start.x >> start.y;
    }while(start.x >= m || start.y >= n);
    Print(n, m);

    queue<point> frontier;
    frontier.push(start);
    graph[start.x][start.y].second = 0;
    point temp;
    while(!frontier.empty())
    {
        temp = frontier.front();
        frontier.pop();
        graph[temp.x][temp.y].first = '.';

        if(temp.x-1 >= 0 && graph[temp.x-1][temp.y].second == INF
           && graph[temp.x-1][temp.y].first != '#')
        {
            frontier.push({temp.x-1, temp.y});
            graph[temp.x-1][temp.y].second = graph[temp.x][temp.y].second + 1;
            graph[temp.x-1][temp.y].first = '+';
        }
        if(temp.y-1 >= 0 && graph[temp.x][temp.y-1].second == INF
           && graph[temp.x][temp.y-1].first != '#')
        {
            frontier.push({temp.x, temp.y-1});
            graph[temp.x][temp.y-1].second = graph[temp.x][temp.y].second + 1;
            graph[temp.x][temp.y-1].first = '+';
        }
        if(temp.x+1 < m && graph[temp.x+1][temp.y].second == INF
           && graph[temp.x+1][temp.y].first != '#')
        {
            frontier.push({temp.x+1, temp.y});
            graph[temp.x+1][temp.y].second = graph[temp.x][temp.y].second + 1;
            graph[temp.x+1][temp.y].first = '+';

        }
        if(temp.y+1 < n
           && graph[temp.x][temp.y+1].second == INF
           && graph[temp.x][temp.y+1].first != '#')
        {
            frontier.push({temp.x, temp.y+1});
            graph[temp.x][temp.y+1].second = graph[temp.x][temp.y].second + 1;
            graph[temp.x][temp.y+1].first = '+';

        }
        cout << frontier.size() << endl;
        Print(n, m);
    }
}
