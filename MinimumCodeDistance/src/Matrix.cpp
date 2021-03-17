//  Defenitions of matrix and vectors classes
#include "pch.h"
#include "Matrix.h"


//  SIMPLE MATRIX CLASS
simpleMatrix::simpleMatrix() {
	newColumns = 0;
	newLines = 0;
	CodeDistance = 0;
}

simpleMatrix::simpleMatrix(short columns, short lines) {

	newColumns = columns;
	newLines = lines;
	CodeDistance = 0;
	__int8 ** a = new __int8*[columns];
	for (short i = 0; i < columns; i++)
		a[i] = new __int8[lines];
	
	for (int i = 0; i<columns; i++)
		for (int j = 0; j<lines; j++)
			a[i][j] = 0;

	newMatrix = a;
}

simpleMatrix::~simpleMatrix() {

	for (int i = 0; i < newColumns; i++) {
		delete[] newMatrix[i];
	}

	delete[] newMatrix;

}


void simpleMatrix::setDimensions(short columns, short lines) {

	newColumns = columns;
	newLines = lines;
	__int8 ** a = new __int8*[columns];
	for (short i = 0; i < columns; i++)
		a[i] = new __int8[lines];

	for (int i = 0; i<columns; i++)
		for (int j = 0; j<lines; j++)
			a[i][j] = 0;

	newMatrix = a;
}


void simpleMatrix::setCodeDistance(short codeDistance) {
	CodeDistance = codeDistance;
}


void simpleMatrix::setCoordinate(__int8 coordinate, short column, short line) {
	newMatrix[column][line] = coordinate;
} 

short simpleMatrix::getColumns() const {
	return newColumns;
}

short simpleMatrix::getLines() const {
	return newLines;
}

__int8& simpleMatrix::getCoordinate(short  column, short  line) const {
	return newMatrix[column][line];
}

short simpleMatrix::getCodeDistance() const {
	return  CodeDistance;
}

void simpleMatrix::getCheckMatrix() const {
	simpleMatrix checkMatrix(newColumns, (newColumns - newLines));

	for (short column = newLines; column < newColumns; column++)
		for (short line = 0; line < newLines; line++)
			checkMatrix.newMatrix[line][column - newLines] = newMatrix[column][line];

	for (short column = newLines; column < newColumns; column++)
		for (short line = 0; line < newLines; line++)
			if (line == (column - newLines))
				checkMatrix.newMatrix[line][column] = 1;
			else
				checkMatrix.newMatrix[line][column] = 0;

}

short simpleMatrix::minCodeDistance(int &checked) const {
	for (short ColumnQuantity = (newColumns - newLines); ColumnQuantity > 0; ColumnQuantity--) {  //здесь загоняем проверку по ColumnQuantity столбцов с помощью метода класса
		if (LinearIndependence(ColumnQuantity, checked))
			return ColumnQuantity + 1;
	}
	return 1;
}

bool simpleMatrix::LinearIndependence(short & ColumnQuantity, int & checked) const {
	//  preparing columns to be pushed into new matrix
	int * c = new int[newColumns];
	int i, n = ColumnQuantity, m = newColumns - 1;
	
	//  making check matrix
	simpleMatrix checkMatrix(newColumns, (newColumns - newLines));

	for (short column = newLines; column < newColumns; column++)
		for (short line = 0; line < newLines; line++)
			checkMatrix.newMatrix[line][column - newLines] = newMatrix[column][line];

	for (short column = newLines; column < newColumns; column++)
		for (short line = 0; line < (newColumns - newLines); line++)
			if (line == (column - newLines)) {
				checkMatrix.newMatrix[column][line] = 1;
			}	
			else {
				checkMatrix.newMatrix[column][line] = 0;
			}
				
			
	simpleMatrix bufferMatrix((newColumns - newLines), ColumnQuantity);

	//  down below is process of making linear combinations of columns and following checking for their linear independence
	for (i = 0; i < n; i++)
		c[i] = n - i - 1;

	while (1) {
		for (i = 0; i < n; i++)
			for (short column = 0; column < (newColumns - newLines); column++)
				bufferMatrix.newMatrix[column][i] = checkMatrix.newMatrix[c[i]][column];
			
		if (bufferMatrix.independence_chech() == false) {
			checked++;
			delete c;
			return false;
		}

		checked++;
		//  need to be checked/controled/examined
		
		for (i = 0;
			c[i] >= m - i;)
			if (++i >= n) {
				delete c;
				return 1;
			}
		for (c[i]++; i; i--)
			c[i - 1] = c[i] + 1;
	}
}

bool simpleMatrix::independence_chech() const {

	int mass[20][20] = { 0 };
	short column, line;
	bool column_state[20] = { 0 };
	
	for (column = 0; column < newColumns; column++)
		for (line = 0; line < newLines; line++)
			mass[column][line] = newMatrix[column][line];

	for (column = 0; column < newColumns; column++)
	{
		for (line = 0; line < newLines; line++)
		{
			if (column_state[line])
				continue;

			if (mass[column][line] == 0)
			{
				for (short i = line + 1; i < newLines; i++)
				{
					if (mass[column][i] != 0)
					{
						// summ vectors
						for (short j = 0; j < newColumns; j++)
							mass[j][line] = (mass[j][line] + mass[j][i]) % 2;


						//  !!! change all line without 0 in curren editing column (start 1: case of first checked line got 0 in current column)
						for (short k = i; k < newLines; k++)
							if (mass[column][k] != 0)
								for (short z = 0; z < newColumns; z++)
									mass[z][k] = (mass[z][line] + mass[z][k]) % 2;
						//  !!! (end of 1)						


						column_state[line] = true;
					}

					if (column_state[line])
						break;
				}
			}
			else
			{
				column_state[line] = true;
				//  !!! change all line without 0 in curren editing column (start 2: case of first checked line got 1 in current column)
				for (short k = line + 1; k < newLines; k++)
					if (mass[column][k] != 0)
						for (short z = 0; z < newColumns; z++)
							mass[z][k] = (mass[z][line] + mass[z][k]) % 2;// проверить как тут всё складывается
																		  //  !!! (end of 2)	
			}

			if (column_state[column])
				break;
		}

		//  !!! change all line without 0 in curren editing column (start 3: in case of non cube matrix) 
		if (column > newLines) {
			for (short i = newLines; i > 0; i--)
			{
				if (mass[column][i] != 0)
					for (short k = i - 1; k >= 0; k--)
						if (mass[column][k] != 0)
							for (short z = 0; z < newColumns; z++)
								mass[z][k] = (mass[z][i] + mass[z][k]) % 2;
			}
		}//  !!! (end of 3)	

	}
	// there should be incapsulated cycles which is checking array for linear independence
	for (short i = 0; i < newLines; i++)
	{
		if (column_state[i] == false)
		{
			return false;
		}
	}

	return true;
}