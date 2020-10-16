// diag.h - Diagonalisation code
// Copyright (C) 2009-2020 Rok Zitko

#ifndef _diag_h_
#define _diag_h_

#define LAPACK_COMPLEX_STRUCTURE
#include "lapack.h"

template<typename T, typename V> void copy(T* eigenvalues, ublas::vector<V>& diagvalues, const size_t M) {
  if (std::adjacent_find(eigenvalues, eigenvalues + M, std::greater<T>()) != eigenvalues + M)
    cout << "WARNING: Values are not in ascending order. Bug in LAPACK dsyev* routines." << endl;
  diagvalues.resize(M);
  copy(eigenvalues, eigenvalues + M, begin(diagvalues));
}

template<typename T, typename V> void copy(T* eigenvectors, ublas::matrix<V>& diagvectors, const size_t dim, const size_t M)
{
  diagvectors.resize(M, dim);
  for (const auto r: range0(M))
    for (const auto j: range0(dim))
      diagvectors(r, j) = eigenvectors[dim * r + j];
}

template<> void copy<lapack_complex_double, cmpl>(lapack_complex_double * eigenvectors, ublas::matrix<cmpl>& diagvectors, size_t dim, size_t M)
{
  diagvectors.resize(M, dim);
  for (const auto r: range0(M))
    for (const auto j: range0(dim)) {
      lapack_complex_double v = eigenvectors[dim * r + j];
      diagvectors(r, j) = cmpl(v.real, v.imag);
    }
}

template<typename T, typename U> Eigen copy_results(T* eigenvalues, U* eigenvectors, char jobz, size_t dim, size_t M)
{
  Eigen d(M, dim);
  copy(eigenvalues, d.value_orig, M);
  if (jobz == 'V')
    copy(eigenvectors, d.matrix, dim, M);
  my_assert(d.value_orig.size() == d.matrix.size1());
  return d;
}

// Perform diagonalisation: wrappers for LAPACK
// m: matrix to be diagonalised
// diag: eigenvalues and eigenvectors
// jobz: 'N' for values only, 'V' for values and vectors

#ifdef NRG_REAL
Eigen diagonalise_dsyev(ublas::matrix<double> &m, char jobz = 'V') {
  const size_t dim = m.size1();
  t_matel *ham = bindings::traits::matrix_storage(m);
  t_eigen eigenvalues[dim]; // eigenvalues on exit
  char UPLO  = 'L';         // lower triangle of a is stored
  int NN     = dim;         // the order of the matrix
  int LDA    = dim;         // the leading dimension of the array a
  int INFO   = 0;           // 0 on successful exit
  int LWORK0 = -1;          // length of the WORK array
  double WORK0[1];
  // Step 1: determine optimal LWORK
  LAPACK_dsyev(&jobz, &UPLO, &NN, ham, &LDA, (double *)eigenvalues, WORK0, &LWORK0, &INFO);
  my_assert(INFO == 0);
  int LWORK    = int(WORK0[0]);
  auto WORK = std::make_unique<double[]>(LWORK);
  // Step 2: perform the diagonalisation
  LAPACK_dsyev(&jobz, &UPLO, &NN, ham, &LDA, (double *)eigenvalues, WORK.get(), &LWORK, &INFO);
  if (INFO != 0) throw std::runtime_error(fmt::format("dsyev failed. INFO={}", INFO));
  return copy_results(eigenvalues, ham, jobz, dim, dim);
}
#endif

#ifdef NRG_REAL
Eigen diagonalise_dsyevd(ublas::matrix<double> &m, char jobz = 'V')
{
  const size_t dim = m.size1();
  t_matel *ham = bindings::traits::matrix_storage(m);
  t_eigen eigenvalues[dim];
  char UPLO  = 'L';
  int NN     = dim;
  int LDA    = dim;
  int INFO   = 0;
  int LWORK  = -1;
  int LIWORK = -1;
  double WORK0[1];
  int IWORK0[1];
  LAPACK_dsyevd(&jobz, &UPLO, &NN, ham, &LDA, (double *)eigenvalues, WORK0, &LWORK,
                IWORK0, &LIWORK, &INFO);
  my_assert(INFO == 0);
  LWORK = int(WORK0[0]);
  LIWORK = IWORK0[0];
  auto WORK = std::make_unique<double[]>(LWORK);
  auto IWORK = std::make_unique<int[]>(LIWORK);
  LAPACK_dsyevd(&jobz, &UPLO, &NN, ham, &LDA, (double *)eigenvalues, WORK.get(), &LWORK,
                IWORK.get(), &LIWORK, &INFO);
  if (INFO != 0) {
    // dsyevd sometimes fails to converge (INFO>0). In such cases we do not trigger
    // an error but return 0, to permit error recovery.
    if (INFO > 0)
      return Eigen();
    else
      throw std::runtime_error(fmt::format("dsyev failed. INFO={}", INFO));
  }
  return copy_results(eigenvalues, ham, jobz, dim, dim);
}
#endif  

#ifdef NRG_REAL
Eigen diagonalise_dsyevr(ublas::matrix<double> &m, double ratio = 1.0,  char jobz = 'V')
{
  const size_t dim = m.size1();
  // M is the number of the eigenvalues that we will attempt to
  // calculate using dsyevr.
  size_t M = dim;
  char RANGE; // 'A'=all, 'V'=interval, 'I'=part
  if (ratio != 1.0) {
    M     = static_cast<size_t>(ceil(ratio * M)); // round up
    M     = std::clamp<size_t>(M, 1, dim);        // at least 1, at most dim
    RANGE = 'I';
  } else
    RANGE = 'A';
  t_matel *ham = bindings::traits::matrix_storage(m);
  t_eigen eigenvalues[dim]; // eigenvalues on exit
  char UPLO     = 'L';      // lower triangle of a is stored
  int NN        = dim;      // the order of the matrix
  int LDA       = dim;      // the leading dimension of the array a
  int INFO      = 0;        // 0 on successful exit
  double VL     = 0;        // value range; not referenced if RANGE != 'V'
  double VU     = 0;
  int IL        = 1; // index range
  int IU        = M;
  double ABSTOL = 0;
  // If ABSTOL=0, EPS*|T| where |T| is the 1-norm of the tridiagonal
  // matrix obtained by reducing m to tridiagonal form.
  int MM; // total number of eigenvalues found
  int LDZ = dim;
  int ISUPPZ[2 * M];
  //  The support of the eigenvectors in Z, i.e., the indices
  //  indicating the nonzero elements in Z.  The i-th eigenvector is
  //  nonzero only in elements ISUPPZ( 2*i-1 ) through ISUPPZ(2*i).
  auto Z = std::make_unique<t_matel[]>(LDZ * M); // eigenvectors
  int LWORK0  = -1;
  int LIWORK0 = -1;
  double WORK0[1];
  int IWORK0[1];
  // Step 1: determine optimal LWORK and LIWORK
  LAPACK_dsyevr(&jobz, &RANGE, &UPLO, &NN, ham, &LDA, &VL, &VU, &IL, &IU, &ABSTOL, &MM, (double *)eigenvalues, Z.get(), &LDZ, ISUPPZ, WORK0, &LWORK0,
                IWORK0, &LIWORK0, &INFO);
  my_assert(INFO == 0);
  int LWORK    = int(WORK0[0]);
  int LIWORK   = IWORK0[0];
  auto WORK = std::make_unique<double[]>(LWORK);
  auto IWORK  = std::make_unique<int[]>(LIWORK);
  // Step 2: perform the diagonalisation
  LAPACK_dsyevr(&jobz, &RANGE, &UPLO, &NN, ham, &LDA, &VL, &VU, &IL, &IU, &ABSTOL, &MM, (double *)eigenvalues, Z.get(), &LDZ, ISUPPZ, WORK.get(), &LWORK,
                IWORK.get(), &LIWORK, &INFO);
  if (INFO != 0) throw std::runtime_error(fmt::format("dsyev failed. INFO={}", INFO));
  if (MM != int(M)) {
    cout << "dsyevr computed " << MM << "/" << M << endl;
    M = MM;
    my_assert(M > 0); // at least one
  }
  return copy_results(eigenvalues, Z.get(), jobz, dim, M);
}
#endif

#ifdef NRG_COMPLEX
Eigen diagonalise_zheev(ublas::matrix<cmpl> &m, char jobz = 'V') {
  const size_t dim = m.size1();
  lapack_complex_double *ham = (lapack_complex_double*)bindings::traits::matrix_storage(m);
  t_eigen eigenvalues[dim]; // eigenvalues on exit
  char UPLO  = 'L';         // lower triangle of a is stored
  int NN     = dim;         // the order of the matrix
  int LDA    = dim;         // the leading dimension of the array a
  int INFO   = 0;           // 0 on successful exit
  int LWORK0 = -1;          // length of the WORK array (-1 == query!)
  lapack_complex_double WORK0[1];
  int RWORKdim = max(1ul, 3 * dim - 2);
  double RWORK[RWORKdim];
  // Step 1: determine optimal LWORK
  LAPACK_zheev(&jobz, &UPLO, &NN, ham, &LDA, (double *)eigenvalues, WORK0, &LWORK0, RWORK, &INFO);
  my_assert(INFO == 0);
  int LWORK  = int(WORK0[0].real);
  auto WORK = std::make_unique<lapack_complex_double[]>(LWORK);
  // Step 2: perform the diagonalisation
  LAPACK_zheev(&jobz, &UPLO, &NN, ham, &LDA, (double *)eigenvalues, WORK.get(), &LWORK, RWORK, &INFO);
  if (INFO != 0) throw std::runtime_error(fmt::format("dsyev failed. INFO={}", INFO));
  return copy_results(eigenvalues, ham, jobz, dim, dim);
}
#endif
  
#ifdef NRG_COMPLEX
Eigen diagonalise_zheevr(ublas::matrix<cmpl> &m, double ratio = 1.0, char jobz = 'V') {
  const size_t dim = m.size1();
  // M is the number of the eigenvalues that we will attempt to
  // calculate using zheevr.
  size_t M = dim;
  char RANGE; // 'A'=all, 'V'=interval, 'I'=part
  if (ratio != 1.0) {
    M     = static_cast<size_t>(ceil(ratio * M)); // round up
    M     = std::clamp<size_t>(M, 1, dim);        // at least 1, at most dim
    RANGE = 'I';
  } else
    RANGE = 'A';
  lapack_complex_double *ham = (lapack_complex_double*)bindings::traits::matrix_storage(m);
  t_eigen eigenvalues[dim]; // eigenvalues on exit
  char UPLO     = 'L';      // lower triangle of a is stored
  int NN        = dim;      // the order of the matrix
  int LDA       = dim;      // the leading dimension of the array a
  int INFO      = 0;        // 0 on successful exit
  double VL     = 0;        // value range; not referenced if RANGE != 'V'
  double VU     = 0;
  int IL        = 1; // index range
  int IU        = M;
  double ABSTOL = 0;
  // If ABSTOL=0, EPS*|T| where |T| is the 1-norm of the tridiagonal
  // matrix obtained by reducing m to tridiagonal form.
  int MM; // total number of eigenvalues found
  int LDZ = dim;
  int ISUPPZ[2 * M];
  //  The support of the eigenvectors in Z, i.e., the indices
  //  indicating the nonzero elements in Z.  The i-th eigenvector is
  //  nonzero only in elements ISUPPZ( 2*i-1 ) through ISUPPZ(2*i).
  auto Z = std::make_unique<lapack_complex_double[]>(LDZ * M); // eigenvectors
  int LWORK0 = -1;                 // length of the WORK array (-1 == query!)
  lapack_complex_double WORK0[1];
  int LRWORK0 = -1; // query
  double RWORK0[1];
  int LIWORK0 = -1; // query
  int IWORK0[1];
  // Step 1: determine optimal LWORK, LRWORK, and LIWORK
  LAPACK_zheevr(&jobz, &RANGE, &UPLO, &NN, ham, &LDA, &VL, &VU, &IL, &IU, &ABSTOL, &MM, (double *)eigenvalues, Z.get(), &LDZ, ISUPPZ, WORK0, &LWORK0,
                RWORK0, &LRWORK0, IWORK0, &LIWORK0, &INFO);
  my_assert(INFO == 0);
  int LWORK   = int(WORK0[0].real);
  auto WORK  = std::make_unique<lapack_complex_double[]>(LWORK);
  int LRWORK  = int(RWORK0[0]);
  auto RWORK = std::make_unique<double[]>(LRWORK);
  int LIWORK  = IWORK0[0];
  auto IWORK  = std::make_unique<int[]>(LIWORK);
  // Step 2: perform the diagonalisation
  LAPACK_zheevr(&jobz, &RANGE, &UPLO, &NN, ham, &LDA, &VL, &VU, &IL, &IU, &ABSTOL, &MM, (double *)eigenvalues, Z.get(), &LDZ, ISUPPZ, WORK.get(), &LWORK,
                RWORK.get(), &LRWORK, IWORK.get(), &LIWORK, &INFO);
  if (INFO != 0) throw std::runtime_error(fmt::format("zheevr failed. INFO={}", INFO));
  if (MM != int(M)) {
    cout << "zheevr computed " << MM << "/" << M << endl;
    M = MM;
    my_assert(M > 0); // at least one
  }
  return copy_results(eigenvalues, Z.get(), jobz, dim, M);
}
#endif

void checkdiag(const Eigen &d, 
               const double NORMALIZATION_EPSILON = 1e-12,
               const double ORTHOGONALITY_EPSILON = 1e-12)
{
  const auto M = d.getnrcomputed(); // number of eigenpairs
  const auto dim = d.getdim();   // dimension of the eigenvector
  my_assert(d.matrix.size2() == dim);
  // Check normalization
  for (const auto r: range0(M)) {
    assert_isfinite(d.value_orig(r));
    double sumabs = 0.0;
    for (const auto j: range0(dim)) {
      assert_isfinite(d.matrix(r, j));
      sumabs += sqr(abs(d.matrix(r, j)));
    }
    my_assert(num_equal(sumabs, 1.0, NORMALIZATION_EPSILON));
  }
  // Check orthogonality
  for (const auto r1: range0(M))
    for (const auto r2: boost::irange(r1+1, M)) {
      t_matel skpdt = 0.0;
      for (const auto j: range0(dim)) skpdt += CONJ_ME(d.matrix(r1, j)) * d.matrix(r2, j);
      my_assert(num_equal(abs(skpdt), 0.0, ORTHOGONALITY_EPSILON));
    }
}

void dump_eigenvalues(const Eigen &d, size_t max_nr = std::numeric_limits<size_t>::max())
{
  cout << "eig= ";
  for_each_n(cbegin(d.value_orig), min(d.getnrcomputed(), max_nr), 
           [](const t_eigen x) { cout << x << ' '; });
  cout << endl;
}

template<typename M>
  bool has_lesseq_rows(const ublas::matrix<M> &A, const ublas::matrix<M> &B) {
    return A.size1() <= B.size1() && A.size2() == B.size2();
  }

// Wrapper for the diagonalization of the Hamiltonian matrix. The number of eigenpairs returned does NOT need to be
// equal to the dimension of the matrix h. m is destroyed in the process, thus no const attribute!
template<typename M> Eigen diagonalise(ublas::matrix<M> &m, const DiagParams &DP) {
  mpilog("diagonalise " << m.size1() << "x" << m.size2() << " " << DP.diag << " " << DP.diagratio);
  time_mem::Timing t;
  check_is_matrix_upper(m);
  Eigen d;
  if constexpr (std::is_same_v<M, double>) {
    if (DP.diag == "dsyev"s) 
      d = diagonalise_dsyev(m);
    if (DP.diag == "dsyevd"s) {
      d = diagonalise_dsyevd(m);
      if (d.getnrcomputed() == 0) {
        std::cout << "dsyevd failed, falling back to dsyev" << std::endl;
        d = diagonalise_dsyev(m);
      }
    }
    if (DP.diag == "dsyevr"s) 
      d = diagonalise_dsyevr(m, DP.diagratio);
  } else if constexpr (std::is_same_v<M, std::complex<double>>) {
    if (DP.diag == "zheev"s)  
      d = diagonalise_zheev(m);
    if (DP.diag == "zheevr"s) 
      d = diagonalise_zheevr(m, DP.diagratio);
  } else my_assert_not_reached();
  my_assert(d.getnrcomputed() > 0);
  my_assert(has_lesseq_rows(d.matrix, m));
  if (DP.logletter('e'))
    dump_eigenvalues(d);
  checkdiag(d);
  nrglogdp('A', "LAPACK, dim=" << m.size1() << " M=" << d.getnrcomputed() << " [rank " << myrank() << "]");
  nrglogdp('t', "Elapsed: " << setprecision(3) << t.total_in_seconds() << " [rank " << myrank() << "]");
  return d;
}

#endif // _diag_h_
