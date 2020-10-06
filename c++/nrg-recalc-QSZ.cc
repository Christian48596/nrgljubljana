// *** WARNING!!! Modify nrg-recalc-QSZ.cc.m4, not nrg-recalc-QSZ.cc !!!

// Quantum number dependant recalculation routines
// Rok Zitko, rok.zitko@ijs.si, 2006-2020
// This file pertains to (Q,SZ) subspaces

// m4 macros for nrg-recalc-*.cc files
// Rok Zitko, rok.zitko@ijs.si, 2007-2015

// m4 comment: $2 is length, $3,... are quantum numbers







  





namespace QSZ {
#include "qsz/qsz-1ch-def.dat"
#include "qsz/qsz-2ch-def.dat"
#include "qsz/qsz-3ch-def.dat"
}

// NOTE: p is ket (right side), 1 is bra (left side). OP is sandwiched
// in between. Thus Q[p] + Q[op] = Q[1].

// Recalculate matrix elements of a doublet tensor operator
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
    switch (P.channels) {
  case 1: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-1ch-doubletp.dat" << ", len=" << QSZ::LENGTH_D_1CH << ", Iop=" << Invar(1, -1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-1ch-doubletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_1CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_1CH, Invar(1, -1));
  }
} } break;
  case 2: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-2ch-doubletp.dat" << ", len=" << QSZ::LENGTH_D_2CH << ", Iop=" << Invar(1, -1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-2ch-doubletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_2CH, Invar(1, -1));
  }
} } break;
  case 3: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-3ch-doubletp.dat" << ", len=" << QSZ::LENGTH_D_3CH << ", Iop=" << Invar(1, -1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-3ch-doubletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_3CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_3CH, Invar(1, -1));
  }
} } break;
  default: my_assert_not_reached();
  };

    Ip = Invar(q1-1, ssz1-1);
    switch (P.channels) {
  case 1: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-1ch-doubletm.dat" << ", len=" << QSZ::LENGTH_D_1CH << ", Iop=" << Invar(1, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-1ch-doubletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_1CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_1CH, Invar(1, +1));
  }
} } break;
  case 2: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-2ch-doubletm.dat" << ", len=" << QSZ::LENGTH_D_2CH << ", Iop=" << Invar(1, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-2ch-doubletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_2CH, Invar(1, +1));
  }
} } break;
  case 3: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-3ch-doubletm.dat" << ", len=" << QSZ::LENGTH_D_3CH << ", Iop=" << Invar(1, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-3ch-doubletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_3CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_3CH, Invar(1, +1));
  }
} } break;
  default: my_assert_not_reached();
  };
    }      // loop
  } else { // substeps
    for(const auto &[I1, eig]: diag) {
      Number q1   = I1.get("Q");
      SZspin ssz1 = I1.get("SSZ");
      Invar Ip;

      Ip = Invar(q1 - 1, ssz1 + 1);
      {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-1ch-doubletp.dat" << ", len=" << QSZ::LENGTH_D_1CH << ", Iop=" << Invar(1, -1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-1ch-doubletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_1CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_1CH, Invar(1, -1));
  }
};

      Ip = Invar(q1 - 1, ssz1 - 1);
      {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-1ch-doubletm.dat" << ", len=" << QSZ::LENGTH_D_1CH << ", Iop=" << Invar(1, +1) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-1ch-doubletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_D_1CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_D_1CH, Invar(1, +1));
  }
};
    } // loop
  }
  return cnew;
}

// Driver routine for recalc_f()
Opch SymmetryQSZ::recalc_irreduc(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp   = Ip.get("Q");
    SZspin sszp = Ip.get("SSZ");
    Invar I1;

    // NOTE: q,ssz only couples to q+1,ssz+-1 in general, even for
    // several channels.

    I1 = Invar(qp + 1, sszp + 1);
    switch (P.channels) {
  case 1: { {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-1ch-spinupa.dat" << ", ch=" << 0 << ", len=" << QSZ::LENGTH_I_1CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-1ch-spinupa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_1CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_1CH);
  }
} } break;
  case 2: { {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-2ch-spinupa.dat" << ", ch=" << 0 << ", len=" << QSZ::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-2ch-spinupa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_2CH);
  }
};
      	   {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-2ch-spinupb.dat" << ", ch=" << 1 << ", len=" << QSZ::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-2ch-spinupb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_2CH);
  }
} } break;
  case 3: { {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-3ch-spinupa.dat" << ", ch=" << 0 << ", len=" << QSZ::LENGTH_I_3CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-3ch-spinupa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_3CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_3CH);
  }
};
           {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-3ch-spinupb.dat" << ", ch=" << 1 << ", len=" << QSZ::LENGTH_I_3CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-3ch-spinupb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_3CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_3CH);
  }
};
      	   {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-3ch-spinupc.dat" << ", ch=" << 2 << ", len=" << QSZ::LENGTH_I_3CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-3ch-spinupc.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_3CH);
    opch[2][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_3CH);
  }
} } break;
  default: my_assert_not_reached();
  };

    I1 = Invar(qp+1, sszp-1);
    switch (P.channels) {
  case 1: { {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-1ch-spindowna.dat" << ", ch=" << 0 << ", len=" << QSZ::LENGTH_I_1CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-1ch-spindowna.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_1CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_1CH);
  }
} } break;
  case 2: { {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-2ch-spindowna.dat" << ", ch=" << 0 << ", len=" << QSZ::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-2ch-spindowna.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_2CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_2CH);
  }
};
	   {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-2ch-spindownb.dat" << ", ch=" << 1 << ", len=" << QSZ::LENGTH_I_2CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-2ch-spindownb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_2CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_2CH);
  }
} } break;
  case 3: { {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-3ch-spindowna.dat" << ", ch=" << 0 << ", len=" << QSZ::LENGTH_I_3CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-3ch-spindowna.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_3CH);
    opch[0][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_3CH);
  }
};
           {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-3ch-spindownb.dat" << ", ch=" << 1 << ", len=" << QSZ::LENGTH_I_3CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-3ch-spindownb.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_3CH);
    opch[1][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_3CH);
  }
};
      	   {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-3ch-spindownc.dat" << ", ch=" << 2 << ", len=" << QSZ::LENGTH_I_3CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-3ch-spindownc.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_3CH);
    opch[2][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_3CH);
  }
} } break;
  default: my_assert_not_reached();
  };
  } // loop
  return opch;
}

OpchChannel SymmetryQSZ::recalc_irreduc_substeps(const Step &step, const DiagInfo &diag, const QSrmax &qsrmax, const Params &P, int M) {
  Opch opch = newopch(P);
  for(const auto &[Ip, eig]: diag) {
    Number qp   = Ip.get("Q");
    SZspin sszp = Ip.get("SSZ");
    Invar I1;

    I1 = Invar(qp + 1, sszp + 1);
    {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-1ch-spinupa.dat" << ", ch=" << M << ", len=" << QSZ::LENGTH_I_1CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-1ch-spinupa.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_1CH);
    opch[M][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_1CH);
  }
};

    I1 = Invar(qp + 1, sszp - 1);
    {
  nrglog('f', "RECALC_F(fn=" << "qsz/qsz-1ch-spindowna.dat" << ", ch=" << M << ", len=" << QSZ::LENGTH_I_1CH << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include "qsz/qsz-1ch-spindowna.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_I_1CH);
    opch[M][0][Twoinvar(I1,Ip)] = recalc_f(diag, qsrmax, I1, Ip, recalc_table, QSZ::LENGTH_I_1CH);
  }
};
  } // loop
  return opch[M];
}

// Recalculate matrix elements of a triplet tenzor operator
MatrixElements SymmetryQSZ::recalc_triplet(const DiagInfo &diag, const QSrmax &qsrmax, const MatrixElements &cold) {
  MatrixElements cnew;
  if (!P.substeps) {
    for(const auto &[I1, eig]: diag) {
      Number q1   = I1.get("Q");
      SZspin ssz1 = I1.get("SSZ");
      Invar Ip;

      Ip = Invar(q1, ssz1);
    switch (P.channels) {
  case 1: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-1ch-triplets.dat" << ", len=" << QSZ::LENGTH_T0_1CH << ", Iop=" << Invar(0, 0) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-1ch-triplets.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_T0_1CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_T0_1CH, Invar(0, 0));
  }
} } break;
  case 2: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-2ch-triplets.dat" << ", len=" << QSZ::LENGTH_T0_2CH << ", Iop=" << Invar(0, 0) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-2ch-triplets.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_T0_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_T0_2CH, Invar(0, 0));
  }
} } break;
  case 3: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-3ch-triplets.dat" << ", len=" << QSZ::LENGTH_T0_3CH << ", Iop=" << Invar(0, 0) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-3ch-triplets.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_T0_3CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_T0_3CH, Invar(0, 0));
  }
} } break;
  default: my_assert_not_reached();
  };

    Ip = Invar(q1, ssz1+2);
    switch (P.channels) {
  case 1: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-1ch-tripletp.dat" << ", len=" << QSZ::LENGTH_Tpm_1CH << ", Iop=" << Invar(0, -2) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-1ch-tripletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_Tpm_1CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_Tpm_1CH, Invar(0, -2));
  }
} } break;
  case 2: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-2ch-tripletp.dat" << ", len=" << QSZ::LENGTH_Tpm_2CH << ", Iop=" << Invar(0, -2) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-2ch-tripletp.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_Tpm_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_Tpm_2CH, Invar(0, -2));
  }
} } break;
  default: my_assert_not_reached();
  };

    Ip = Invar(q1, ssz1-2);
    switch (P.channels) {
  case 1: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-1ch-tripletm.dat" << ", len=" << QSZ::LENGTH_Tpm_1CH << ", Iop=" << Invar(0, +2) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-1ch-tripletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_Tpm_1CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_Tpm_1CH, Invar(0, +2));
  }
} } break;
  case 2: { {
  nrglog('f', "RECALC(fn=" << "qsz/qsz-2ch-tripletm.dat" << ", len=" << QSZ::LENGTH_Tpm_2CH << ", Iop=" << Invar(0, +2) << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include "qsz/qsz-2ch-tripletm.dat"
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == QSZ::LENGTH_Tpm_2CH);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, QSZ::LENGTH_Tpm_2CH, Invar(0, +2));
  }
} } break;
  default: my_assert_not_reached();
  };
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
      const Twoinvar II = make_pair(I1, I1);
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
      const Twoinvar II = make_pair(I1, I1);
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
      const Twoinvar II = make_pair(I1, I1);
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
