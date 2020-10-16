// *** WARNING!!! Modify nrg-recalc-SL.cc.m4, not nrg-recalc-SL.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, June 2006, Nov 2007
// This file pertains to the spinless-fermions code.

include(recalc-macros.m4)

MatrixElements SymmetrySL::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Number q1 = I1.get("Q");
    Invar Ip  = Invar(q1 - 1);
    ONE23(`RECALC_TAB("sl/sl-1ch-doublet.dat", Invar(1))',
    	  `RECALC_TAB("sl/sl-2ch-doublet.dat", Invar(1))',
          `RECALC_TAB("sl/sl-3ch-doublet.dat", Invar(1))');
  }
  return cnew;
}

Opch SymmetrySL::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp = Ip.get("Q");
    Invar I1  = Invar(qp + 1);
    ONE23(`RECALC_F_TAB("sl/sl-1ch-a.dat", 0)',
    
    	   `RECALC_F_TAB("sl/sl-2ch-a.dat", 0); 
          RECALC_F_TAB("sl/sl-2ch-b.dat", 1)',

         `RECALC_F_TAB("sl/sl-3ch-a.dat", 0); 
          RECALC_F_TAB("sl/sl-3ch-b.dat", 1);
          RECALC_F_TAB("sl/sl-3ch-c.dat", 2)' );
  }
  return opch;
}

#undef QDIFF
#define QDIFF(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#undef QTOT
#define QTOT(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#undef N1
#define N1(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#undef N2
#define N2(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#undef N3
#define N3(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

void SymmetrySL::recalc_global(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, string name, MatrixElements &cnew) {
  if (name == "Qdiff") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 2:
#include "sl/sl-2ch-qdiff.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "Qtot") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 2:
#include "sl/sl-2ch-qtot.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "N1") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 3:
#include "sl/sl-3ch-N1.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "N2") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 3:
#include "sl/sl-3ch-N2.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "N3") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 3:
#include "sl/sl-3ch-N3.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  my_assert_not_reached();
}
