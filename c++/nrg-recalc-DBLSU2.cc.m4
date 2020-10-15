// *** WARNING!!! Modify nrg-recalc-DBLSU2.cc.m4, not nrg-recalc-DBLSU2.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, Dec 2009
// This file pertains to (I1,I2) subspaces

include(recalc-macros.m4)

// Recalculate matrix elements of a doublet tenzor operator
MatrixElements SymmetryDBLSU2::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Ispin ii11 = I1.get("II1");
    Ispin ii21 = I1.get("II2");
    Invar Ip;

    Ip = Invar(ii11 - 1, ii21);
    RECALC_TAB("dblsu2/dblsu2-2ch-doubletm0.dat", Invar(2, 0));

    Ip = Invar(ii11 + 1, ii21);
    RECALC_TAB("dblsu2/dblsu2-2ch-doubletp0.dat", Invar(2, 0));

    Ip = Invar(ii11, ii21 - 1);
    RECALC_TAB("dblsu2/dblsu2-2ch-doublet0m.dat", Invar(0, 2));

    Ip = Invar(ii11, ii21 + 1);
    RECALC_TAB("dblsu2/dblsu2-2ch-doublet0p.dat", Invar(0, 2));
  }
  return cnew;
}

// Driver routine for recalc_f()
Opch SymmetryDBLSU2::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Invar I1;

    Ispin ii1p = Ip.get("II1");
    Ispin ii2p = Ip.get("II2");

    // NN is index n of f_n, the last site in the chain prior to adding
    // the new site (f_{n+1}).
    int NN = step.getnn();

    // RECALC_F_TAB_... (filename, channel_number, matrix_number, array_length)

    // type 1: [f^dag_UP, f_DO]
    // type 2: [f^dag_DO, f_UP]

    I1 = Invar(ii1p + 1, ii2p);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type1-isoup-a.dat", 0, 0);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type2-isoup-a.dat", 0, 1);

    I1 = Invar(ii1p, ii2p + 1);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type1-isoup-b.dat", 1, 0);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type2-isoup-b.dat", 1, 1);

    I1 = Invar(ii1p - 1, ii2p);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type1-isodown-a.dat", 0, 0);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type2-isodown-a.dat", 0, 1);

    I1 = Invar(ii1p, ii2p - 1);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type1-isodown-b.dat", 1, 0);
    RECALC_F_TAB_N("dblsu2/dblsu2-2ch-type2-isodown-b.dat", 1, 1);
  }
  return opch;
}

#undef SPINX
#define SPINX(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)
#undef SPINZ
#define SPINZ(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#ifdef NRG_COMPLEX
#undef SPINY
#define SPINY(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)
#undef Complex
#define Complex(x, y) cmpl(x, y)
#endif // NRG_COMPLEX

void SymmetryDBLSU2::recalc_global(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, string name, MatrixElements &cnew) {
  if (name == "SZtot") {
   for(const auto &[I1, eig]: diag) {
      const Twoinvar II{I1, I1};
      Matrix &cn = cnew[II];
      switch (P.channels) {
        case 2:
#include "dblsu2/dblsu2-2ch-spinz.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

#ifdef NRG_COMPLEX
  if (name == "SYtot") {
   for(const auto &[I1, eig]: diag) {
      const Twoinvar II{I1, I1};
      Matrix &cn = cnew[II];
      switch (P.channels) {
        case 2:
#include "dblsu2/dblsu2-2ch-spiny.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }
#endif // NRG_COMPLEX

  if (name == "SXtot") {
   for(const auto &[I1, eig]: diag) {
      const Twoinvar II{I1, I1};
      Matrix &cn = cnew[II];
      switch (P.channels) {
        case 2:
#include "dblsu2/dblsu2-2ch-spinx.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  my_assert_not_reached();
}
