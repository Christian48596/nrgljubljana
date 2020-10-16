// *** WARNING!!! Modify nrg-recalc-U1.cc.m4, not nrg-recalc-U1.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, June 2006, Oct 2012
// This file pertains to (Q) subspaces

include(recalc-macros.m4)

// Recalculate matrix elements of a doublet tensor operator
MatrixElements SymmetryU1::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Number q1 = I1.get("Q");
    Invar Ip  = Invar(q1 - 1);
    ONE23(`RECALC_TAB("u1/u1-1ch-doublet.dat", Invar(1))',
    	    `RECALC_TAB("u1/u1-2ch-doublet.dat", Invar(1))',
	        `RECALC_TAB("u1/u1-3ch-doublet.dat", Invar(1))');
  }
  return cnew;
}

// Driver routine for recalc_f()
Opch SymmetryU1::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp = Ip.get("Q");
    Invar I1  = Invar(qp + 1);
    ONE23(`RECALC_F_TAB_N("u1/u1-1ch-a-DO.dat", 0, 1);
           RECALC_F_TAB_N("u1/u1-1ch-a-UP.dat", 0, 0)',

          `RECALC_F_TAB_N("u1/u1-2ch-a-DO.dat", 0, 1);
	         RECALC_F_TAB_N("u1/u1-2ch-b-DO.dat", 1, 1);
	         RECALC_F_TAB_N("u1/u1-2ch-a-UP.dat", 0, 0);
	         RECALC_F_TAB_N("u1/u1-2ch-b-UP.dat", 1, 0)',

          `RECALC_F_TAB_N("u1/u1-3ch-a-DO.dat", 0, 1);
	         RECALC_F_TAB_N("u1/u1-3ch-b-DO.dat", 1, 1);
	         RECALC_F_TAB_N("u1/u1-3ch-c-DO.dat", 2, 1);
	         RECALC_F_TAB_N("u1/u1-3ch-a-UP.dat", 0, 0);
	         RECALC_F_TAB_N("u1/u1-3ch-b-UP.dat", 1, 0);
	         RECALC_F_TAB_N("u1/u1-3ch-c-UP.dat", 2, 0);');
  }
  return opch;
}

#undef SPINX
#define SPINX(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)
#undef SPINY
#define SPINY(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)
#undef SPINZ
#define SPINZ(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

void SymmetryU1::recalc_global(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, string name, MatrixElements &cnew) {
  if (name == "SZtot") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 1:
#include "u1/u1-1ch-spinz.dat"
          break;
        case 2:
#include "u1/u1-2ch-spinz.dat"
          break;
        case 3:
#include "u1/u1-3ch-spinz.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

#ifdef NRG_COMPLEX
#undef Complex
#define Complex(x, y) cmpl(x, y)
  if (name == "SYtot") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 1:
#include "u1/u1-1ch-spiny.dat"
          break;
        case 2:
#include "u1/u1-2ch-spiny.dat"
          break;
        case 3:
#include "u1/u1-3ch-spiny.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }
#endif

  if (name == "SXtot") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 1:
#include "u1/u1-1ch-spinx.dat"
          break;
        case 2:
#include "u1/u1-2ch-spinx.dat"
          break;
        case 3:
#include "u1/u1-3ch-spinx.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  my_assert_not_reached();
}
