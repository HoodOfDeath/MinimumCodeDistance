//  Declarations of matrix and vectors classes
class Matrix
{
public:

	//  default constructur
	Matrix();

	//  overload constructur
	Matrix(unsigned __int8 columns, unsigned __int8 rows);

	~Matrix();

	void SetDimensions(unsigned __int8 columns, unsigned __int8 rows);

	void SetRow(unsigned __int16 value, unsigned __int8 rowIndex);

	void CalculateMinCodeDistance();

	unsigned __int16 GetRow(unsigned __int8 rowIndex) const;

	unsigned __int8 GetColumnsCount() const;

	unsigned __int8 GetRowsCount() const;

	unsigned __int8 GetCodeDistance() const;

	unsigned __int8  operator()(unsigned row, unsigned column) const;

private:

	unsigned __int8 _columns, _rows, _codeDistance;

	unsigned __int16* _matrix;
};

std::ostream& operator<<(std::ostream& dest, const Matrix& matrix);