//  Defenitions of matrix and vectors classes
#include "pch.h"
#include "Matrix.h"


//  SIMPLE MATRIX CLASS
Matrix::Matrix()
{
	newColumns = 0;
	newLines = 0;
	CodeDistance = 0;
}

Matrix::Matrix(short columns, short lines)
{
	newColumns = columns;
	newLines = lines;
	CodeDistance = 0;

	__int8** a = new __int8* [columns];

	for (short i = 0; i < columns; i++)
	{
		a[i] = new __int8[lines];
	}

	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < lines; j++)
		{
			a[i][j] = 0;
		}
	}

	newMatrix = a;
}

Matrix::~Matrix()
{
	for (int i = 0; i < newColumns; i++)
	{
		delete[] newMatrix[i];
	}

	delete[] newMatrix;

}


void Matrix::SetDimensions(short columns, short lines)
{
	newColumns = columns;
	newLines = lines;

	__int8** a = new __int8* [columns];

	for (short i = 0; i < columns; i++)
	{
		a[i] = new __int8[lines];
	}

	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < lines; j++)
		{
			a[i][j] = 0;
		}
	}

	newMatrix = a;
}


void Matrix::SetCodeDistance(short codeDistance)
{
	CodeDistance = codeDistance;
}


void Matrix::SetCoordinate(__int8 coordinate, short column, short line)
{
	newMatrix[column][line] = coordinate;
}

short Matrix::GetColumns() const
{
	return newColumns;
}

short Matrix::GetLines() const
{
	return newLines;
}

__int8& Matrix::GetCoordinate(short  column, short  line) const
{
	return newMatrix[column][line];
}

short Matrix::GetCodeDistance() const
{
	return  CodeDistance;
}

void Matrix::GetCheckMatrix() const
{
	Matrix checkMatrix(newColumns, (newColumns - newLines));

	for (short column = newLines; column < newColumns; column++)
	{
		for (short line = 0; line < newLines; line++)
		{
			checkMatrix.newMatrix[line][column - newLines] = newMatrix[column][line];
		}
	}

	for (short column = newLines; column < newColumns; column++)
	{
		for (short line = 0; line < newLines; line++)
		{
			if (line == (column - newLines))
			{
				checkMatrix.newMatrix[line][column] = 1;
			}
			else
			{
				checkMatrix.newMatrix[line][column] = 0;
			}
		}
	}

}

short Matrix::MinCodeDistance(int& checked) const
{
	for (short ColumnQuantity = (newColumns - newLines); ColumnQuantity > 0; ColumnQuantity--)
	{  //здесь загоняем проверку по ColumnQuantity столбцов с помощью метода класса
		if (LinearIndependence(ColumnQuantity, checked))
		{
			return ColumnQuantity + 1;
		}
	}
	return 1;
}

bool Matrix::LinearIndependence(short& ColumnQuantity, int& checked) const
{
	//  preparing columns to be pushed into new matrix
	int* c = new int[newColumns];
	int i, n = ColumnQuantity, m = newColumns - 1;

	//  making check matrix
	Matrix checkMatrix(newColumns, (newColumns - newLines));

	for (short column = newLines; column < newColumns; column++)
	{
		for (short line = 0; line < newLines; line++)
		{
			checkMatrix.newMatrix[line][column - newLines] = newMatrix[column][line];
		}
	}

	for (short column = newLines; column < newColumns; column++)
	{
		for (short line = 0; line < (newColumns - newLines); line++)
		{
			if (line == (column - newLines))
			{
				checkMatrix.newMatrix[column][line] = 1;
			}
			else
			{
				checkMatrix.newMatrix[column][line] = 0;
			}
		}
	}


	Matrix bufferMatrix((newColumns - newLines), ColumnQuantity);

	//  down below is process of making linear combinations of columns and following checking for their linear independence
	for (i = 0; i < n; i++)
	{
		c[i] = n - i - 1;
	}

	while (1)
	{
		for (i = 0; i < n; i++)
		{
			for (short column = 0; column < (newColumns - newLines); column++)
			{
				bufferMatrix.newMatrix[column][i] = checkMatrix.newMatrix[c[i]][column];
			}
		}

		if (bufferMatrix.IndependenceChech() == false)
		{
			checked++;
			delete c;
			return false;
		}

		checked++;
		//  need to be checked/controled/examined

		for (i = 0; c[i] >= m - i;)
		{
			if (++i >= n)
			{
				delete c;
				return 1;
			}
		}
		for (c[i]++; i; i--)
		{
			c[i - 1] = c[i] + 1;
		}
	}
}

bool Matrix::IndependenceChech() const
{

	int mass[20][20] = { 0 };
	short column, line;
	bool columnSate[20] = { 0 };

	for (column = 0; column < newColumns; column++)
	{
		for (line = 0; line < newLines; line++)
		{
			mass[column][line] = newMatrix[column][line];
		}
	}

	for (column = 0; column < newColumns; column++)
	{
		for (line = 0; line < newLines; line++)
		{
			if (columnSate[line])
			{
				continue;
			}

			if (mass[column][line] == 0)
			{
				for (short i = line + 1; i < newLines; i++)
				{
					if (mass[column][i] != 0)
					{
						// summ vectors
						for (short j = 0; j < newColumns; j++)
						{
							mass[j][line] = (mass[j][line] + mass[j][i]) % 2;
						}


						//  !!! change all line without 0 in curren editing column (start 1: case of first checked line got 0 in current column)
						for (short k = i; k < newLines; k++)
						{
							if (mass[column][k] != 0)
							{
								for (short z = 0; z < newColumns; z++)
								{
									mass[z][k] = (mass[z][line] + mass[z][k]) % 2;
								}
							}
						}
						//  !!! (end of 1)						

						columnSate[line] = true;
					}

					if (columnSate[line])
					{
						break;
					}
				}
			}
			else
			{
				columnSate[line] = true;
				//  !!! change all line without 0 in curren editing column (start 2: case of first checked line got 1 in current column)
				for (short k = line + 1; k < newLines; k++)
				{
					if (mass[column][k] != 0)
					{
						for (short z = 0; z < newColumns; z++)
						{
							mass[z][k] = (mass[z][line] + mass[z][k]) % 2;// проверить как тут всё 
						}
					}
				}
																		  //  !!! (end of 2)	
			}

			if (columnSate[column])
			{
				break;
			}
		}

		//  !!! change all line without 0 in curren editing column (start 3: in case of non cube matrix) 
		if (column > newLines)
		{
			for (short i = newLines; i > 0; i--)
			{
				if (mass[column][i] != 0)
				{
					for (short k = i - 1; k >= 0; k--)
					{
						if (mass[column][k] != 0)
						{
							for (short z = 0; z < newColumns; z++)
							{
								mass[z][k] = (mass[z][i] + mass[z][k]) % 2;
							}
						}
					}
				}
			}
		}//  !!! (end of 3)	

	}
	// there should be incapsulated cycles which is checking array for linear independence
	for (short i = 0; i < newLines; i++)
	{
		if (columnSate[i] == false)
		{
			return false;
		}
	}

	return true;
}