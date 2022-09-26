// snail.cpp
#pragma once

#include <iostream>
#include <iomanip>
using namespace std;

const int MAX = 100;
int arr[MAX][MAX] = {0, };

void print_snail(int num)
{
	cout << "****************\n";
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			//if (arr[i][j] < 10)
			//	cout << "0";
			//cout << arr[i][j] << " ";
			cout << setfill('0') << setw(2) << arr[i][j] << " ";
		}
		cout << endl;
	}
	cout << "****************\n";
}

void make_snail(int num)
{
	int i = 1, row = 0, col = 0;
	enum {RIGHT, DOWN, LEFT, UP};
	int dir = RIGHT;

	while (i <= num * num)
	{
		arr[row][col] = i;
		if (dir == RIGHT)
		{
			if (col == num - 1 || arr[row][col + 1] != 0)
			{
				dir = DOWN;
				row++;
			}
			else
				col++;
		}
		else if (dir == DOWN)
		{
			if (row == num - 1 || arr[row + 1][col] != 0)
			{
				dir = LEFT;
				col--;
			}
			else
				row++;
		}
		else if (dir == LEFT)
		{
			if (col == 0 || arr[row][col - 1] != 0)
			{
				dir = UP;
				row--;
			}
			else
				col--;
		}
		else if (dir == UP)
		{
			if (row == 0 || arr[row - 1][col] != 0)
			{
				dir = RIGHT;
				col++;
			}
			else
				row--;
		}
		i++;
	}
}

int main()
{
	int num;
	cin >> num;

	make_snail(num);
	print_snail(num);
}