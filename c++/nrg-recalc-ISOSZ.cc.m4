// *** WARNING!!! Modify nrg-recalc-ISO.cc.m4, not nrg-recalc-ISO.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, Feb 2006, Jan 2009
// This file pertains to (I,S) subspaces

include(recalc-macros.m4)

namespace ISOSZ {
#include "isosz/isosz-1ch-def.dat"
#include "isosz/isosz-2ch-def.dat"
}

// Recalculate matrix elements of a doublet tenzor operator
MatrixElements SymmetryISOSZ::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Ispin ii1   = I1.get("II");
    SZspin ssz1 = I1.get("SSZ");
    Invar Ip;

    Ip = Invar(ii1 - 1, ssz1 + 1);
    ONETWO(`RECALC_TAB("isosz/isosz-1ch-doubletmp.dat", ISOSZ::LENGTH_D_1CH, Invar(2, -1))',
           `RECALC_TAB("isosz/isosz-2ch-doubletmp.dat", ISOSZ::LENGTH_D_2CH, Invar(2, -1))');

    Ip = Invar(ii1-1, ssz1-1);
    ONETWO(`RECALC_TAB("isosz/isosz-1ch-doubletmm.dat", ISOSZ::LENGTH_D_1CH, Invar(2, +1))',
    	   `RECALC_TAB("isosz/isosz-2ch-doubletmm.dat", ISOSZ::LENGTH_D_2CH, Invar(2, +1))');

    Ip = Invar(ii1+1, ssz1+1);
    ONETWO(`RECALC_TAB("isosz/isosz-1ch-doubletpp.dat", ISOSZ::LENGTH_D_1CH, Invar(2, -1))',
    	   `RECALC_TAB("isosz/isosz-2ch-doubletpp.dat", ISOSZ::LENGTH_D_2CH, Invar(2, -1))');

    Ip = Invar(ii1+1, ssz1-1);
    ONETWO(`RECALC_TAB("isosz/isosz-1ch-doubletpm.dat", ISOSZ::LENGTH_D_1CH, Invar(2, +1))',
   	   `RECALC_TAB("isosz/isosz-2ch-doubletpm.dat", ISOSZ::LENGTH_D_2CH, Invar(2, +1))');
  }
  return cnew;
}

// (ISOSZ): Four calls of recalc_f() are necessary for each channel.

// Driver routine for recalc_f()
Opch SymmetryISOSZ::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Invar I1;

    // NOTE: ii,ss only couples to ii+-1,ss+-1 in general, even for
    // several channels.

    Ispin iip   = Ip.get("II");
    SZspin sszp = Ip.get("SSZ");
    // NN is index n of f_n, the last site in the chain prior to adding
    // the new site (f_{n+1}).
    int NN = step.getnn();

    I1 = Invar(iip + 1, sszp + 1);
    ONETWO(`RECALC_F_TAB("isosz/isosz-1ch-spinup-isoupa.dat", 0, ISOSZ::LENGTH_I_1CH)',

           `RECALC_F_TAB("isosz/isosz-2ch-spinup-isoupa.dat", 0, ISOSZ::LENGTH_I_2CH);
	    RECALC_F_TAB("isosz/isosz-2ch-spinup-isoupb.dat", 1, ISOSZ::LENGTH_I_2CH)');

    I1 = Invar(iip+1, sszp-1);
    ONETWO(`RECALC_F_TAB("isosz/isosz-1ch-spindown-isoupa.dat", 0, ISOSZ::LENGTH_I_1CH)',

           `RECALC_F_TAB("isosz/isosz-2ch-spindown-isoupa.dat", 0, ISOSZ::LENGTH_I_2CH);
	    RECALC_F_TAB("isosz/isosz-2ch-spindown-isoupb.dat", 1, ISOSZ::LENGTH_I_2CH)');

    I1 = Invar(iip-1, sszp+1);
    ONETWO(`RECALC_F_TAB("isosz/isosz-1ch-spinup-isodowna.dat", 0, ISOSZ::LENGTH_I_1CH)',

           `RECALC_F_TAB("isosz/isosz-2ch-spinup-isodowna.dat", 0, ISOSZ::LENGTH_I_2CH);
  	    RECALC_F_TAB("isosz/isosz-2ch-spinup-isodownb.dat", 1, ISOSZ::LENGTH_I_2CH)');

    I1 = Invar(iip-1, sszp-1);
    ONETWO(`RECALC_F_TAB("isosz/isosz-1ch-spindown-isodowna.dat", 0, ISOSZ::LENGTH_I_1CH)',

    	   `RECALC_F_TAB("isosz/isosz-2ch-spindown-isodowna.dat", 0, ISOSZ::LENGTH_I_2CH);
	    RECALC_F_TAB("isosz/isosz-2ch-spindown-isodownb.dat", 1, ISOSZ::LENGTH_I_2CH)');
  }
  return opch;
}

// Recalculate matrix elements of a triplet tenzor operator
MatrixElements SymmetryISOSZ::recalc_triplet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Ispin ii1   = I1.get("II");
    SZspin ssz1 = I1.get("SSZ");
    Invar Ip;

    Ip = Invar(ii1, ssz1);
    ONETWO(`RECALC_TAB("isosz/isosz-1ch-triplets.dat", ISOSZ::LENGTH_T0_1CH, Invar(1, 0))',
           `RECALC_TAB("isosz/isosz-2ch-triplets.dat", ISOSZ::LENGTH_T0_2CH, Invar(1, 0))');

    Ip = Invar(ii1, ssz1+2);
    ONETWO(`RECALC_TAB("isosz/isosz-1ch-tripletp.dat", ISOSZ::LENGTH_Tpm_1CH, Invar(1, -2))',
           `RECALC_TAB("isosz/isosz-2ch-tripletp.dat", ISOSZ::LENGTH_Tpm_2CH, Invar(1, -2))');

    Ip = Invar(ii1, ssz1-2);
    ONETWO(`RECALC_TAB("isosz/isosz-1ch-tripletm.dat", ISOSZ::LENGTH_Tpm_1CH, Invar(1, +2))',
           `RECALC_TAB("isosz/isosz-2ch-tripletm.dat", ISOSZ::LENGTH_Tpm_2CH, Invar(1, +2))');
  }
  return cnew;
}

#undef SPINZ
#define SPINZ(i1, ip, ch, value) recalc1_global(diag, qsrmax, I1, cn, i1, ip, value)

void SymmetryISOSZ::recalc_global(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, string name, MatrixElements &cnew) {
  if (name == "SZtot") {
    for(const auto &[I1, eig]: diag) {
      const Twoinvar II{I1, I1};
      Matrix &cn = cnew[II];
      switch (P.channels) {
        case 1:
#include "isosz/isosz-1ch-spinz.dat"
          break;
        case 2:
#include "isosz/isosz-2ch-spinz.dat"
          break;
        default: my_assert_not_reached();
      }
    }
    return;
  }
  
  my_assert_not_reached();
}
