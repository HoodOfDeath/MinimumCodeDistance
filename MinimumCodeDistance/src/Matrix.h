//  Declarations of matrix and vectors classes
#pragma once
#include <list>

using namespace std;

#ifndef MATRIX_H
#define MATRIX_H



class simpleMatrix {
public:

	//  default constructur
	simpleMatrix();

	//  overload constructur
	simpleMatrix(short, short);

	~simpleMatrix();

	//  set quantity of columns and lines
	void setDimensions(short, short);

	void setCodeDistance(short);

	void setCoordinate(__int8, short, short);

	short getColumns() const;

	short getLines() const;

	__int8& getCoordinate(short , short) const;

	short getCodeDistance() const;

	void getCheckMatrix() const;

	short minCodeDistance(int &) const;

	bool LinearIndependence(short &, int &) const;

	bool independence_chech() const;

private:

	__int8** newMatrix;

	short newColumns, newLines, CodeDistance;

};



#endif
