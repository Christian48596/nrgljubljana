// *** WARNING!!! Modify nrg-recalc-QSZ.cc.m4, not nrg-recalc-QSZ.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, 2006-2020
// This file pertains to (Q,SZ) subspaces

include(recalc-macros.m4)

// NOTE: p is ket (right side), 1 is bra (left side). OP is sandwiched in between. Thus Q[p] + Q[op] = Q[1].

MatrixElements SymmetryQSZ::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  if (!P.substeps) {
    for(const auto &[I1, eig]: diag) {
      Number q1   = I1.get("Q");
      SZspin ssz1 = I1.get("SSZ");
      Invar Ip;

      // In the case of (Q,S_z) basis, spin up and spin down are not
      // equivalent. The distinction appears in this recalculation code, but
      // also during the evaluation of the spectral densities, where two
      // spin-diagonal spectral densities (and also spin-flip spectral
      // density) can be defined.

      Ip = Invar(q1 - 1, ssz1 + 1);
    ONE23(`RECALC_TAB("qsz/qsz-1ch-doubletp.dat", Invar(1, -1))',
          `RECALC_TAB("qsz/qsz-2ch-doubletp.dat", Invar(1, -1))',
          `RECALC_TAB("qsz/qsz-3ch-doubletp.dat", Invar(1, -1))');

    Ip = Invar(q1-1, ssz1-1);
    ONE23(`RECALC_TAB("qsz/qsz-1ch-doubletm.dat", Invar(1, +1))',
          `RECALC_TAB("qsz/qsz-2ch-doubletm.dat", Invar(1, +1))',
          `RECALC_TAB("qsz/qsz-3ch-doubletm.dat", Invar(1, +1))');
    }      // loop
  } else { // substeps
    for(const auto &[I1, eig]: diag) {
      Number q1   = I1.get("Q");
      SZspin ssz1 = I1.get("SSZ");
      Invar Ip;

      Ip = Invar(q1 - 1, ssz1 + 1);
      RECALC_TAB("qsz/qsz-1ch-doubletp.dat", Invar(1, -1));

      Ip = Invar(q1 - 1, ssz1 - 1);
      RECALC_TAB("qsz/qsz-1ch-doubletm.dat", Invar(1, +1));
    } // loop
  }
  return cnew;
}

Opch SymmetryQSZ::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp   = Ip.get("Q");
    SZspin sszp = Ip.get("SSZ");
    Invar I1;

    // NOTE: q,ssz only couples to q+1,ssz+-1 in general, even for
    // several channels.

    I1 = Invar(qp + 1, sszp + 1);
    ONE23(`RECALC_F_TAB("qsz/qsz-1ch-spinupa.dat", 0)',

          `RECALC_F_TAB("qsz/qsz-2ch-spinupa.dat", 0);
      	   RECALC_F_TAB("qsz/qsz-2ch-spinupb.dat", 1)',

          `RECALC_F_TAB("qsz/qsz-3ch-spinupa.dat", 0);
           RECALC_F_TAB("qsz/qsz-3ch-spinupb.dat", 1);
      	   RECALC_F_TAB("qsz/qsz-3ch-spinupc.dat", 2)');

    I1 = Invar(qp+1, sszp-1);
    ONE23(`RECALC_F_TAB("qsz/qsz-1ch-spindowna.dat", 0)',

          `RECALC_F_TAB("qsz/qsz-2ch-spindowna.dat", 0);
	         RECALC_F_TAB("qsz/qsz-2ch-spindownb.dat", 1)',

          `RECALC_F_TAB("qsz/qsz-3ch-spindowna.dat", 0);
           RECALC_F_TAB("qsz/qsz-3ch-spindownb.dat", 1);
      	   RECALC_F_TAB("qsz/qsz-3ch-spindownc.dat", 2)');
  } // loop
  return opch;
}

OpchChannel SymmetryQSZ::recalc_irreduc_substeps(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, int M) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp   = Ip.get("Q");
    SZspin sszp = Ip.get("SSZ");
    Invar I1;

    I1 = Invar(qp + 1, sszp + 1);
    RECALC_F_TAB("qsz/qsz-1ch-spinupa.dat", M);

    I1 = Invar(qp + 1, sszp - 1);
    RECALC_F_TAB("qsz/qsz-1ch-spindowna.dat", M);
  } // loop
  return opch[M];
}

MatrixElements SymmetryQSZ::recalc_triplet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  if (!P.substeps) {
    for(const auto &[I1, eig]: diag) {
      Number q1   = I1.get("Q");
      SZspin ssz1 = I1.get("SSZ");
      Invar Ip;

      Ip = Invar(q1, ssz1);
    ONE23(`RECALC_TAB("qsz/qsz-1ch-triplets.dat", Invar(0, 0))',
          `RECALC_TAB("qsz/qsz-2ch-triplets.dat", Invar(0, 0))',
          `RECALC_TAB("qsz/qsz-3ch-triplets.dat", Invar(0, 0))');

    Ip = Invar(q1, ssz1+2);
    ONETWO(`RECALC_TAB("qsz/qsz-1ch-tripletp.dat", Invar(0, -2))',
           `RECALC_TAB("qsz/qsz-2ch-tripletp.dat", Invar(0, -2))',
           `RECALC_TAB("qsz/qsz-3ch-tripletp.dat", Invar(0, -2))');

    Ip = Invar(q1, ssz1-2);
    ONETWO(`RECALC_TAB("qsz/qsz-1ch-tripletm.dat", Invar(0, +2))',
           `RECALC_TAB("qsz/qsz-2ch-tripletm.dat", Invar(0, +2))',
           `RECALC_TAB("qsz/qsz-3ch-tripletm.dat", Invar(0, +2))');
    }      // loop
  } else { // substeps
    my_assert_not_reached();
  }
  return cnew;
}

#undef SPINZ
#define SPINZ(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)
#undef Q1U
#define Q1U(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)
#undef Q1D
#define Q1D(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

void SymmetryQSZ::recalc_global(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, string name, MatrixElements &cnew) {
  if (name == "SZtot") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 1:
#include "qsz/qsz-1ch-spinz.dat"
          break;
        case 2:
#include "qsz/qsz-2ch-spinz.dat"
          break;
        case 3:
#include "qsz/qsz-3ch-spinz.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "Q1u") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 1:
#include "qsz/qsz-1ch-q1u.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "Q1d") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = {I1, I1};
      Matrix &cn        = cnew[II];
      switch (P.channels) {
        case 1:
#include "qsz/qsz-1ch-q1d.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  my_assert_not_reached();
}
