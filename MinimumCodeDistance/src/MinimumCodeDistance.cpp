// MinimumCodeDistance.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "Matrix.h"

int Factorial(int value)
{
	if (value < 0)
	{
		throw std::invalid_argument("Argument of factorial is negative.");
	}

	int result = 1;

	while (value > 1)
	{
		result *= value;
		--value;
	}
	
	return result;
}

int main()
{
	const int one = 1;
	const int zero = 0;

	int codeDistance[10] = { 0 };
	int checked = 0;
	short lines, columns;
	short maxCodeDistance = 1;
	std::string str;

	std::cout << "Enter code parameters (n k): ";
	std::cin >> columns >> lines;

	int vectorsQuantity = 0; //  quantity of unique vectors size of "lines"

	for (short i = 1; i <= lines; i++)
	{
		vectorsQuantity += (Factorial(lines)) / (Factorial(i) * Factorial(lines - i));
	}

	int** vectorArray = new int* [lines];

	for (short i = 0; i < lines; i++)
	{
		vectorArray[i] = new int[vectorsQuantity];
	}

	for (short i = 0; i < lines; i++)
	{
		for (int j = 0; j < vectorsQuantity; j++)
		{
			vectorArray[i][j] = 0;
		}
	}
	//  creating unique vectors

	int startPos;
	int endPos = 0;

	for (short k = 1; k <= lines; k++)
	{
		startPos = endPos;
		endPos += Factorial(lines) / (Factorial(k) * (Factorial(lines - k)));
		for (short vect = startPos; vect < endPos; vect++)
		{
			int c[100];
			int i;
			for (i = 0; i < k; i++)//n = k
				c[i] = k - i;

			while (1)
			{
				for (i = 0; i < k; i++)
				{
					vectorArray[c[i] - 1][vect] = 1;
				}

				for (i = 0; c[i] >= lines - i;)// m = lines
				{
					if (++i >= k)
					{
						goto m1; //  some kind of "return"
					}
				}
				for (c[i]++; i; i--)
				{
					c[i - 1] = c[i] + 1;
				}

				vect++;
			}
		}
	m1: continue;
	}

	unsigned __int64 matrixQuantity = 1;


	for (int i = 0; i < columns - lines; i++)
	{
		matrixQuantity *= vectorsQuantity;
	}


	Matrix* matrixArray = new Matrix[matrixQuantity];

	for (int i = 0; i < matrixQuantity; i++)
	{
		matrixArray[i].SetDimensions(columns, lines);
	}

	int i, n, m;
	int buffMatrix = 0;
	int c[100];

	short* columnsCombinations = new short[columns - lines]; // columnsCombinations[vectorsQuantity][matrixQuantity]
	// there starts making P submatrix as combinations of precobined vectors


	for (int i = 0; i <= (columns - lines); i++)
	{
		columnsCombinations[i] = 0;
	}

	for (int i = 0; i < matrixQuantity; i++)
	{
		for (int k = 0; k < (columns - lines); k++)
		{
			for (int f = 0; f < lines; f++)
			{
				matrixArray[i].SetCoordinate(vectorArray[f][columnsCombinations[k]], k + lines, f);
			}

		}

		columnsCombinations[0]++;

		for (int j = 0; columnsCombinations[j] >= vectorsQuantity;)
		{
			if (j < (columns - lines))
			{
				columnsCombinations[j] = 0;
				j++;
				columnsCombinations[j]++;
			}
		}
	}


	//  making E submatrix
	for (int i = 0; i < matrixQuantity; i++)
	{
		for (short line = 0; line < lines; line++)
		{
			for (short column = 0; column < lines; column++)
			{
				if (column == line)
				{
					matrixArray[i].SetCoordinate(1, column, line);
				}
				else
				{
					matrixArray[i].SetCoordinate(0, column, line);
				}
			}
		}
	}
	// end of making matrix

	//cout << "\n \n";


	for (int matrixCount = 0; matrixCount < matrixQuantity; matrixCount++)
	{
		matrixArray[matrixCount].SetCodeDistance(matrixArray[matrixCount].MinCodeDistance(checked));
	}

	for (int f = 0; f < matrixQuantity; f++)
	{
		if (maxCodeDistance < matrixArray[f].GetCodeDistance())
		{
			maxCodeDistance = matrixArray[f].GetCodeDistance();
		}
	}

	std::cout << "Max code distance is " << maxCodeDistance << "\n";

	for (int i = 0; i < matrixQuantity; i++)
	{
		int D = matrixArray[i].GetCodeDistance();

		codeDistance[D - 1]++;
	}

	for (short i = 0; i < maxCodeDistance; i++)
	{
		std::cout << "There are " << codeDistance[i] << " matrix with code distance " << i + 1 << "\n";
	}

	system("pause");

	return 0;
}

