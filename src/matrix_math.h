#ifndef MATRIX_MATH_H_
#define MATRIX_MATH_H_

class Matrix {
 public:
  Matrix();
  explicit Matrix(int rows);
  Matrix(int rows, int cols);
  Matrix(const Matrix &other);
  Matrix(Matrix &&other) noexcept;
  ~Matrix();

  int GetRows() const;
  int GetCols() const;
  double GetValue(int row, int col) const;

  void SetSize(int rows, int cols);
  void SetRows(int rows);
  void SetCols(int cols);
  void SetValue(int row, int col, double value);

  bool EqMatrix(const Matrix &other) const;
  void SumMatrix(const Matrix &other);
  void SubMatrix(const Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const Matrix &other);
  Matrix Transpose();
  Matrix CalcComplements();
  double Determinant() const;
  Matrix InverseMatrix();

  Matrix operator+(const Matrix &other) const;
  Matrix operator-(const Matrix &other) const;
  Matrix operator*(const Matrix &other) const;
  Matrix operator*(const double x) const;
  friend Matrix operator*(const double x, const Matrix &other);
  bool operator==(const Matrix &other) const;
  Matrix &operator=(const Matrix &other);
  Matrix &operator=(Matrix &&other) noexcept;
  Matrix &operator+=(const Matrix &other);
  Matrix &operator-=(const Matrix &other);
  Matrix &operator*=(const Matrix &other);
  Matrix &operator*=(double x);
  double &operator()(int row, int col) const;

 private:
  int rows_, cols_;
  double **matrix_;
  void AllocateMatrixMemory();
  void FreeMatrixMemory();
  void NullMatrix();
  void CheckEqMatrixSize(int rows, int cols) const;
  void CheckMatrixSquareSize() const;
  void CheckIndexInRange(int row, int col) const;
};

Matrix operator*(const double x, const Matrix &other);

#endif  // MATRIX_MATH_H_
