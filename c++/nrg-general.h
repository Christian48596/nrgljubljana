// Numerical renormalization group
// (c) Rok Zitko, rok.zitko@ijs.si, 2005-2020

#ifndef _nrg_general_h_
#define _nrg_general_h_

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <string>
using namespace std::string_literals;

#include <complex>
#include <unordered_map>
#include <map>
#include <list>
#include <deque>
#include <set>
#include <vector>

#include <numeric>
#include <utility>
#include <algorithm>
#include <functional>
#include <limits>
#include <memory>

using namespace std;

// C headers
#include <cassert>
#include <cmath>
#include <cfloat>
#include <climits>
#include <cstring>

#include <unistd.h>
#include <cstdlib> // mkdtemp

#include <boost/range/adaptor/map.hpp>

// ublas matrix & vector containers
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/operation.hpp>
using namespace boost::numeric;
using namespace boost::numeric::ublas;

// Numeric bindings to BLAS/LAPACK
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/atlas/cblas.hpp>
namespace atlas = boost::numeric::bindings::atlas;

#include <boost/lexical_cast.hpp>
#include <boost/io/ios_state.hpp>

// Serialization support (used for storing to files and for MPI)
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#ifdef NRG_COMPLEX
 #include <boost/serialization/complex.hpp>
#endif

// This must be set manually through CXXFLAG=-DNRG_MPI
#ifdef NRG_MPI
#include <boost/optional.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
namespace mpi = boost::mpi;
#endif // NRG_MPI

// Support for compiler dependant optimizations

#ifdef __GNUC__
#define PUREFNC __attribute__((pure))
#define CONSTFNC __attribute__((const))
#else
#define PUREFNC
#define CONSTFNC
#endif

#define FN_ENERGIES_NRG "energies.nrg"
#define FN_ENERGIES_DMNRG "energies.dmnrg"
#define FN_ANNOTATED "annotated.dat"
#define FN_TD "td"
#define FN_TDFDM "tdfdm"
#define FN_CUSTOM "custom"
#define FN_CUSTOMFDM "customfdm"
#define FN_UNITARY "unitary"
#define FN_RHO "rho"
#define FN_RHOFDM "rhofdm"
#define FN_SUBSPACES "subspaces.dat"

#define HIGHPREC(val) setprecision(std::numeric_limits<double>::max_digits10) << (val)

int myrank();

#endif
