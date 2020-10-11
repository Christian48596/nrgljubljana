class SymmetryISOSZ : public SymField {
  private:
  outfield Sz2, Sz, Q2;

  public:
   template<typename ... Args> SymmetryISOSZ(Args&& ... args) : SymField(std::forward<Args>(args)...),
     Sz2(P, allfields, "<Sz^2>", 1), Sz(P, allfields, "<Sz>", 2), Q2(P, allfields, "<Q^2>", 3) {
       InvarStructure InvStruc[] = {
         {"II", additive}, // isospin
         {"SSZ", additive} // spin projection
       };
       initInvar(InvStruc, ARRAYLENGTH(InvStruc));
       InvarSinglet = Invar(1, 0);
     }

  // Multiplicity of the I=(II,SSZ) subspace = (2I+1) = II.
  size_t mult(const Invar &I) const override {
    return I.get("II"); // isospin multiplicity
  }

  bool check_SPIN(const Invar &I1, const Invar &Ip, const int &SPIN) const override {
    // The spin projection of the operator is defined by the difference
    // in Sz of both the invariant subspaces.
    SZspin ssz1  = I1.get("SSZ");
    SZspin sszp  = Ip.get("SSZ");
    SZspin sszop = ssz1 - sszp;
    return sszop == SPIN;
  }

  bool triangle_inequality(const Invar &I1, const Invar &I2, const Invar &I3) const override {
    return u1_equality(I1.get("SSZ"), I2.get("SSZ"), I3.get("SSZ")) && su2_triangle_inequality(I1.get("II"), I2.get("II"), I3.get("II"));
  }

  // We always must have I >= 0.
  bool Invar_allowed(const Invar &I) const override { return I.get("II") > 0; }

  void load() override {
    switch (P.channels) {
      case 1:
#include "isosz/isosz-1ch-In2.dat"
#include "isosz/isosz-1ch-QN.dat"
        break;
      case 2:
#include "isosz/isosz-2ch-In2.dat"
#include "isosz/isosz-2ch-QN.dat"
        break;
      default: my_assert_not_reached();
    }
  }

  double specdens_factor(const Invar &Ip, const Invar &I1) const override {
    check_abs_diff(Ip, I1, "SSZ", 1);
    const Ispin iip = Ip.get("II");
    const Ispin ii1 = I1.get("II");
    const double isofactor = (ii1 == iip + 1 ? ISO(iip) + 1.0 : ISO(iip));
    return isofactor;
  }

  void calculate_TD(const Step &step, const DiagInfo &diag, const Stats &stats, double factor) override {
    bucket trSZ, trSZ2, trIZ2; // Tr[S_z], Tr[S_z^2], Tr[I_z^2]
    for (const auto &[I, eig]: diag) {
      const Ispin ii    = I.get("II");
      const SZspin ssz  = I.get("SSZ");
      const double sumZ = calculate_Z(I, eig, factor);
      trSZ += sumZ * SZ(ssz);
      trSZ2 += sumZ * sqr(SZ(ssz));        // isospin multiplicity contained in sumZ
      trIZ2 += sumZ * (ii * ii - 1) / 12.; // spin multiplicity contained in sumZ
    }
    Sz  = trSZ / stats.Z;
    Sz2 = trSZ2 / stats.Z;
    Q2  = (4 * trIZ2) / stats.Z;
  }

  DECL;
  HAS_DOUBLET;
  HAS_TRIPLET;
  HAS_GLOBAL;
};

#undef OFFDIAG
#define OFFDIAG(i, j, ch, factor0) offdiag_function(step, i, j, ch, 0, t_matel(factor0) * coef.xi(step.N(), ch), h, qq, In, opch)

void SymmetryISOSZ::make_matrix(Matrix &h, const Step &step, const Rmaxvals &qq, const Invar &I, const InvarVec &In, const Opch &opch, const Coef &coef) {
  Ispin ii = I.get("II");
  int NN   = step.getnn();

  switch (P.channels) {
    case 1:
#include "isosz/isosz-1ch-offdiag.dat"
      break;
    case 2:
#include "isosz/isosz-2ch-offdiag.dat"
      break;
    default: my_assert_not_reached();
  }
}

#include "nrg-recalc-ISOSZ.cc"
