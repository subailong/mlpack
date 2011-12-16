// Copyright (C) 2008-2011 NICTA (www.nicta.com.au)
// Copyright (C) 2008-2011 Conrad Sanderson
// 
// This file is part of the Armadillo C++ library.
// It is provided without any warranty of fitness
// for any purpose. You can redistribute this file
// and/or modify it under the terms of the GNU
// Lesser General Public License (LGPL) as published
// by the Free Software Foundation, either version 3
// of the License or (at your option) any later version.
// (see http://www.opensource.org/licenses for more info)


//! \addtogroup fn_min
//! @{

//! Immediate minimums of sparse matrices and/or subviews.
template<typename eT>
arma_inline
arma::vec
min(const SpMat<eT>& x, const uword dim = 0)
  {
  arma_extra_debug_sigprint();

  if(dim == 1)
    {
    arma::vec ret(x.n_rows);
    ret.fill(priv::most_pos<eT>());

    // stupid slow
    for (uword c = 0; c < x.n_cols; ++c)
      {
      for (uword r = 0; r < x.n_rows; ++r)
        {
        const eT val = x.at(r, c);

        if(val < ret[r])
          {
          ret[r] = val;
          }
        }
      }

    return ret; // copies are bad
    }
  else
    {
    arma::vec ret(x.n_cols);
    ret.fill(priv::most_pos<eT>());

    // stupid slow
    for (uword c = 0; c < x.n_cols; ++c)
      {
      for (uword r = 0; r < x.n_rows; ++r)
        {
        const eT val = x.at(r, c);

        if(val < ret[c])
          {
          ret[c] = val;
          }
        }
      }

    return ret;
    }
  }


//! Immediate 'find the minimum value in a row vector' operation
template<typename eT>
inline
arma_warn_unused
eT
min(const SpRow<eT>& A)
  {
  arma_extra_debug_sigprint();
  
  const uword A_n_elem = A.n_elem;
  const uword A_n_nonzero = A.n_nonzero;
  
  arma_debug_check( (A_n_elem == 0), "min(): given object has no elements" );
  
  eT min = ((A_n_elem == A_n_nonzero) ? priv::most_pos<eT>() : 0);

  for(uword i = 0; i < A_n_elem; ++i)
    {
    if(A.values[i] < min)
      {
      min = A.values[i];
      }
    }

  return min;
  }



//! Immediate 'find the minimum value in a column vector'
template<typename eT>
inline
arma_warn_unused
eT
min(const SpCol<eT>& A)
  {
  arma_extra_debug_sigprint();
  
  const uword A_n_elem = A.n_elem;
  const uword A_n_nonzero = A.n_nonzero;
  
  arma_debug_check( (A_n_elem == 0), "min(): given object has no elements" );

  eT min = ((A_n_elem == A_n_nonzero) ? priv::most_pos<eT>() : 0);

  for(uword i = 0; i < A_n_elem; ++i)
    {
    if(A.values[i] < min)
      {
      min = A.values[i];
      }
    }

  return min;
  }


template<typename eT>
inline
arma_warn_unused
arma::vec
min(const SpSubview<eT>& A, const uword dim = 0)
  {
  arma_extra_debug_sigprint();
  
  if(dim == 1)
    {
    arma::vec ret(A.n_rows);
    ret.fill(priv::most_pos<eT>());

    // stupid slow
    for (uword c = 0; c < A.n_cols; ++c)
      {
      for (uword r = 0; r < A.n_rows; ++r)
        {
        const eT val = A.at(r, c);

        if(val < ret[r])
          {
          ret[r] = val;
          }
        }
      }

    return ret; // copies are bad
    }
  else
    {
    arma::vec ret(A.n_cols);
    ret.fill(priv::most_pos<eT>());

    // stupid slow
    for (uword c = 0; c < A.n_cols; ++c)
      {
      for (uword r = 0; r < A.n_rows; ++r)
        {
        const eT val = A.at(r, c);

        if(val < ret[c])
          {
          ret[c] = val;
          }
        }
      }

    return ret;
    }
  }

//! @}
