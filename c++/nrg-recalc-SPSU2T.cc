// *** WARNING!!! Modify nrg-recalc-SPSU2T.cc.m4, not nrg-recalc-SPSU2T.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, Aug 2015
// This file pertains to (S,T) subspaces

// m4 macros for nrg-recalc-*.cc files
// Rok Zitko, rok.zitko@ijs.si, 2007-2015

// m4 comment: $2 is length, $3,... are quantum numbers







  





namespace SPSU2T {
#include "spsu2t/spsu2t-def.dat"
}

// Recalculate matrix elements of a doublet tensor operator
MatrixElements SymmetrySPSU2T::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Sspin ss1  = I1.get("SS");
    Tangmom t1 = I1.get("T");
    double T   = t1; // trick!
    Invar Ip;

    // Two different lengths: D_3CH_a and D_3CH_b

    Ip = Invar(ss1 + 1, t1 - 1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-doubletp-1.dat" << ", len=" << SPSU2T::LENGTH_D_3CH_1 << ", Iop=" << Invar(1, 2, 1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-doubletp-1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_D_3CH_1);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_D_3CH_1, Invar(1, 2, 1));
  }
};

    Ip = Invar(ss1 - 1, t1 - 1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-doubletm-1.dat" << ", len=" << SPSU2T::LENGTH_D_3CH_1 << ", Iop=" << Invar(1, 2, 1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-doubletm-1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_D_3CH_1);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_D_3CH_1, Invar(1, 2, 1));
  }
};

    Ip = Invar(ss1 + 1, t1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-doubletp0.dat" << ", len=" << SPSU2T::LENGTH_D_3CH_0 << ", Iop=" << Invar(1, 2, 1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-doubletp0.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_D_3CH_0);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_D_3CH_0, Invar(1, 2, 1));
  }
};

    Ip = Invar(ss1 - 1, t1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-doubletm0.dat" << ", len=" << SPSU2T::LENGTH_D_3CH_0 << ", Iop=" << Invar(1, 2, 1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-doubletm0.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_D_3CH_0);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_D_3CH_0, Invar(1, 2, 1));
  }
};

    Ip = Invar(ss1 + 1, t1 + 1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-doubletp+1.dat" << ", len=" << SPSU2T::LENGTH_D_3CH_1 << ", Iop=" << Invar(1, 2, 1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-doubletp+1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_D_3CH_1);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_D_3CH_1, Invar(1, 2, 1));
  }
};

    Ip = Invar(ss1 - 1, t1 + 1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-doubletm+1.dat" << ", len=" << SPSU2T::LENGTH_D_3CH_1 << ", Iop=" << Invar(1, 2, 1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-doubletm+1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_D_3CH_1);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_D_3CH_1, Invar(1, 2, 1));
  }
};
  }
  return cnew;
}

// ch=1 <-> Tz=+1
// ch=2 <-> Tz=0
// ch=3 <-> Tz=-1

// Driver routine for recalc_f()
Opch SymmetrySPSU2T::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Sspin ssp  = Ip.get("SS");
    Tangmom tp = Ip.get("T");
    double T   = tp; // trick!
    Invar I1;

    // The different files just correspond to contributions computed
    // for various d[CR,sz,tz] operators.
    // Check: there should not be any lines with equal subspaces
    // indexes in different files!! That's indeed the case for the
    // generated files for symtype=SPSU2T.
    I1 = Invar(ssp + 1, tp + 1);
    {
  nrglog('f', "RECALC_F(fn=" << "spsu2t/spsu2t-spinup+1.dat" << ", ch=" << 0 << ", len=" << SPSU2T::LENGTH_I_3CH_0 << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "spsu2t/spsu2t-spinup+1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_I_3CH_0);
    recalc_f(diag, qsrmax, opch[0][0], I1, Ip, recalc_table, SPSU2T::LENGTH_I_3CH_0);
  }
};

    I1 = Invar(ssp + 1, tp);
    {
  nrglog('f', "RECALC_F(fn=" << "spsu2t/spsu2t-spinup0.dat" << ", ch=" << 0 << ", len=" << SPSU2T::LENGTH_I_3CH_1 << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "spsu2t/spsu2t-spinup0.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_I_3CH_1);
    recalc_f(diag, qsrmax, opch[0][0], I1, Ip, recalc_table, SPSU2T::LENGTH_I_3CH_1);
  }
};

    I1 = Invar(ssp + 1, tp - 1);
    {
  nrglog('f', "RECALC_F(fn=" << "spsu2t/spsu2t-spinup-1.dat" << ", ch=" << 0 << ", len=" << SPSU2T::LENGTH_I_3CH_2 << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "spsu2t/spsu2t-spinup-1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_I_3CH_2);
    recalc_f(diag, qsrmax, opch[0][0], I1, Ip, recalc_table, SPSU2T::LENGTH_I_3CH_2);
  }
};

    I1 = Invar(ssp - 1, tp + 1);
    {
  nrglog('f', "RECALC_F(fn=" << "spsu2t/spsu2t-spindo+1.dat" << ", ch=" << 0 << ", len=" << SPSU2T::LENGTH_I_3CH_0 << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "spsu2t/spsu2t-spindo+1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_I_3CH_0);
    recalc_f(diag, qsrmax, opch[0][0], I1, Ip, recalc_table, SPSU2T::LENGTH_I_3CH_0);
  }
};

    I1 = Invar(ssp - 1, tp);
    {
  nrglog('f', "RECALC_F(fn=" << "spsu2t/spsu2t-spindo0.dat" << ", ch=" << 0 << ", len=" << SPSU2T::LENGTH_I_3CH_1 << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "spsu2t/spsu2t-spindo0.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_I_3CH_1);
    recalc_f(diag, qsrmax, opch[0][0], I1, Ip, recalc_table, SPSU2T::LENGTH_I_3CH_1);
  }
};

    I1 = Invar(ssp - 1, tp - 1);
    {
  nrglog('f', "RECALC_F(fn=" << "spsu2t/spsu2t-spindo-1.dat" << ", ch=" << 0 << ", len=" << SPSU2T::LENGTH_I_3CH_2 << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "spsu2t/spsu2t-spindo-1.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_I_3CH_2);
    recalc_f(diag, qsrmax, opch[0][0], I1, Ip, recalc_table, SPSU2T::LENGTH_I_3CH_2);
  }
};
  }
  return opch;
}

// Recalculate matrix elements of a triplet tenzor operator
MatrixElements SymmetrySPSU2T::recalc_triplet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Sspin ss1  = I1.get("SS");
    Tangmom t1 = I1.get("T");
    double T   = t1; // trick!
    Invar Ip;

    Ip = Invar(ss1, t1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-triplets.dat" << ", len=" << SPSU2T::LENGTH_T0_3CH << ", Iop=" << Invar(3, 0) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-triplets.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_T0_3CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_T0_3CH, Invar(3, 0));
  }
};

    Ip = Invar(ss1 + 2, t1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-tripletp.dat" << ", len=" << SPSU2T::LENGTH_Tpm_3CH << ", Iop=" << Invar(3, 0) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-tripletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_Tpm_3CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_Tpm_3CH, Invar(3, 0));
  }
};

    Ip = Invar(ss1 - 2, t1);
    {
  nrglog('f', "RECALC(fn=" << "spsu2t/spsu2t-tripletm.dat" << ", len=" << SPSU2T::LENGTH_Tpm_3CH << ", Iop=" << Invar(3, 0) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "spsu2t/spsu2t-tripletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == SPSU2T::LENGTH_Tpm_3CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, SPSU2T::LENGTH_Tpm_3CH, Invar(3, 0));
  }
};
  }
  return cnew;
}
