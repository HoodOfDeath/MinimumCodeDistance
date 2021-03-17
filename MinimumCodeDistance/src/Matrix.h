//  Declarations of matrix and vectors classes
class Matrix
{
public:

	//  default constructur
	Matrix();

	//  overload constructur
	Matrix(short, short);

	~Matrix();

	//  set quantity of columns and lines
	void SetDimensions(short, short);

	void SetCodeDistance(short);

	void SetCoordinate(__int8, short, short);

	short GetColumns() const;

	short GetLines() const;

	__int8& GetCoordinate(short, short) const;

	short GetCodeDistance() const;

	void GetCheckMatrix() const;

	short MinCodeDistance(int&) const;

	bool LinearIndependence(short&, int&) const;

	bool IndependenceChech() const;

private:

	__int8** newMatrix;

	short newColumns, newLines, CodeDistance;

};