#pragma once
#include "pch.h"
#include "Matrix.h"

bool CheckColumnsIndependency(unsigned __int16* columns, unsigned __int8 numberOfColumns);

unsigned __int8 CheckColumnsCombinationsIndependency(unsigned __int16* columns, unsigned __int8 numberOfColumns, unsigned __int8 numberOfRows);

Matrix::Matrix() : _columns(0), _rows(0), _codeDistance(0), _matrix(nullptr)
{
}

Matrix::Matrix(unsigned __int8 columns, unsigned __int8 rows) : _columns(columns), _rows(rows), _codeDistance(0)
{
	if (_columns > 16)
	{
		throw std::invalid_argument("The number of columns should be less than 16");
	}

	if (_rows >= _columns)
	{
		throw std::invalid_argument("The number of lines be less columns");
	}
	_matrix = new unsigned __int16[rows];
}

Matrix::~Matrix()
{
	delete[] _matrix;
}

void Matrix::SetDimensions(unsigned __int8 columns, unsigned __int8 rows)
{
	_columns = columns;
	_rows = rows;

	if (_columns > 16)
	{
		throw std::invalid_argument("The number of columns should be less than 16");
	}

	if (_rows >= _columns)
	{
		throw std::invalid_argument("The number of lines be less columns");
	}

	delete[] _matrix;

	_matrix = new unsigned __int16[rows];
}

void Matrix::CalculateMinCodeDistance()
{
	if (_rows == 0 || _columns == 0)
	{
		throw std::exception("Can't calculate minimum code distance. Matrix have yet been initialized.");
	}
	unsigned __int16* checkMatrix = new unsigned __int16[_columns];

	unsigned __int8 pSubmatrixColumns = _columns - _rows;

#pragma warning(disable:6386)
	for (int i = 0; i < _rows; ++i)//take rows from p submatrix
	{
		unsigned __int16 tempColumn = (1 << pSubmatrixColumns);
		--tempColumn;

		tempColumn = tempColumn & _matrix[i];

		if (tempColumn == 0)
		{
			_codeDistance = 1;
			delete[] checkMatrix;
			return;
		}

		checkMatrix[i] = tempColumn;
	}

	for (int i = 0; i < pSubmatrixColumns; ++i)//creating identity submatrix
	{
		checkMatrix[_rows + i] = (1 << i);
	}
#pragma warning(default:6386)

	if (!::CheckColumnsIndependency(checkMatrix, _columns))
	{
		_codeDistance = 2;
		delete[] checkMatrix;
		return;
	}

	_codeDistance = CheckColumnsCombinationsIndependency(checkMatrix, _columns, _rows);
	delete[] checkMatrix;
}

void Matrix::SetRow(unsigned __int16 value, unsigned __int8 rowIndex)
{
	_matrix[rowIndex] = value;
}

unsigned __int8 Matrix::GetColumnsCount() const
{
	return _columns;
}

unsigned __int8 Matrix::GetRowsCount() const
{
	return _rows;
}

unsigned __int8 Matrix::GetCodeDistance() const
{
	return _codeDistance;
}

unsigned __int8 Matrix::operator()(unsigned row, unsigned column) const
{
	++column;//increase column index to make bitshift one digit smaller, so it stops right before needed number
	if (row >= _rows || column > _columns)
	{
		throw std::invalid_argument("The elment's index out of bounds.");
	}

	unsigned __int8 result = _matrix[row];
	result = result >> (_columns - column);
	return result & 1;
}

unsigned __int16 Matrix::GetRow(unsigned __int8 rowIndex) const
{
	if (rowIndex >= _rows)
	{
		throw std::invalid_argument("The elment's index out of bounds.");
	}

	return _matrix[rowIndex];
}

std::ostream& operator<<(std::ostream& dest, const Matrix& matrix)
{
	const unsigned __int8 numberOfRows = matrix.GetRowsCount();
	const unsigned __int8 numberOfColumns = matrix.GetColumnsCount();

	for (unsigned __int8 i = 0; i < numberOfRows; ++i)
	{
		for (unsigned __int8 j = 0; j < numberOfColumns; ++j)
		{
			if (matrix(i, j))
			{
				dest << '1';
			}
			else
			{
				dest << '0';
			}
		}
		dest << '\n';
	}

	return dest;
}

bool CheckColumnsIndependency(unsigned __int16* columns, unsigned __int8 numberOfColumns)
{
	for (unsigned __int8 i = 0; i < numberOfColumns - 1; ++i)
	{
		for (unsigned __int8 j = i + 1; j < numberOfColumns; ++j)
		{
			if (columns[i] == columns[j])
			{
				return false;
			}
		}
	}

	return true;
}

unsigned __int8 CheckColumnsCombinationsIndependency(unsigned __int16* columns, unsigned __int8 numberOfColumns, unsigned __int8 numberOfRows)
{
	int c[64];
	int i;

	unsigned __int8 upperBound = numberOfColumns - numberOfRows;

	for (unsigned __int8 columnsToSumm = 3; columnsToSumm < upperBound; columnsToSumm++)
	{
		for (i = 0; i < columnsToSumm; i++)// putting least column at last position (like [3,2,1])
		{
			c[i] = columnsToSumm - i;
		}

		while (1)
		{
			unsigned __int16 summ = 0;

			for (i = 0; i < columnsToSumm; i++)
			{
				summ = summ ^ columns[c[i] - 1];
			}

			if (summ == 0)
			{
				return columnsToSumm;
			}

			for (i = 0; c[i] >= numberOfColumns - i;)
			{
				if (++i >= columnsToSumm)//	if index is bigger than number of summing columns
				{
					goto m1; //  go to next iteration of cycle (increase number of summing columns)
				}
			}

			for (c[i]++; i; i--)
			{
				c[i - 1] = c[i] + 1;
			}
		}
	m1: continue;
	}

	return upperBound;
}