// *** WARNING!!! Modify nrg-recalc-QSZLR.cc.m4, not nrg-recalc-QSZLR.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, June 2006
// This file pertains to (Q,Sz,P) subspaces

namespace QSZLR {
#include "qszlr/qszlr-2ch-def.dat"
}

// m4 macros for nrg-recalc-*.cc files
// Rok Zitko, rok.zitko@ijs.si, 2007-2015

// m4 comment: $2 is length, $3,... are quantum numbers







  





// Driver routine for recalc_f()
Opch SymmetryQSZLR::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp   = Ip.get("Q");
    SZspin sszp = Ip.get("SSZ");
    int pp      = Ip.get("P");

    // NOTE: q,ssz only couples to q+1,ssz+-1 in general, even for
    // several channels.

    // ****** CASE I: SAME PARITY ******

    Invar I1;
    I1 = Invar(qp + 1, sszp + 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spinupa.dat" << ", ch=" << 0 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spinupa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spinupb.dat" << ", ch=" << 1 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spinupb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};

    I1 = Invar(qp + 1, sszp - 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spindowna.dat" << ", ch=" << 0 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spindowna.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spindownb.dat" << ", ch=" << 1 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spindownb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};

    // ****** CASE II: DIFFERENT PARITY ******

    I1 = Invar(qp + 1, sszp + 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spinupdiffa.dat" << ", ch=" << 0 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spinupdiffa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spinupdiffb.dat" << ", ch=" << 1 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spinupdiffb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};

    I1 = Invar(qp + 1, sszp - 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spindowndiffa.dat" << ", ch=" << 0 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spindowndiffa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "qszlr/qszlr-2ch-spindowndiffb.dat" << ", ch=" << 1 << ", len=" << QSZLR::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qszlr/qszlr-2ch-spindowndiffb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZLR::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZLR::LENGTH_I_2CH);
  }
};
  }
  return opch;
}
