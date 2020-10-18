#ifndef _spec_cc_
#define _spec_cc_

// For argument 'sign' in calc_generic_* functions. This is the only difference between the GFs for bosonic and
// fermionic operators.
const int S_FERMIONIC = -1;
const int S_BOSONIC   = 1;

const double WEIGHT_TOL = 1e-8; // where to switch to l'Hospital rule form

#include "spec_FT.cc"
#include "spec_DMNRG.cc"
#include "spec_FDM.cc"
#include "spec_CFS.cc"

// Calculate (finite temperature) spectral function 1/Pi Im << op1^\dag(t) op2(0) >>. Required spin direction is
// determined by 'SPIN'. For SPIN=0 both spin direction are equivalent. For QSZ, we need to differentiate the two.

template <typename FactorFnc, typename CheckSpinFnc, typename S>
void calc_generic(const BaseSpectrum &bs, const Step &step, const DiagInfo_tmpl<S> &diag, 
                  FactorFnc && factorfnc, CheckSpinFnc && checkspinfnc, 
                  const DensMatElements_tmpl<S> &rho, const DensMatElements_tmpl<S> &rhoFDM, const Stats &stats) {
  auto cs = bs.algotype->make_cs(bs);
  const auto & rho_here = bs.algotype->rho_type() == "rhoFDM" ? rhoFDM : rho;
  // Strategy: we loop through all subspace pairs and check whether they have non-zero irreducible matrix elements.
  for(const auto &[Ii, diagi]: diag)
    for(const auto &[Ij, diagj]: diag) {
      const Twoinvar II {Ij,Ii};
      if (bs.op1.count(II) && bs.op2.count(II) && checkspinfnc(Ij, Ii, bs.spin))
        bs.algotype->calc(step, diagi, diagj, bs.op1.at(II), bs.op2.at(II), bs, factorfnc(Ii, Ij), cs, Ii, Ij, rho_here, stats);
    }
  bs.spec->merge(cs, step);
}

#endif // _spec_cc_
