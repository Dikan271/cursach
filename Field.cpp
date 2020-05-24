#include "Field.h"

Field::Field()
{
}
//метод инициализирует поле
void Field::InitializationMap(vector < vector < pair<char,int> > > *graph, int &n_out, int &m_out)
{
	string text = GetTextFromFile();    //сичтывается текст из файла
    GetSize(text, n_out, m_out);        // поиск размеров поля
    graph->clear();                     //если граф использовался ранее, нужно его очистить
    graph->resize(n_out, vector< pair<char,int> >(m_out, make_pair(' ', 99999)));
	int iter = 0;
	for (int i = 0; i < n_out; i++)
	{
		for (int j = 0; j <= m_out; j++, iter++)
		{   //заполнение графа
			(*graph)[i][j].first = text[iter];
		}
	}
}

//метод считывает из текст из файла
string Field::GetTextFromFile()
{
	string txt;
	fstream file("file/map2.txt", ios_base::in);
	char simbol;
	while ((simbol = file.get()) != EOF)
	{
		txt += simbol;
	}
	return txt;
}

//метод считает размеры поля
void Field::GetSize(string txt_Location, int &n_out, int &m_out)
{
	int heigth = 0;
	int length = txt_Location.size();
	for (int i = 0; i < length; i++)
	{
		if (txt_Location[i] == '\n')
			heigth++;
	}
	length /= ++heigth;
	n_out = heigth;
	m_out = length;
}
