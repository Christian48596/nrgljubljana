class SymmetryQS : public Symmetry {
  private:
  outfield Sz2, Q, Q2;

  public:
  SymmetryQS() : Symmetry() { all_syms["QS"] = this; }

  void init() override {
    Sz2.set("<Sz^2>", 1);
    Q.set("<Q>", 2);
    Q2.set("<Q^2>", 3);
    InvarStructure InvStruc[] = {
       {"Q", additive}, // charge
       {"SS", additive} // spin
    };
    initInvar(InvStruc, ARRAYLENGTH(InvStruc));
    InvarSinglet = Invar(0, 1);
    Invar_f      = Invar(1, 2);
  }

  // Multiplicity of the (Q,SS) subspace is 2S+1 = SS.
  int mult(const Invar &I) override { return I.get("SS"); }

  bool triangle_inequality(const Invar &I1, const Invar &I2, const Invar &I3) override {
    return u1_equality(I1.get("Q"), I2.get("Q"), I3.get("Q")) && su2_triangle_inequality(I1.get("SS"), I2.get("SS"), I3.get("SS"));
  }

  bool Invar_allowed(const Invar &I) override {
    const bool spin_ok = I.get("SS") > 0;
    return spin_ok;
  }

  void load() override {
    if (!substeps) {
      switch (channels) {
        case 1:
#include "qs/qs-1ch-In2.dat"
#include "qs/qs-1ch-QN.dat"
          break;

        case 2:
#include "qs/qs-2ch-In2.dat"
#include "qs/qs-2ch-QN.dat"
          break;

        case 3:
#include "qs/qs-3ch-In2.dat"
#include "qs/qs-3ch-QN.dat"
          break;

        case 4:
#include "qs/qs-4ch-In2.dat"
#include "qs/qs-4ch-QN.dat"
          break;

        default: my_assert_not_reached();
      } // switch
    } else {
#include "qs/qs-1ch-In2.dat"
#include "qs/qs-1ch-QN.dat"
    } // if
  }   // load

  double dynamicsusceptibility_factor(const Invar &Ip, const Invar &I1) override {
    check_diff(Ip, I1, "Q", 0);
    const Sspin ssp = Ip.get("SS");
    const Sspin ss1 = I1.get("SS");
    my_assert((abs(ss1 - ssp) == 2 || ss1 == ssp));
    return switch3(ss1, ssp + 2, 1. + (ssp - 1) / 3., ssp, ssp / 3., ssp - 2, (-2. + ssp) / 3.);
  }

  double specdens_factor(const Invar &Ip, const Invar &I1) override {
    check_diff(Ip, I1, "Q", 1);
    const Sspin ssp = Ip.get("SS");
    const Sspin ss1 = I1.get("SS");
    my_assert(abs(ss1 - ssp) == 1);
    return (ss1 == ssp + 1 ? S(ssp) + 1.0 : S(ssp));
  }

  void calculate_TD(const DiagInfo &diag, double factor) override {
    bucket trSZ, trQ, trQ2; // Tr[S_z^2], Tr[Q], Tr[Q^2]

    for (const auto &[I, eig]: diag) {
      const Sspin ss    = I.get("SS");
      const Number q    = I.get("Q");
      const double sumZ = calculate_Z(I, eig, factor);

      trQ += sumZ * q;
      trQ2 += sumZ * q * q;
      trSZ += sumZ * (ss * ss - 1) / 12.;
    }

    Sz2 = trSZ / stats.Z;
    Q   = trQ / stats.Z;
    Q2  = trQ2 / stats.Z;
  }

  DECL;
  HAS_DOUBLET;
  HAS_TRIPLET;
  HAS_GLOBAL;
  HAS_SUBSTEPS;

  void show_coefficients() override;
};

Symmetry *SymQS = new SymmetryQS;

// *** Helper macros for makematrix() members in matrix.cc
#undef OFFDIAG
#define OFFDIAG(i, j, ch, factor0) offdiag_function(i, j, ch, 0, t_matel(factor0) * xi(stats.N, ch), h, qq, In, opch)

/* i - subspace index
   ch - channel (0 or 1)
   number - number of electrons added in channel 'ch' in subspace 'i' */
#undef DIAG
#define DIAG(i, ch, number) diag_function(i, ch, number, zeta(stats.N + 1, ch), h, qq)

#undef OFFDIAG_MIX
#define OFFDIAG_MIX(i, j, ch, factor) offdiag_function(i, j, ch, 0, t_matel(factor) * xiR(stats.N, ch), h, qq, In, opch)

#undef RUNGHOP
#define RUNGHOP(i, j, factor) diag_offdiag_function(i, j, 0, t_matel(factor) * zetaR(stats.N + 1, 0), h, qq)

ATTRIBUTE_NO_SANITIZE_DIV_BY_ZERO // avoid false positives
void SymmetryQS::makematrix(Matrix &h, const Rmaxvals &qq, const Invar &I, const InvarVec &In, const Opch &opch) {
  Sspin ss = I.get("SS");

  if (!substeps) {
    switch (channels) {
      case 1:
#include "qs/qs-1ch-offdiag.dat"
#include "qs/qs-1ch-diag.dat"
        break;

      case 2:
#include "qs/qs-2ch-diag.dat"
#include "qs/qs-2ch-offdiag.dat"
        if (P.rungs) {
#include "qs/qs-2ch-offdiag-mix.dat"
#include "qs/qs-2ch-runghop.dat"
        }
        break;

      case 3:
#include "qs/qs-3ch-diag.dat"
#include "qs/qs-3ch-offdiag.dat"
        break;

      case 4:
#include "qs/qs-4ch-diag.dat"
#include "qs/qs-4ch-offdiag.dat"
        break;

      default: my_assert_not_reached();
    }
  } else {
    my_assert(P.coeffactor == 1);
    int Ntrue, M;
    tie(Ntrue, M) = get_Ntrue_M(stats.N);

// Here we need scale_fix, because SCALE() function is different from
// the convention for rescaling in regular two-channel cases.
#undef OFFDIAG
#define OFFDIAG(i, j, ch, factor0) offdiag_function(i, j, M, 0, t_matel(factor0) * xi(Ntrue, M) / scale_fix(stats.N), h, qq, In, opch)

// No scale_fix here, because SCALE() is defined as it should be.
#undef DIAG
#define DIAG(i, ch, number) diag_function(i, M, number, zeta(Ntrue + 1, M), h, qq)

#include "qs/qs-1ch-offdiag.dat"
#include "qs/qs-1ch-diag.dat"

    if (P.rungs) my_error("Not implemented.");
  }
}

void SymmetryQS::show_coefficients() {
  if (P.rungs)
    for (unsigned int i = 0; i < P.channels; i++)
      cout << "[" << i + 1 << "]"
           << " xi_rung(" << stats.N << ")=" << xiR(stats.N, i) << " zeta_rung(" << stats.N + 1 << ")=" << zetaR(stats.N + 1, i) << endl;
}

#include "nrg-recalc-QS.cc"
