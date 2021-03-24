// MinimumCodeDistance.cpp : This file contains the 'main' function. Program execution begins and ends there.
#pragma once
#include "pch.h"
#include "Matrix.h"
#include <chrono>
#include <vector>
#include <future>

unsigned __int16* _vectorArray;
unsigned __int16* _identitySubmatrix;
Matrix* _matrixArray;
std::vector<std::future<void>> _futures;

void ConstructMatricesRange(const unsigned short lines, const unsigned short columns, const unsigned int startIndex, const unsigned int count) noexcept
{
	short pSubmatrixColumns(columns - lines);

	unsigned int vectorsQuantity = (1 << pSubmatrixColumns); //  quantity of unique vectors size of "pSubmatrixColumns". Equals to (2 power "pSubmatrixColumns") - 1
	vectorsQuantity--;

	std::vector<unsigned int> linesCombinations;
	linesCombinations.reserve(lines);

	unsigned int remainder = startIndex;

	for (unsigned short i = 0; i < lines; ++i)
	{
		linesCombinations.push_back(remainder % vectorsQuantity);
		remainder = remainder / vectorsQuantity;
	}

	const unsigned int endIndex = startIndex + count;

	// there starts making P submatrix as combinations of precombined vectors
	for (unsigned int i = startIndex; i < endIndex; ++i)//this loot is iterate through all combinations of vectors
	{
		_matrixArray[i].SetDimensions(columns, lines);

		for (unsigned int lineIndex = 0; lineIndex < lines; ++lineIndex)
		{
			unsigned __int16 rowTemp = _identitySubmatrix[lineIndex];
			unsigned int lineVectorIndex = linesCombinations[lineIndex];
			unsigned __int16 bitValue = _vectorArray[lineVectorIndex];
			rowTemp = ((rowTemp << pSubmatrixColumns) + bitValue);

			_matrixArray[i].SetRow(rowTemp, lineIndex);
		}

		_matrixArray[i].CalculateMinCodeDistance();

		if (linesCombinations[0] < vectorsQuantity - 1)
		{
			linesCombinations[0]++;
		}
		else
		{
			linesCombinations[0] = 0;
			for (unsigned int pLines = 1; pLines < lines; ++pLines)
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
	}
}

int main()
{
	unsigned int hardwareThreadsCount = std::thread::hardware_concurrency();
	
	if (hardwareThreadsCount == 0)
	{
		hardwareThreadsCount = 2;
	}

	int codeDistance[10] = { 0 };
	int checked = 0;
	unsigned short lines, columns;
	short maxCodeDistance = 1;

	std::cout << "Enter code parameters (n k): ";
	std::cin >> columns >> lines;

	short pSubmatrixColumns(columns - lines);

	auto start = std::chrono::high_resolution_clock::now();

	unsigned int vectorsQuantity = (1 << pSubmatrixColumns); //  quantity of unique vectors size of "lines". Equals to (2 power "lines") - 1
	vectorsQuantity--;

	_vectorArray = new unsigned __int16 [vectorsQuantity];

	for (unsigned int i = 1; i <= vectorsQuantity; ++i)
	{
		_vectorArray[i - 1] = i;
	}

	unsigned int matrixQuantity = 1;

	_identitySubmatrix = new unsigned __int16[lines];

	for (unsigned int i = 0; i < lines; i++)
	{
		matrixQuantity *= vectorsQuantity;
		_identitySubmatrix[i] = (1 << (lines - i - 1));
	}

	_matrixArray = new Matrix[matrixQuantity];

	const unsigned int baseCount = matrixQuantity / hardwareThreadsCount;
	const unsigned int remainder = matrixQuantity % hardwareThreadsCount;

	std::vector<unsigned int> matrixPerThread;
	matrixPerThread.reserve(hardwareThreadsCount);

	for (unsigned int i = 0; i < hardwareThreadsCount; i++)
	{
		if (i < remainder)
		{
			matrixPerThread.push_back(baseCount + 1);
		}
		else
		{
			matrixPerThread.push_back(baseCount);
		}
	}

	unsigned int startIndex = 0;
	unsigned int endIndex = 0;
	
	_futures.reserve(hardwareThreadsCount);

	for (unsigned int i = 0; i < hardwareThreadsCount; i++)
	{
		startIndex = endIndex;
		endIndex += matrixPerThread[i];
		_futures.push_back(std::async(std::launch::async, ConstructMatricesRange, lines, columns, startIndex, matrixPerThread[i]));
	}

	for (unsigned int i = 0; i < hardwareThreadsCount; i++)
	{
		_futures[i].wait();
	}

	delete[] _vectorArray;

	delete[] _identitySubmatrix;

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = end - start;

	std::cout << duration.count() << "s" << std::endl;

	/*for (int i = 0; i < matrixQuantity; i++)
	{
		std::cout << matrixArray[i] << "\n\n";
	}*/

	for (unsigned int f = 0; f < matrixQuantity; f++)
	{
		if (maxCodeDistance < _matrixArray[f].GetCodeDistance())
		{
			maxCodeDistance = _matrixArray[f].GetCodeDistance();
		}
	}

	std::cout << "Matrix quantity " << matrixQuantity << "\n";
	std::cout << "Max code distance is " << maxCodeDistance << "\n";

	for (unsigned int i = 0; i < matrixQuantity; i++)
	{
		int D = _matrixArray[i].GetCodeDistance();

		codeDistance[D - 1]++;
	}

	for (short i = 0; i < maxCodeDistance; i++)
	{
		std::cout << "There are " << codeDistance[i] << " matrix with code distance " << i + 1 << "\n";
	}

	system("pause");

	return 0;
}

