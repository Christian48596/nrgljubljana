class SymmetryQSZTZ : public Symmetry {
  private:
  outfield Sz, Sz2, Tz, Tz2, Q, Q2;

  public:
  SymmetryQSZTZ() : Symmetry() { all_syms["QSZTZ"] = this; }

  void init() override {
    Sz.set("<Sz>", 1);
    Sz2.set("<Sz^2>", 2);
    Tz.set("<Tz>", 3);
    Tz2.set("<Tz^2>", 4);
    Q.set("<Q>", 5);
    Q2.set("<Q^2>", 6);
    InvarStructure InvStruc[] = {
       {"Q", additive},  // charge
       {"SZ", additive}, // spin
       {"TZ", additive}  // angular momentum
    };
    initInvar(InvStruc, ARRAYLENGTH(InvStruc));
    InvarSinglet = Invar(0, 0, 0);
    Invar_f      = Invar(1, 2, 1);
  }

  size_t mult(const Invar &I) const override { return 1; }

  bool triangle_inequality(const Invar &I1, const Invar &I2, const Invar &I3) override {
    return u1_equality(I1.get("Q"), I2.get("Q"), I3.get("Q")) && u1_equality(I1.get("SZ"), I2.get("SZ"), I3.get("SZ"))
       && u1_equality(I1.get("TZ"), I2.get("TZ"), I3.get("TZ"));
  }

  bool Invar_allowed(const Invar &I) override { return true; }

  void load() override {
    my_assert(!substeps);
    my_assert(channels == 3);
#include "qsztz/qsztz-In2.dat"
#include "qsztz/qsztz-QN.dat"
  } // load

  void calculate_TD(const Step &step, const DiagInfo &diag, double factor) override {
    bucket trSZ, trSZ2, trTZ, trTZ2, trQ, trQ2;

    for (const auto &[I, eig]: diag) {
      const Number q    = I.get("Q");
      const Sspin ssz   = I.get("SZ");
      const Tangmom tz  = I.get("TZ");
      const double sumZ = calculate_Z(I, eig, factor);

      trQ += sumZ * q;
      trQ2 += sumZ * q * q;
      trSZ += sumZ * (ssz - 1) / 2.;
      trSZ2 += sumZ * pow((ssz - 1) / 2., 2);
      trTZ += sumZ * tz;
      trTZ2 += sumZ * tz * tz;
    }

    Sz  = trSZ / stats.Z;
    Sz2 = trSZ2 / stats.Z;
    Tz  = trTZ / stats.Z;
    Tz2 = trTZ2 / stats.Z;
    Q   = trQ / stats.Z;
    Q2  = trQ2 / stats.Z;
  }

  DECL;
  HAS_DOUBLET;
  HAS_TRIPLET;
};

Symmetry *SymQSZTZ = new SymmetryQSZTZ;

// We take the coefficients of the first channel (indexed as 0),
// because all three set are exactly the same due to orbital
// symmetry. [XXXX Can be generalized for symmetry-broken cases.]
#undef OFFDIAG
#define OFFDIAG(i, j, factor0) offdiag_function(step, i, j, 0, 0, t_matel(factor0) * xi(step.N(), 0), h, qq, In, opch)

#undef DIAG
#define DIAG(i, number) diag_function(step, i, 0, number, zeta(step.N() + 1, 0), h, qq)

void SymmetryQSZTZ::makematrix(Matrix &h, const Step &step, const Rmaxvals &qq, const Invar &I, const InvarVec &In, const Opch &opch) {
  my_assert(!substeps);
  my_assert(channels == 3);

#include "qsztz/qsztz-offdiag.dat"
#include "qsztz/qsztz-diag.dat"
}

#include "nrg-recalc-QSZTZ.cc"
