/*
 * =====================================================================================
 * 
 *       Filename:  sparse_matrix.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  12/01/2007 04:12:00 PM EST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Nikolaos Vasiloglou (NV), nvasil@ieee.org
 *        Company:  Georgia Tech Fastlab-ESP Lab
 * 
 * =====================================================================================
 */

#ifndef SPARSE_MATRIX_H_
#define SPARSE_MATRIX_H_
#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H
#endif
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "fastlib/fastlib.h"
#include "fastlib/la/vector.h"
#include "Epetra_CrsMatrix.h"
#include "Epetra_SerialComm.h"
#include "Epetra_Map.h"

class SparseMatrix {
 public:
  SparseMatrix();
	// Constructor, note num_of_rows is always greater or 
	// equal to num_of_columns
	SparseMatrix(index_t num_of_rows, index_t num_of_columns);
	// This constructor creates square matrices
	SparseMatrix(index_t num_of_rows);
	// Copy constructor
  SparseMatrix(const SparseMatrix);
	~SparseMatrix();
	void Destruct();
	// Initializer: nnz_per_row is the  estimated non zero elements per row
  void Init(index_t num_of_rows, index_t num_of_columns, 
			      index_t nnz_per_row);
	// Initializer nnz_per_row has the estimated non zero elements per row
	// notice that it is not the same for every row;
  void Init(index_t num_of_rows, index_t num_of_columns, index_t *nnz_per_row);
	// This initializer takes as an input a vector of the row indices along
	// with the column indices and the corresponding values and creates a sparse
	// matrix. If the dimension (number of rows)and the expected (nnz elements per row)
	// are set to a negative value, the function will automatically detect it 
	void Init(const std::vector<index_t> &row_indices,
		       	const std::vector<index_t> &col_indices,
			      const Vector values, 
			      index_t nnz_per_row, 
			      index_t dimension);
	// Initialize from a text file in the following format
	// row column value \n
	void Init(std::string textfile);
	// Initialize the diagonal 
	void InitDiagonal(const Vector &vec); 
	// Initialize the diagonal with a constant
	void InitDiagonal(const double value);
	void StartLoading();
  void LoadRow();
  void EndLoading();
  void MakeSymetric();	
  void SetAll(double d);
  void SetZero();
  void SetDiagonal(const Vector &vector); 
  void Copy(const SparseMatrix &other);
  void Alias(const Matrix& other);
  void SwapValues(SparseMatrix* other);
  void CopyValues(const SparseMatrix& other);
  double get(index_t r, index_t c);
  void set(index_t r, index_t c, double v);
	void set_non_zero_to_non_zero(index_t r, index_t c, double v);
	void set_zero_to_non_zero(index_t r, index_t c, double v);
  void set_non_zero_to_zero(index_t r, index_t c);
  double &ref(index_t r, index_t c);
  index_t get_dimension();
	index_t get_nnz();
 
 private:
	index_t num_of_rows_;
	index_t num_of_columns_;
	Epetra_SerialComm comm_;
	Epetra_Map *map_;
  Epetra_CrsMatrix *matrix_;
  index_t *MyglobalElements_;
  void AllRowsLoad(Vector &rows, Vector &columns);

};

#include "u/nvasil/sparse_matrix/sparse_matrix_impl.h"
#endif
