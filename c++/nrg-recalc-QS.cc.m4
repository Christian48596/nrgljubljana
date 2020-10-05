// *** WARNING!!! Modify nrg-recalc-QS.cc.m4, not nrg-recalc-QS.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, Feb 2006
// This file pertains to (Q,S) subspaces

include(recalc-macros.m4)

namespace QS {
#include "qs/qs-1ch-def.dat"
#include "qs/qs-2ch-def.dat"
#include "qs/qs-3ch-def.dat"
#include "qs/qs-4ch-def.dat"
}

// Recalculate matrix elements of a doublet tensor operator
ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO // avoid false positives
MatrixElements SymmetryQS::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  if (!substeps) {
    for(const auto &[I1, eig]: diag) {
      Number q1 = I1.get("Q");
      Sspin ss1 = I1.get("SS");
      Invar Ip;

      Ip = Invar(q1 - 1, ss1 + 1);
    ONE234(`RECALC_TAB("qs/qs-1ch-doubletp.dat", QS::LENGTH_D_1CH, Invar(1, 2))',
           `RECALC_TAB("qs/qs-2ch-doubletp.dat", QS::LENGTH_D_2CH, Invar(1, 2))',
           `RECALC_TAB("qs/qs-3ch-doubletp.dat", QS::LENGTH_D_3CH, Invar(1, 2))',
           `RECALC_TAB("qs/qs-4ch-doubletp.dat", QS::LENGTH_D_4CH, Invar(1, 2))');

    Ip = Invar(q1-1, ss1-1);
    ONE234(`RECALC_TAB("qs/qs-1ch-doubletm.dat", QS::LENGTH_D_1CH, Invar(1, 2))',
           `RECALC_TAB("qs/qs-2ch-doubletm.dat", QS::LENGTH_D_2CH, Invar(1, 2))',
           `RECALC_TAB("qs/qs-3ch-doubletm.dat", QS::LENGTH_D_3CH, Invar(1, 2))',
           `RECALC_TAB("qs/qs-4ch-doubletm.dat", QS::LENGTH_D_4CH, Invar(1, 2))');
    }
  } else {
    for(const auto &[I1, eig]: diag) {
      Number q1 = I1.get("Q");
      Sspin ss1 = I1.get("SS");
      Invar Ip;

      Ip = Invar(q1 - 1, ss1 + 1);
      RECALC_TAB("qs/qs-1ch-doubletp.dat", QS::LENGTH_D_1CH, Invar(1, 2));

      Ip = Invar(q1 - 1, ss1 - 1);
      RECALC_TAB("qs/qs-1ch-doubletm.dat", QS::LENGTH_D_1CH, Invar(1, 2));
    }
  }
  return cnew;
}

// (QS): Two calls of recalc_f() are necessary (for S+1/2 and S-1/2)
// for each channel.
// See Krishna-Murthy p. 1034, equation (B10).

// Driver routine for recalc_f()
ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO // avoid false positives
Opch SymmetryQS::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp = Ip.get("Q");
    Sspin ssp = Ip.get("SS");
    Invar I1;

    // NOTE: q,ss only couples to q+1,ss+-1 in general, even for
    // several channels.

    I1 = Invar(qp + 1, ssp + 1);
    ONE234(`RECALC_F_TAB("qs/qs-1ch-spinupa.dat", 0, QS::LENGTH_I_1CH)',

           `RECALC_F_TAB("qs/qs-2ch-spinupa.dat", 0, QS::LENGTH_I_2CH);
	    RECALC_F_TAB("qs/qs-2ch-spinupb.dat", 1, QS::LENGTH_I_2CH)',

           `RECALC_F_TAB("qs/qs-3ch-spinupa.dat", 0, QS::LENGTH_I_3CH_0);
	    RECALC_F_TAB("qs/qs-3ch-spinupb.dat", 1, QS::LENGTH_I_3CH_1);
	    RECALC_F_TAB("qs/qs-3ch-spinupc.dat", 2, QS::LENGTH_I_3CH_2)',

           `RECALC_F_TAB("qs/qs-4ch-spinupa.dat", 0, QS::LENGTH_I_4CH_0);
	    RECALC_F_TAB("qs/qs-4ch-spinupb.dat", 1, QS::LENGTH_I_4CH_1);
	    RECALC_F_TAB("qs/qs-4ch-spinupc.dat", 2, QS::LENGTH_I_4CH_2);
	    RECALC_F_TAB("qs/qs-4ch-spinupd.dat", 3, QS::LENGTH_I_4CH_3)');

    I1 = Invar(qp+1, ssp-1);
    ONE234(`RECALC_F_TAB("qs/qs-1ch-spindowna.dat", 0, QS::LENGTH_I_1CH)',

           `RECALC_F_TAB("qs/qs-2ch-spindowna.dat", 0, QS::LENGTH_I_2CH);
            RECALC_F_TAB("qs/qs-2ch-spindownb.dat", 1, QS::LENGTH_I_2CH)',

           `RECALC_F_TAB("qs/qs-3ch-spindowna.dat", 0, QS::LENGTH_I_3CH_0);
	    RECALC_F_TAB("qs/qs-3ch-spindownb.dat", 1, QS::LENGTH_I_3CH_1);
	    RECALC_F_TAB("qs/qs-3ch-spindownc.dat", 2, QS::LENGTH_I_3CH_2)',

           `RECALC_F_TAB("qs/qs-4ch-spindowna.dat", 0, QS::LENGTH_I_4CH_0);
	    RECALC_F_TAB("qs/qs-4ch-spindownb.dat", 1, QS::LENGTH_I_4CH_1);
	    RECALC_F_TAB("qs/qs-4ch-spindownc.dat", 2, QS::LENGTH_I_4CH_2);
	    RECALC_F_TAB("qs/qs-4ch-spindownd.dat", 3, QS::LENGTH_I_4CH_3)');
  }
  return opch;
}

// Driver routine for recalc_f()
ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO // avoid false positives
OpchChannel SymmetryQS::recalc_irreduc_substeps(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P, int M) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp = Ip.get("Q");
    Sspin ssp = Ip.get("SS");
    Invar I1;

    I1 = Invar(qp + 1, ssp + 1);
    RECALC_F_TAB("qs/qs-1ch-spinupa.dat", M, QS::LENGTH_I_1CH);

    I1 = Invar(qp + 1, ssp - 1);
    RECALC_F_TAB("qs/qs-1ch-spindowna.dat", M, QS::LENGTH_I_1CH);
  }
  return opch[M];
}

// Recalculate matrix elements of a triplet tenzor operator
ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO // avoid false positives
MatrixElements SymmetryQS::recalc_triplet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  if (!substeps) {
    for(const auto &[I1, eig]: diag) {
      Number q1 = I1.get("Q");
      Sspin ss1 = I1.get("SS");
      Invar Ip;

      Ip = Invar(q1, ss1);
    ONE234(`RECALC_TAB("qs/qs-1ch-triplets.dat", QS::LENGTH_T0_1CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-2ch-triplets.dat", QS::LENGTH_T0_2CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-3ch-triplets.dat", QS::LENGTH_T0_3CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-4ch-triplets.dat", QS::LENGTH_T0_4CH, Invar(0, 3))');

    Ip = Invar(q1, ss1+2);
    ONE234(`RECALC_TAB("qs/qs-1ch-tripletp.dat", QS::LENGTH_Tpm_1CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-2ch-tripletp.dat", QS::LENGTH_Tpm_2CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-3ch-tripletp.dat", QS::LENGTH_Tpm_3CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-4ch-tripletp.dat", QS::LENGTH_Tpm_4CH, Invar(0, 3))');

    Ip = Invar(q1, ss1-2);
    ONE234(`RECALC_TAB("qs/qs-1ch-tripletm.dat", QS::LENGTH_Tpm_1CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-2ch-tripletm.dat", QS::LENGTH_Tpm_2CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-3ch-tripletm.dat", QS::LENGTH_Tpm_3CH, Invar(0, 3))',
           `RECALC_TAB("qs/qs-4ch-tripletm.dat", QS::LENGTH_Tpm_4CH, Invar(0, 3))');
    }
  } else my_assert_not_reached();
  return cnew;
}

#undef QDIFF
#define QDIFF(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#undef Q1
#define Q1(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#undef Q2
#define Q2(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

#undef QTOT
#define QTOT(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO // avoid false positives
void SymmetryQS::recalc_global(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, string name, MatrixElements &cnew) {
  if (name == "Qdiff") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = make_pair(I1, I1);
      Matrix &cn        = cnew[II];
      switch (channels) {
        case 2:
#include "qs/qs-2ch-qdiff.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "Q1") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = make_pair(I1, I1);
      Matrix &cn        = cnew[II];
      switch (channels) {
        case 2:
#include "qs/qs-2ch-q1.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "Q2") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = make_pair(I1, I1);
      Matrix &cn        = cnew[II];
      switch (channels) {
        case 2:
#include "qs/qs-2ch-q2.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  if (name == "Qtot") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II = make_pair(I1, I1);
      Matrix &cn        = cnew[II];
      switch (channels) {
        case 2:
#include "qs/qs-2ch-qtot.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }

  my_assert_not_reached();
}

#undef Q2
