// *** WARNING!!! Modify nrg-recalc-ISOSZLR.cc.m4, not nrg-recalc-ISOSZLR.cc !!!

// Quantum number dependent recalculation routines
// Rok Zitko, rok.zitko@ijs.si, June 2009
// This file pertains to (I,Sz,P) subspaces

// m4 macros for nrg-recalc-*.cc files
// Rok Zitko, rok.zitko@ijs.si, 2007-2020










  





// Driver routine for recalc_f()
Opch SymmetryISOSZLR::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Invar I1;

    Ispin iip   = Ip.get("II");
    SZspin sszp = Ip.get("SSZ");
    int pp      = Ip.get("P");

    // nn is index n of f_n, the last site in the chain prior to adding
    // the new site (f_{n+1}).
    int NN = step.getnn();

    // ****** CASE I: SAME PARITY ******

    I1 = Invar(iip + 1, sszp + 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isoupa.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isoupa.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isoupb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isoupb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};

    I1 = Invar(iip + 1, sszp - 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isoupa.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isoupa.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isoupb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isoupb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};

    I1 = Invar(iip - 1, sszp + 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isodowna.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isodowna.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isodownb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isodownb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};

    I1 = Invar(iip - 1, sszp - 1, pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isodowna.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isodowna.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isodownb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isodownb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};

    // ****** CASE II: DIFFERENT PARITY ******

    I1 = Invar(iip + 1, sszp + 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isoupdiffa.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isoupdiffa.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isoupdiffb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isoupdiffb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};

    I1 = Invar(iip + 1, sszp - 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isoupdiffa.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isoupdiffa.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isoupdiffb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isoupdiffb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};

    I1 = Invar(iip - 1, sszp + 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isodowndiffa.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isodowndiffa.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spinup-isodowndiffb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spinup-isodowndiffb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};

    I1 = Invar(iip - 1, sszp - 1, -pp);
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isodowndiffa.dat" << ", ch=" << 0 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isodowndiffa.dat"
      };
      opch[0][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[0][0][II] = Matrix(0,0); // ???
    }
  }
};
    {
  nrglog('f', "RECALC_F(fn=" << "isoszlr/isoszlr-2ch-spindown-isodowndiffb.dat" << ", ch=" << 1 << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip) && recalc_f_coupled(I1, Ip, Invar_f)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc_f recalc_table[] = {
#include "isoszlr/isoszlr-2ch-spindown-isodowndiffb.dat"
      };
      opch[1][0][II] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table));
    } else {
      opch[1][0][II] = Matrix(0,0); // ???
    }
  }
};
  }
  return opch;
}

// Recalculate matrix elements of a doublet tensor operator [EVEN PARITY]
MatrixElements SymmetryISOSZLR::recalc_doublet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Ispin ii1   = I1.get("II");
    SZspin ssz1 = I1.get("SSZ");
    int p1      = I1.get("P");
    Invar Ip;

    Ip = Invar(ii1 - 1, ssz1 + 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "isoszlr/isoszlr-2ch-doubletmp.dat" << ", Iop=" << Invar(2, -1, +1) << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc recalc_table[] = {
#include "isoszlr/isoszlr-2ch-doubletmp.dat"
      };
      cnew[II] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table), Invar(2, -1, +1));
    } else {
      cnew[II] = Matrix(0,0); // ???
    }
  }
};

    Ip = Invar(ii1 - 1, ssz1 - 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "isoszlr/isoszlr-2ch-doubletmm.dat" << ", Iop=" << Invar(2, +1, +1) << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc recalc_table[] = {
#include "isoszlr/isoszlr-2ch-doubletmm.dat"
      };
      cnew[II] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table), Invar(2, +1, +1));
    } else {
      cnew[II] = Matrix(0,0); // ???
    }
  }
};

    Ip = Invar(ii1 + 1, ssz1 + 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "isoszlr/isoszlr-2ch-doubletpp.dat" << ", Iop=" << Invar(2, -1, +1) << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc recalc_table[] = {
#include "isoszlr/isoszlr-2ch-doubletpp.dat"
      };
      cnew[II] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table), Invar(2, -1, +1));
    } else {
      cnew[II] = Matrix(0,0); // ???
    }
  }
};

    Ip = Invar(ii1 + 1, ssz1 - 1, p1);
    {
  nrglog('f', "RECALC(fn=" << "isoszlr/isoszlr-2ch-doubletpm.dat" << ", Iop=" << Invar(2, +1, +1) << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc recalc_table[] = {
#include "isoszlr/isoszlr-2ch-doubletpm.dat"
      };
      cnew[II] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table), Invar(2, +1, +1));
    } else {
      cnew[II] = Matrix(0,0); // ???
    }
  }
};
  }
  return cnew;
}

// Recalculate matrix elements of a triplet tensor operator [EVEN PARITY]
MatrixElements SymmetryISOSZLR::recalc_triplet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  for(const auto &[I1, eig]: diag) {
    Ispin ii1   = I1.get("II");
    SZspin ssz1 = I1.get("SSZ");
    int p1      = I1.get("P");
    Invar Ip;

    Ip = Invar(ii1, ssz1, p1);
    {
  nrglog('f', "RECALC(fn=" << "isoszlr/isoszlr-2ch-triplets.dat" << ", Iop=" << Invar(1, 0, +1) << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc recalc_table[] = {
#include "isoszlr/isoszlr-2ch-triplets.dat"
      };
      cnew[II] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table), Invar(1, 0, +1));
    } else {
      cnew[II] = Matrix(0,0); // ???
    }
  }
};

    Ip = Invar(ii1, ssz1 + 2, p1);
    {
  nrglog('f', "RECALC(fn=" << "isoszlr/isoszlr-2ch-tripletp.dat" << ", Iop=" << Invar(1, -2, +1) << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc recalc_table[] = {
#include "isoszlr/isoszlr-2ch-tripletp.dat"
      };
      cnew[II] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table), Invar(1, -2, +1));
    } else {
      cnew[II] = Matrix(0,0); // ???
    }
  }
};

    Ip = Invar(ii1, ssz1 - 2, p1);
    {
  nrglog('f', "RECALC(fn=" << "isoszlr/isoszlr-2ch-tripletm.dat" << ", Iop=" << Invar(1, +2, +1) << ")");
  auto II = Twoinvar(I1, Ip);
  if (diag.count(I1) && diag.count(Ip)) {
    if (diag.at(I1).getnrstored() && diag.at(Ip).getnrstored()) {
      struct Recalc recalc_table[] = {
#include "isoszlr/isoszlr-2ch-tripletm.dat"
      };
      cnew[II] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, ARRAYLENGTH(recalc_table), Invar(1, +2, +1));
    } else {
      cnew[II] = Matrix(0,0); // ???
    }
  }
};
  }
  return cnew;
}
