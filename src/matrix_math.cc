#include "matrix_math.h"

#include <stdexcept>

void Matrix::AllocateMatrixMemory() {
  matrix_ = new double *[rows_];
  for (auto i = 0; i < rows_; i++) matrix_[i] = new double[cols_]();
}

void Matrix::FreeMatrixMemory() {
  if (matrix_) {
    for (int i = 0; i < rows_; i++)
      if (matrix_[i]) delete[] matrix_[i];
    delete[] matrix_;
  }
}

void Matrix::NullMatrix() {
  rows_ = 0;
  cols_ = 0;
  matrix_ = nullptr;
}

void Matrix::CheckEqMatrixSize(int rows, int cols) const {
  if (rows_ != rows || cols_ != cols)
    throw std::out_of_range(
        "Incorrect input, matrices should have the same size");
}

void Matrix::CheckMatrixSquareSize() const {
  if (rows_ != cols_)
    throw std::out_of_range("Incorrect input, matrice should be square");
}

void Matrix::CheckIndexInRange(int row, int col) const {
  if (row >= rows_ || col >= cols_ || row < 0 || col < 0)
    throw std::out_of_range("Incorrect input, index is out of range");
}

Matrix::Matrix() { NullMatrix(); }

Matrix::Matrix(int rows) : Matrix(rows, rows) {}

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ <= 0 || cols_ <= 0)
    throw std::out_of_range("Incorrect input, index is out of range");
  AllocateMatrixMemory();
}

Matrix::Matrix(const Matrix &other) : Matrix(other.rows_, other.cols_) {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] = other.matrix_[i][j];
}

Matrix::Matrix(Matrix &&other) noexcept
    : rows_(other.rows_), cols_(other.cols_) {
  matrix_ = other.matrix_;
  other.NullMatrix();
}

Matrix::~Matrix() {
  FreeMatrixMemory();
  NullMatrix();
}

int Matrix::GetRows() const { return rows_; }

int Matrix::GetCols() const { return cols_; }

double Matrix::GetValue(int row, int col) const {
  CheckIndexInRange(row, col);
  return matrix_[row][col];
}

void Matrix::SetSize(int rows, int cols) {
  Matrix tmp(rows, cols);
  for (int i = 0; i < tmp.rows_; i++)
    for (int j = 0; j < tmp.cols_; j++)
      tmp.matrix_[i][j] = (i < rows_ && j < cols_) ? matrix_[i][j] : 0.0;
  *this = tmp;
}

void Matrix::SetRows(int rows) { SetSize(rows, cols_); }

void Matrix::SetCols(int cols) { SetSize(rows_, cols); }

void Matrix::SetValue(int row, int col, double value) {
  CheckIndexInRange(row, col);
  matrix_[row][col] = value;
}

bool Matrix::EqMatrix(const Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;
  bool result = true;
  for (auto i = 0; result && (i < rows_); i++)
    for (auto j = 0; result && (j < cols_); j++) {
      double tmp = (matrix_[i][j] - other.matrix_[i][j]) > 0
                       ? (matrix_[i][j] - other.matrix_[i][j])
                       : (other.matrix_[i][j] - matrix_[i][j]);
      if (tmp > 1.E-14) result = false;
    }
  return result;
}

void Matrix::SumMatrix(const Matrix &other) {
  CheckEqMatrixSize(other.cols_, other.rows_);
  for (auto i = 0; i < rows_; i++)
    for (auto j = 0; j < cols_; j++) matrix_[i][j] += other.matrix_[i][j];
}

void Matrix::SubMatrix(const Matrix &other) {
  CheckEqMatrixSize(other.cols_, other.rows_);
  for (auto i = 0; i < rows_; i++)
    for (auto j = 0; j < cols_; j++) matrix_[i][j] -= other.matrix_[i][j];
}

void Matrix::MulNumber(const double num) {
  for (auto i = 0; i < rows_; i++)
    for (auto j = 0; j < cols_; j++) matrix_[i][j] *= num;
}

void Matrix::MulMatrix(const Matrix &other) {
  if (cols_ != other.rows_)
    throw std::out_of_range(
        "Incorrect input, matrice columns "
        "should have the same size with other matrice rows");
  Matrix tmp(rows_, other.cols_);
  for (auto i = 0; i < rows_; i++)
    for (auto j = 0; j < other.cols_; j++)
      for (auto k = 0; k < cols_; k++)
        tmp.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
  SetCols(other.cols_);
  for (auto i = 0; i < rows_; i++)
    for (auto j = 0; j < cols_; j++) matrix_[i][j] = tmp.matrix_[i][j];
}

Matrix Matrix::Transpose() {
  Matrix result(cols_, rows_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) result.matrix_[j][i] = matrix_[i][j];
  return result;
}

Matrix Matrix::CalcComplements() {
  CheckMatrixSquareSize();
  Matrix result(rows_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) {
      Matrix tmp(cols_ - 1, rows_ - 1);
      int t_i = 0;
      for (int k = 0; k < tmp.rows_; k++) {
        int t_j = 0;
        if (k == i) t_i++;
        for (int l = 0; l < tmp.cols_; l++) {
          if (l == j) t_j++;
          tmp.matrix_[k][l] = matrix_[k + t_i][l + t_j];
        }
      }
      result.matrix_[i][j] = (((i + j) % 2) ? -1 : 1) * tmp.Determinant();
    }
  return result;
}

double Matrix::Determinant() const {
  CheckMatrixSquareSize();
  double result = 0;
  if (cols_ > 2) {
    for (int i = 0; i < rows_; i++) {
      Matrix tmp(cols_ - 1, rows_ - 1);
      int t_i = 0;
      for (int k = 0; k < tmp.rows_; k++) {
        if (k == i) t_i++;
        for (int l = 0; l < tmp.cols_; l++) {
          tmp.matrix_[k][l] = matrix_[k + t_i][l + 1];
        }
      }
      result += ((i % 2) ? -1 : 1) * matrix_[i][0] * tmp.Determinant();
    }
  } else if (cols_ == 2) {
    result = matrix_[0][0] * matrix_[1][1] - matrix_[1][0] * matrix_[0][1];
  } else if (cols_ == 1) {
    result = matrix_[0][0];
  }
  return result;
}

Matrix Matrix::InverseMatrix() {
  CheckMatrixSquareSize();
  double det = Determinant();
  if (det != det || (det < 1.E-14 && det > -1E-14))
    throw std::out_of_range("Incorrect input, determinant should be not zero");
  det = 1.0 / det;
  Matrix result = CalcComplements();
  result = result.Transpose();
  result.MulNumber(det);
  return result;
}

Matrix Matrix::operator+(const Matrix &other) const {
  Matrix res(*this);
  res.SumMatrix(other);
  return res;
}

Matrix &Matrix::operator=(const Matrix &other) {
  if (this == &other) return *this;
  FreeMatrixMemory();
  cols_ = other.cols_;
  rows_ = other.rows_;
  AllocateMatrixMemory();
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] = other.matrix_[i][j];
  return *this;
}

Matrix &Matrix::operator=(Matrix &&other) noexcept {
  if (this == &other) return *this;
  FreeMatrixMemory();
  cols_ = other.cols_;
  rows_ = other.rows_;
  matrix_ = other.matrix_;
  other.NullMatrix();
  return *this;
}

bool Matrix::operator==(const Matrix &other) const { return EqMatrix(other); }

Matrix &Matrix::operator+=(const Matrix &other) {
  SumMatrix(other);
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
  SubMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
  MulMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(double x) {
  MulNumber(x);
  return *this;
}

Matrix Matrix::operator-(const Matrix &other) const {
  Matrix res(*this);
  res.SubMatrix(other);
  return res;
}

Matrix Matrix::operator*(const Matrix &other) const {
  Matrix res(*this);
  res.MulMatrix(other);
  return res;
}

Matrix Matrix::operator*(const double x) const {
  Matrix res(*this);
  res.MulNumber(x);
  return res;
}

Matrix operator*(const double x, const Matrix &other) {
  Matrix result(other);
  result.MulNumber(x);
  return result;
}

double &Matrix::operator()(int row, int col) const {
  CheckIndexInRange(row, col);
  return matrix_[row][col];
}
