// *** WARNING!!! Modify nrg-recalc-ISOLR.cc.m4, not nrg-recalc-ISOLR.cc !!!

// Quantum number dependent recalculation routines
// Rok Zitko, rok.zitko@ijs.si, Feb 2006, June 2006, Nov 2007, May 2008
// This file pertains to (I,S,P) subspaces
// Version for EVEN number of impurities

// m4 macros for nrg-recalc-*.cc files
// Rok Zitko, rok.zitko@ijs.si, 2007-2015

// m4 comment: $2 is length, $3,... are quantum numbers







  





namespace ISO2LR {
#include "iso2lr/iso2lr-2ch-def.dat"
}

double sign(double x) {
  if (x > 0.0) return +1.0;
  if (x < 0.0) return -1.0;
  my_assert_not_reached();
}

// (ISOLR): 8 calls of recalc_f() are necessary: different parities are also possible!

// Driver routine for recalc_f()
Opch SymmetryISO2LR::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Invar I1;

    // NOTE: ii,ss only couples to ii+-1,ss+-1 in general, even for
    // several channels.

    Ispin iip = Ip.get("II");
    Sspin ssp = Ip.get("SS");

    // IMPORTANT NEW ELEMENT: the parity is important here!!
    int pp = Ip.get("P");

    // nn is index n of f_n, the last site in the chain prior to adding
    // the new site (f_{n+1}).
    int NN = step.getnn();

    // Both parities yield non-zero <I+-1/2, S+-1/2, P| a^\mu_\nu
    // |I,S,P'>.  Coefficients *DO* depend on P,P', or more accurately,
    // on whether or not P and P' are the same.
    //
    // Observation: due to reflection symmetry, the coefficient for 'a' and
    // 'b' (2 channels) are either all the same or differ in sign.

    // ****** CASE I: SAME PARITY ******

    I1 = Invar(iip + 1, ssp + 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isoupa.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isoupa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isoupb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isoupb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};

    I1 = Invar(iip + 1, ssp - 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isoupa.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isoupa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isoupb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isoupb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};

    I1 = Invar(iip - 1, ssp + 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isodowna.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isodowna.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isodownb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isodownb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};

    I1 = Invar(iip - 1, ssp - 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isodowna.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isodowna.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isodownb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isodownb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};

    // ****** CASE II: DIFFERENT PARITY ******

    I1 = Invar(iip + 1, ssp + 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isoupdiffa.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isoupdiffa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isoupdiffb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isoupdiffb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};

    I1 = Invar(iip + 1, ssp - 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isoupdiffa.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isoupdiffa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isoupdiffb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isoupdiffb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};

    I1 = Invar(iip - 1, ssp + 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isodowndiffa.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isodowndiffa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spinup-isodowndiffb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spinup-isodowndiffb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};

    I1 = Invar(iip - 1, ssp - 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isodowndiffa.dat" << ", ch=" << 0 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isodowndiffa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "iso2lr/iso2lr-2ch-spindown-isodowndiffb.dat" << ", ch=" << 1 << ", len=" << ISO2LR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "iso2lr/iso2lr-2ch-spindown-isodowndiffb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1, Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ISO2LR::LENGTH_I_2CH);
  }
};
  }
  return opch;
}

// Recalculate matrix elements of a doublet tensor operator [EVEN PARITY]
MatrixElements SymmetryISO2LR::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Ispin ii1 = I1.get("II");
    Sspin ss1 = I1.get("SS");
    int p1    = I1.get("P");
    Invar Ip;

    Ip = Invar(ii1 - 1, ss1 + 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "iso2lr/iso2lr-2ch-doubletmp.dat" << ", len=" << ISO2LR::LENGTH_D_2CH << ", Iop=" << Invar(2, 2, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "iso2lr/iso2lr-2ch-doubletmp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_D_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ISO2LR::LENGTH_D_2CH, Invar(2, 2, +1));
  }
};

    Ip = Invar(ii1 - 1, ss1 - 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "iso2lr/iso2lr-2ch-doubletmm.dat" << ", len=" << ISO2LR::LENGTH_D_2CH << ", Iop=" << Invar(2, 2, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "iso2lr/iso2lr-2ch-doubletmm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_D_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ISO2LR::LENGTH_D_2CH, Invar(2, 2, +1));
  }
};

    Ip = Invar(ii1 + 1, ss1 + 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "iso2lr/iso2lr-2ch-doubletpp.dat" << ", len=" << ISO2LR::LENGTH_D_2CH << ", Iop=" << Invar(2, 2, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "iso2lr/iso2lr-2ch-doubletpp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_D_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ISO2LR::LENGTH_D_2CH, Invar(2, 2, +1));
  }
};

    Ip = Invar(ii1 + 1, ss1 - 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "iso2lr/iso2lr-2ch-doubletpm.dat" << ", len=" << ISO2LR::LENGTH_D_2CH << ", Iop=" << Invar(2, 2, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "iso2lr/iso2lr-2ch-doubletpm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_D_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ISO2LR::LENGTH_D_2CH, Invar(2, 2, +1));
  }
};
  }
  return cnew;
}

// Recalculate matrix elements of a triplet tensor operator [EVEN PARITY]
MatrixElements SymmetryISO2LR::recalc_triplet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Ispin ii1 = I1.get("II");
    Sspin ss1 = I1.get("SS");
    int p1    = I1.get("P");
    Invar Ip;

    Ip = Invar(ii1, ss1, p1);
    {
  nrglog('f', "RECALC(fn=" << "iso2lr/iso2lr-2ch-triplets.dat" << ", len=" << ISO2LR::LENGTH_T0_2CH << ", Iop=" << Invar(1, 3, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "iso2lr/iso2lr-2ch-triplets.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_T0_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ISO2LR::LENGTH_T0_2CH, Invar(1, 3, +1));
  }
};

    Ip = Invar(ii1, ss1 + 2, p1);
    {
  nrglog('f', "RECALC(fn=" << "iso2lr/iso2lr-2ch-tripletp.dat" << ", len=" << ISO2LR::LENGTH_Tpm_2CH << ", Iop=" << Invar(1, 3, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "iso2lr/iso2lr-2ch-tripletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_Tpm_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ISO2LR::LENGTH_Tpm_2CH, Invar(1, 3, +1));
  }
};

    Ip = Invar(ii1, ss1 - 2, p1);
    {
  nrglog('f', "RECALC(fn=" << "iso2lr/iso2lr-2ch-tripletm.dat" << ", len=" << ISO2LR::LENGTH_Tpm_2CH << ", Iop=" << Invar(1, 3, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "iso2lr/iso2lr-2ch-tripletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == ISO2LR::LENGTH_Tpm_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ISO2LR::LENGTH_Tpm_2CH, Invar(1, 3, +1));
  }
};
  }
  return cnew;
}
