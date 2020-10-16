// dmnrg.h - Density-matrix NRG
// Copyright (C) 2009-2020 Rok Zitko

#ifndef _dmnrg_h_
#define _dmnrg_h_

// Calculation of the contribution from subspace I1 of rhoN (density matrix at iteration N) to rhoNEW (density matrix
// at iteration N-1)
void cdmI(const size_t i,        // Subspace index (alpha=1,...,P.combs)
          const Invar &I1,       // Quantum numbers corresponding to subspace i
          const Matrix &rhoN,    // rho^N
          const Eigen &diagI1,   // contains U_{I1}
          Matrix &rhoNEW,        // rho^{N-1}
          const size_t N,
          const t_factor factor, // multiplicative factor that accounts for multiplicity
          const AllSteps &dm,
          const Params &P) 
{
  nrglog('D', "cdmI i=" << i << " I1=" << I1 << " factor=" << factor);
  // Range of indexes r and r' in matrix C^{QS,N}_{r,r'}, cf. Eq. (3.55) in my dissertation.
  const auto dim = rhoNEW.size2();
  // number of states taken into account in the density-matrix at *current* (Nth) stage (in subspace I1)
  const auto nromega = rhoN.size2();
  if (nromega == 0 || dim == 0) return;   // continue only if connection exists
  // rmax (info[I1].rmax[i]) is the range of r in U^N_I1(omega|ri), only those states that we actually kept..
  const auto rmax = dm[N].at(I1).rmax.rmax(i);
  if (rmax == 0) return;    // rmax can be zero in the case a subspace has been completely truncated
  my_assert(rmax == dim);   // Otherwise, rmax must equal dim
  // Check range of omega: do the dimensions of C^N_I1(omega omega') and U^N_I1(omega|r1) match? We do this test at
  // this point, to ensure rmax!=0 and dim!=0 and nromega!=0, otherwise there is no contribution anyway.
  const auto I1nr = diagI1.getnrstored();
  my_assert(nromega <= I1nr);
  // offset gives the offset that is added to r1,rp to find the elements ri in U^N_I1(omega|ri)
  const auto offset = dm[N].at(I1).rmax.offset(i);
  const auto dim1   = diagI1.matrix.size1();
  const auto dim2   = diagI1.matrix.size2();
  my_assert(nromega <= dim1 && offset + dim <= dim2);
  const ublas::matrix_range<const Matrix> U(diagI1.matrix, ublas::range(0, nromega), ublas::range(offset, offset + dim));
  Matrix T(dim, nromega);
  atlas::gemm(CblasConjTrans, CblasNoTrans, t_factor(1.0), U, rhoN, t_factor(0.0), T);    // T <- U^dag rhoN
  atlas::gemm(CblasNoTrans, CblasNoTrans, t_factor(factor), T, U, t_factor(1.0), rhoNEW); // rhoNEW <- rhoNEW + factor T U
}

// Calculation of the shell-N REDUCED DENSITY MATRICES: Calculate rho at previous iteration (N-1, rhoPrev) from rho
// at the current iteration (N, rho)
void calc_densitymatrix_iterN(const DiagInfo &diag,
                              const DensMatElements &rho, // input
                              DensMatElements &rhoPrev,   // output
                              const size_t N, const AllSteps &dm, shared_ptr<Symmetry> Sym, const Params &P) {
  nrglog('D', "calc_densitymatrix_iterN N=" << N);
  for (const auto &[I, dimsub] : dm[N - 1]) { // loop over all subspaces at *previous* iteration
    const auto subs = Sym->dmnrg_subspaces(I);
    const auto dim  = dimsub.kept;
    rhoPrev[I]      = Matrix(dim, dim, 0);
    if (dim == 0) continue;
//    for (const auto &sub: subs) { : 0-based
    for (const auto i : Sym->combs1()) {
      Invar sub = subs[i];
      const auto x = rho.find(sub);
      const auto y = diag.find(sub);
      if (x != rho.end() && y != diag.end()) {
        const auto coef = double(Sym->mult(sub)) / double(Sym->mult(I));
        cdmI(i, sub, x->second, y->second, rhoPrev[I], N, coef, dm, P);
      }
    }
  }
}

inline bool file_exists(const std::string &fn)
{
  ofstream F(fn, ios::binary | ios::out);
  return bool(F);
}

// Returns true if all the required density matrices are already
// saved on the disk.
bool already_computed(const std::string &prefix, const Params &P) {
  for (auto N = P.Nmax - 1; N > P.Ninit; N--) {
    const std::string fn = workdir.rhofn(prefix, N - 1); // note the minus 1
    if (!file_exists(fn)) {
      cout << fn << " not found. Computing." << endl;
      return false;
    }
  }
  return true;
}

/* calc_densitymatrix() is called prior to starting the NRG procedure for
 the second time. Here we calculate the shell-N density matrices for all
 iteration steps. */

void calc_densitymatrix(DensMatElements &rho, const AllSteps &dm, shared_ptr<Symmetry> Sym, const Params &P,
                        const std::string filename = FN_RHO) {
  if (P.resume && already_computed(filename, P)) return;
  check_trace_rho(rho, Sym); // Must be 1.
  if (P.ZBW) return;
  TIME("DM");
  for (size_t N = P.Nmax - 1; N > P.Ninit; N--) {
    cout << "[DM] " << N << endl;
    DiagInfo diag_loaded(N);
    DensMatElements rhoPrev;
    calc_densitymatrix_iterN(diag_loaded, rho, rhoPrev, N, dm, Sym, P);
    check_trace_rho(rhoPrev, Sym); // Make sure rho is normalized to 1.
    rhoPrev.save(N-1, filename);
    rho.swap(rhoPrev);
  }
}

// ****************** Calculation of the FULL REDUCED DENSITY MATIRICES

// Calculate rho(N), the shell-N density matrix, computed using
// the discarded states at shell N.
// Must be called AFTER calc_ZnD().
// Called fron nrg.cc immediately after the first NRG run (with N=Nmax-1),
// and also from calc_fulldensity_iterN (with lower N).
// A. Weichselbaum, J. von Delft, Phys. Rev. Lett. 99, 076402 (2007)
// T. A. Costi, V. Zlatic, Phys. Rev. B 81, 235127 (2010)
// H. Zhang, X. C. Xie, Q. Sun, Phys. Rev. B 82, 075111 (2010)
DensMatElements init_rho_FDM(const size_t N, const AllSteps &dm, const Stats &stats, shared_ptr<Symmetry> Sym, const double T) {
  DensMatElements rhoFDM;
  for (const auto &[I, ds] : dm[N]) {
    rhoFDM[I] = Matrix(ds.max(), ds.max(), 0);
    if (stats.ZnDNd[N] != 0.0)
      for (const auto i: ds.all())
        rhoFDM[I](i, i) = exp(-ds.eig.absenergyN[i] / T) * stats.wn[N] / stats.ZnDNd[N];
  }
  if (stats.wn[N] != 0.0) { // note: wn \propto ZnDNd, so this is the same condition as above
    // Trace should be equal to the total weight of the shell-N contribution to the FDM.
    const auto tr = rhoFDM.trace(Sym->multfnc());
    const auto diff = (tr - stats.wn[N]) / stats.wn[N]; // relative error
    if (!num_equal(diff, 0.0, 1e-8)) my_assert(stats.wn[N] < 1e-12); // OK if small enough overall
  }
  return rhoFDM;
}

void calc_fulldensitymatrix_iterN(const Step &step, // only required for step::last()
                                  const DiagInfo &diag,
                                  const DensMatElements &rhoFDM, // input
                                  DensMatElements &rhoFDMPrev,   // output
                                  const size_t N, const AllSteps &dm, const Stats &stats,
                                  shared_ptr<Symmetry> Sym, const Params &P) {
  nrglog('D', "calc_fulldensitymatrix_iterN N=" << N);
  DensMatElements rhoDD;
  if (!step.last(N))
    rhoDD = init_rho_FDM(N, dm, stats, Sym, P.T);
  for (const auto &[I, ds] : dm[N - 1]) { // loop over all subspaces at *previous* iteration
    const auto subs = Sym->dmnrg_subspaces(I);
    const auto dim  = ds.kept;
    rhoFDMPrev[I]   = Matrix(dim, dim, 0);
    if (!dim) continue;
    for (const auto i : Sym->combs1()) {
      const auto sub = subs[i];
      // DM construction for non-Abelian symmetries: must include the ratio of multiplicities as a coefficient.
      const t_factor coef = double(Sym->mult(sub)) / double(Sym->mult(I));
      // Contribution from the KK sector.
      const auto x1 = rhoFDM.find(sub);
      const auto y = diag.find(sub);
      if (x1 != rhoFDM.end() && y != diag.end())
        cdmI(i, sub, x1->second, y->second, rhoFDMPrev[I], N, coef, dm, P);
      // Contribution from the DD sector. rhoDD -> rhoFDMPrev
      if (!step.last(N))
        if (const auto x2 = rhoDD.find(sub); x2 !=rhoDD.end() && y != diag.end())
          cdmI(i, sub, x2->second, y->second, rhoFDMPrev[I], N, coef, dm, P);
      // (Exception: for the N-1 iteration, the rhoPrev is already initialized with the DD sector of the last iteration.) }
    } // over combinations
  } // over subspaces
}

// Sum of statistical weights from site N to the end of the Wilson chain.
auto sum_wn(const size_t N, const Stats &stats, const Params &P) {
  double sum = 0.0;
  for (size_t n = P.Nmax - 1; n >= N; n--) sum += stats.wn[n];
  return sum;
}

void calc_fulldensitymatrix(const Step &step, DensMatElements &rhoFDM, const AllSteps &dm, const Stats &stats, 
                            shared_ptr<Symmetry> Sym, const Params &P, const std::string filename = FN_RHOFDM) {
  if (P.resume && already_computed(filename, P)) return;
  if (P.ZBW) return;
  TIME("FDM");
  for (size_t N = P.Nmax - 1; N > P.Ninit; N--) {
    cout << "[FDM] " << N << endl;
    DiagInfo diag_loaded(N);
    DensMatElements rhoFDMPrev;
    calc_fulldensitymatrix_iterN(step, diag_loaded, rhoFDM, rhoFDMPrev, N, dm, stats, Sym, P);
    const auto tr       = rhoFDMPrev.trace(Sym->multfnc());
    const auto expected = sum_wn(N, stats, P);
    const auto diff     = (tr - expected) / expected;
    nrglog('w', "tr[rhoFDM(" << N << ")]=" << tr << " sum(wn)=" << expected << " diff=" << diff);
    my_assert(num_equal(diff, 0.0));
    rhoFDMPrev.save(N-1, filename);
    rhoFDM.swap(rhoFDMPrev);
  }
}

#endif // _dmnrg_h_
