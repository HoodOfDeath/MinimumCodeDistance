// MinimumCodeDistance.cpp : This file contains the 'main' function. Program execution begins and ends there.
#pragma once
#include "pch.h"
#include "Matrix.h"
#include <chrono>

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
	int codeDistance[10] = { 0 };
	int checked = 0;
	short lines, columns;
	short maxCodeDistance = 1;

	std::cout << "Enter code parameters (n k): ";
	std::cin >> columns >> lines;

	short pSubmatrixColumns(columns - lines);

	auto start = std::chrono::high_resolution_clock::now();

	int vectorsQuantity = (1 << pSubmatrixColumns); //  quantity of unique vectors size of "lines". Equals to (2 power "lines") - 1
	vectorsQuantity--;

	unsigned __int8* vectorArray = new unsigned __int8 [vectorsQuantity];

	for (unsigned int i = 1; i <= vectorsQuantity; i++)
	{
		vectorArray[i - 1] = i;
	}

	unsigned __int64 matrixQuantity = 1;

	for (int i = 0; i < lines; i++)
	{
		matrixQuantity *= vectorsQuantity;
	}


	Matrix* matrixArray = new Matrix[matrixQuantity];

	for (int i = 0; i < matrixQuantity; i++)
	{
		matrixArray[i].SetDimensions(columns, lines);
	}

	unsigned __int16* identitySubmatrix = new unsigned __int16[lines];

	for (int i = 0; i < lines; ++i)
	{
		identitySubmatrix[i] = (1 << (lines - i - 1));
	}

	short* linesCombinations = new short[lines]; // columnsCombinations[vectorsQuantity][matrixQuantity]
	// there starts making P submatrix as combinations of precombined vectors

	for (int i = 0; i < lines; i++)
	{
		linesCombinations[i] = 0;
	}

	for (int i = 0; i < matrixQuantity; i++)//this loot is iterate through all combinations of vectors
	{
		for (int lineIndex = 0; lineIndex < lines; ++lineIndex)
		{
			unsigned __int16 rowTemp = identitySubmatrix[lineIndex];
			unsigned short lineVectorIndex = linesCombinations[lineIndex];
			unsigned __int16 bitValue = vectorArray[lineVectorIndex];
			rowTemp = ((rowTemp << pSubmatrixColumns) + bitValue);

			matrixArray[i].SetRow(rowTemp, lineIndex);
		}

		if (linesCombinations[0] < vectorsQuantity - 1)
		{
			linesCombinations[0]++;
		}
		else
		{
			linesCombinations[0] = 0;
			for (int pLines = 1; pLines < lines; ++pLines)
			{
				if (linesCombinations[pLines] < vectorsQuantity - 1)
				{
					linesCombinations[pLines]++;
					break;
				}
				else
				{
					linesCombinations[pLines] = 0;
				}
			}
		}

		matrixArray[i].CalculateMinCodeDistance();
	}

	delete[] vectorArray;

	delete[] identitySubmatrix;

	delete[] linesCombinations;

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = end - start;

	std::cout << duration.count() << "s" << std::endl;

	/*for (int i = 0; i < matrixQuantity; i++)
	{
		std::cout << matrixArray[i] << "\n\n";
	}*/

	for (int f = 0; f < matrixQuantity; f++)
	{
		if (maxCodeDistance < matrixArray[f].GetCodeDistance())
		{
			maxCodeDistance = matrixArray[f].GetCodeDistance();
		}
	}

	std::cout << "Matrix quantity " << matrixQuantity << "\n";
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

