// MinimumCodeDistance.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <string>
#include "Matrix.h"

using namespace std;

int factorial(short value) {
	int result = 1;
	if (value == 0)
		result = 1;
	else
	{
		for (short i = 1; i <= value; i++)
			result *= i;
	}
	return result;
}

unsigned __int64 Factorial(short value) {
	unsigned __int64 result = 1;
	if (value == 0)
		result = 1;
	else
	{
		for (short i = 1; i <= value; i++)
			result *= i;
	}
	return result;
}

unsigned __int64 Cnk(int n, short k) {
	unsigned __int64 result = 1;
	unsigned __int64 denominator = 1;
	if (n >= k) {
		if (k >= (n - k)) {
			for (int i = k + 1; i <= n; i++)
				result *= i;

			for (int i = 1; i <= (n - k); i++)
				denominator *= i;

			result /= denominator;
		}
		else {
			for (int i = (n - k) + 1; i <= n; i++)
				result *= i;

			for (int i = 1; i <= k; i++)
				denominator *= i;

			result /= denominator;
		}

	}

	else
		result = 1;
	return result;
}

int main()
{
	const int one = 1;
	const int zero = 0;

	int codeDistance[10] = { 0 };
	int checked = 0;
	short q, lines, columns;
	short maxCodeDistance = 1;
	string str;
	//cout << "Enter demension of the field (q): ";
	//cin >> q;
	/*cout << "Enter demensions of the matrix (k n): ";
	cin >> lines >> columns;
	matrix matrix1(columns, lines);
	cout << "Enter enter matrix as shown \n1 0 1 1 0\n1 0 0 1 1\n1 1 0 0 1\nH:\n";
	getchar();
	for (short i = 0; i < lines; i++) {
		getline(std::cin, str);
		matrix1.setVector(str, i);
	}

	cout << "Matrix min code distance is " << matrix1.minCodeDistance(checked);
	matrix1.setChecked(checked);
	cout << ". " << matrix1.getChecked() << " matrixs were ckecked" << "\n";
	*/

	//  There will be 4'th lab


	cout << "Enter code parameters (n k): ";
	cin >> columns >> lines;

	int vectorsQuantity = 0; //  quantity of unique vectors size of "lines"

	for (short i = 1; i <= lines; i++)
		vectorsQuantity += (factorial(lines)) / (factorial(i) * factorial(lines - i));

	int** vectorArray = new int* [lines];

	for (short i = 0; i < lines; i++)
		vectorArray[i] = new int[vectorsQuantity];

	for (short i = 0; i < lines; i++)
		for (int j = 0; j < vectorsQuantity; j++)
			vectorArray[i][j] = 0;

	//  creating unique vectors

	int startPos;
	int endPos = 0;

	for (short k = 1; k <= lines; k++) {
		startPos = endPos;
		endPos += factorial(lines) / (factorial(k) * (factorial(lines - k)));
		for (short vect = startPos; vect < endPos; vect++) {
			int c[100];
			int i;
			for (i = 0; i < k; i++)//n = k
				c[i] = k - i;

			while (1) {
				for (i = 0; i < k; i++)
					vectorArray[c[i] - 1][vect] = 1;

				for (i = 0; c[i] >= lines - i;)// m = lines
					if (++i >= k)
						goto m1; //  some kind of "return"
				for (c[i]++; i; i--)
					c[i - 1] = c[i] + 1;

				vect++;
			}
		}
	m1: continue;
	}

	unsigned __int64 matrixQuantity = 1;


	for (int i = 0; i < columns - lines; i++)
		matrixQuantity *= vectorsQuantity;


	simpleMatrix* matrixArray = new simpleMatrix[matrixQuantity];

	for (int i = 0; i < matrixQuantity; i++)
		matrixArray[i].setDimensions(columns, lines);

	int i, n, m;
	int buffMatrix = 0;
	int c[100];

	short* columnsCombinations = new short[columns - lines]; // columnsCombinations[vectorsQuantity][matrixQuantity]
	// there starts making P submatrix as combinations of precobined vectors


	for (int i = 0; i <= (columns - lines); i++)
		columnsCombinations[i] = 0;

	for (int i = 0; i < matrixQuantity; i++) {

		for (int k = 0; k < (columns - lines); k++)
			for (int f = 0; f < lines; f++)
				matrixArray[i].setCoordinate(vectorArray[f][columnsCombinations[k]], k + lines, f);

		columnsCombinations[0]++;

		for (int j = 0; columnsCombinations[j] >= vectorsQuantity;) {
			if (j < (columns - lines)) {

				columnsCombinations[j] = 0;
				j++;
				columnsCombinations[j]++;

			}
		}


	}


	//  making E submatrix
	for (int i = 0; i < matrixQuantity; i++)
		for (short line = 0; line < lines; line++)
			for (short column = 0; column < lines; column++)
				if (column == line)
					matrixArray[i].setCoordinate(1, column, line);
				else
					matrixArray[i].setCoordinate(0, column, line);
	// end of making matrix

	//cout << "\n \n";


	for (int matrixCount = 0; matrixCount < matrixQuantity; matrixCount++) {
		/*
		for (short line = 0; line < lines; line++) {
			for (short column = 0; column < columns; column++)
				cout << matrixArray[matrixCount].getCoordinate(column, line);
			cout << "\n";
		}
		cout << "\n";
*/
		matrixArray[matrixCount].setCodeDistance(matrixArray[matrixCount].minCodeDistance(checked));
		//	cout << matrixArray[matrixCount].getCodeDistance();

		//	cout << "\n";
	}

	for (int f = 0; f < matrixQuantity; f++)
		if (maxCodeDistance < matrixArray[f].getCodeDistance())
			maxCodeDistance = matrixArray[f].getCodeDistance();

	cout << "Max code distance is " << maxCodeDistance << "\n";

	for (int i = 0; i < matrixQuantity; i++) {

		int D = matrixArray[i].getCodeDistance();

		codeDistance[D - 1]++;
	}

	for (short i = 0; i < maxCodeDistance; i++)
		cout << "There are " << codeDistance[i] << " matrix with code distance " << i + 1 << "\n";


	system("pause");

	return 0;
}

