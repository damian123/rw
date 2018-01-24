#ifndef rw_lapack_doxygen_h__
#define rw_lapack_doxygen_h__

//////////////////////////////////////////////////////////////////////////////
// Documentation related
//////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup lapack_classes Linear Algebra Module
 * @ingroup analysis_classes
 *
 * This module provides a complete, intuitive, object-oriented interface to
 * the entities and algorithms of numerical linear algebra. The algorithms
 * are taken from LAPACK, the most complete procedural linear algebra library
 * available today.
 */

/**
 * @defgroup decomposition_classes Decomposition
 * @ingroup lapack_classes
 *
 * The Decomposition classes allow construction and manipulation of
 * QR, complete orthogonal, and singular value decompositions.
 */

/**
 * @defgroup factorization_classes Factorization
 * @ingroup lapack_classes
 *
 * Factorization objects provide an alternate representation of a system
 * of equations that can be used for solving the system, computing
 * determinants, estimating a condition number, or computing an inverse.
 * Factorizations are provided for the following types of systems: general
 * dense, positive definite symmetric, general symmetric, positive definite
 * Hermitian, general Hermitian, banded, positive definite banded,
 * tridiagonal, and positive definite tridiagonal.
 */

/**
 * @defgroup least_squares_factorization_classes Least Squares Factorization
 * @ingroup lapack_classes
 *
 * The Least Squares Factorization classes allow you to find the best
 * solution to an over-determined or under-determined system of equations.
 */

/**
 * @defgroup symmetric_eigenvalue_decomposition_classes Symmetric Eigenvalue Decomposition
 * @ingroup lapack_classes
 *
 * The Symmetric Eigenvalue Decomposition group provides classes for solving
 * symmetric eigenvalue problems through a number of decompositions and
 * servers in a carefully layered interface.
 */

/**
 * @defgroup nonsymmetric_eigenvalue_decomposition_classes Nonsymmetric Eigenvalue Decomposition
 * @ingroup lapack_classes
 *
 * The Nonsymmetric Eigenvalue Decomposition group provides classes for
 * solving nonsymmetric eigenvalue problems through a number of decompositions
 * and servers in a carefully layered interface. In particular, you can
 * construct and use the Schur and Hessenberg decompositions as easily as the
 * more standard eigenvalue decomposition.
 */

/**
 * @defgroup sparse_matrix_classes Sparse Matrices
 * @ingroup lapack_classes
 *
 * The Sparse Matrices group provides classes for representing a variety of
 * matrix types, including: general, dense symmetric, skew symmetric,
 * Hermitian, banded, symmetric banded, Hermitian banded, upper-triangular,
 * lower-triangular, and tridiagonal.
 */

#endif // rw_lapack_doxygen_h__
