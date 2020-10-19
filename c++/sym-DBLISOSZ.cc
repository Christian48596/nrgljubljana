template<typename SC>
class SymmetryDBLISOSZ_tmpl : public SymField_tmpl<SC> {
 private:
   outfield Sz2, Sz, Q12, Q22;
   using Symmetry_tmpl<SC>::P;
   using Symmetry_tmpl<SC>::In;
   using Symmetry_tmpl<SC>::QN;
   
 public:
   using Matrix = typename traits<SC>::Matrix;
   SymmetryDBLISOSZ_tmpl(const Params &P, Allfields &allfields) : SymField_tmpl<SC>(P),
     Sz2(P, allfields, "<Sz^2>", 1), Sz(P, allfields, "<Sz>", 2), Q12(P, allfields, "<Q1^2>", 3), Q22(P, allfields, "<Q2^2>", 4) {
       initInvar({
         {"II1", additive}, // isospin 1
         {"II2", additive}, // isospin 2
         {"SSZ", additive}  // spin projection
       });
       this->InvarSinglet = Invar(1, 1, 0);
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
     return su2_triangle_inequality(I1.get("II1"), I2.get("II1"), I3.get("II1"))
       && su2_triangle_inequality(I1.get("II2"), I2.get("II2"), I3.get("II2")) && u1_equality(I1.get("SSZ"), I2.get("SSZ"), I3.get("SSZ"));
   }

  // Multiplicity of the I=(II1,II2) subspace = (2I1+1)(2I2+1) = II1 II2.
  size_t mult(const Invar &I) const override { return I.get("II1") * I.get("II2"); }

  // We always must have I1 >= 0 and I2 >= 0.
  bool Invar_allowed(const Invar &I) const override { return (I.get("II1") > 0) && (I.get("II2") > 0); }

  // This function supports isospin doublets with respect to either first or the
  // second isospin quantum number. Since the doublet operators are either doublets
  // with respect to one or another, this function should always return a non-zero
  // value!
  double specdens_factor(const Invar &Ip, const Invar &I1) const override {
    check_abs_diff(Ip, I1, "SSZ", 1);
    const Ispin ii1p = Ip.get("II1");
    const Ispin ii11 = I1.get("II1");
    const Ispin ii2p = Ip.get("II2");
    const Ispin ii21 = I1.get("II2");
    if (abs(ii11 - ii1p) == 1) {
      const double isofactor1 = (ii11 == ii1p + 1 ? ISO(ii1p) + 1.0 : ISO(ii1p));
      my_assert(ii2p == ii21);
      my_assert(ii2p >= 1);
      const double isofactor2 = ii2p; // multiplicity wrt 2nd isospin quantum number
      return isofactor1 * isofactor2;
    }
    if (abs(ii21 - ii2p) == 1) {
      const double isofactor1 = (ii21 == ii2p + 1 ? ISO(ii2p) + 1.0 : ISO(ii2p));
      my_assert(ii1p == ii11);
      my_assert(ii1p >= 1);
      const double isofactor2 = ii1p;
      return isofactor1 * isofactor2;
    }
    my_assert_not_reached();
  }

  void load() override {
    switch (P.channels) {
      case 2:
#include "dblisosz/dblisosz-2ch-In2.dat"
#include "dblisosz/dblisosz-2ch-QN.dat"
        break;
      default: my_assert_not_reached();
    }
  }

  void calculate_TD(const Step &step, const DiagInfo_tmpl<SC> &diag, const Stats &stats, const double factor) override {
    bucket trSZ, trSZ2; // Tr[S_z], Tr[S_z^2]
    bucket trIZ12;      // Tr[I1_z^2]
    bucket trIZ22;      // Tr[I2_z^2]
    for (const auto &[I, eig]: diag) {
      const Number ii1  = I.get("II1");
      const Number ii2  = I.get("II2");
      const SZspin ssz  = I.get("SSZ");
      const double sumZ = this->calculate_Z(I, eig, factor);
      trSZ += sumZ * SZ(ssz);
      trSZ2 += sumZ * sqr(SZ(ssz)); // isospin multiplicity contained in sumZ
      trIZ12 += sumZ * (ii1 * ii1 - 1) / 12.;
      trIZ22 += sumZ * (ii2 * ii2 - 1) / 12.;
    }
    Sz  = trSZ / stats.Z;
    Sz2 = trSZ2 / stats.Z;
    Q12 = (4 * trIZ12) / stats.Z;
    Q22 = (4 * trIZ22) / stats.Z;
  }

  DECL;
  HAS_DOUBLET;
  HAS_GLOBAL; // HAS_TRIPLET;
};

#undef OFFDIAG
#define OFFDIAG(i, j, ch, factor0) offdiag_function(step, i, j, ch, 0, t_matel(factor0) * coef.xi(step.N(), ch), h, qq, In, opch)

template<typename SC>
void SymmetryDBLISOSZ_tmpl<SC>::make_matrix(Matrix &h, const Step &step, const Rmaxvals &qq, const Invar &I, const InvarVec &In, const Opch_tmpl<SC> &opch, const Coef_tmpl<SC> &coef) {
  switch (P.channels) {
    case 2:
#include "dblisosz/dblisosz-2ch-offdiag.dat"
      break;
    default: my_assert_not_reached();
  }
}

#include "nrg-recalc-DBLISOSZ.cc"
