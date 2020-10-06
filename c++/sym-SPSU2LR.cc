class SymmetrySPSU2LR : public SymLR {
 private:
   outfield Sz2;

 public:
   template<typename ... Args> SymmetrySPSU2LR(Args&& ... args) : SymLR(std::forward<Args>(args)...),
     Sz2(P, allfields, "<Sz^2>", 1) {
       InvarStructure InvStruc[] = {
         {"SS", additive},     // spin
         {"P", multiplicative} // parity
       };
       initInvar(InvStruc, ARRAYLENGTH(InvStruc));
       InvarSinglet = Invar(0, 1);
     }

  size_t mult(const Invar &I) const override { return I.get("SS"); }

  bool Invar_allowed(const Invar &I) override { return I.get("SS") > 0; }

  bool triangle_inequality(const Invar &I1, const Invar &I2, const Invar &I3) override {
    return su2_triangle_inequality(I1.get("SS"), I2.get("SS"), I3.get("SS")) && z2_equality(I1.get("P"), I2.get("P"), I3.get("P"));
  }

  void load() override {
    my_assert(P.channels == 2);
#include "spsu2lr/spsu2lr-2ch-In2.dat"
#include "spsu2lr/spsu2lr-2ch-QN.dat"
  }

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
    return (ss1 == ssp + 1 ? S(ssp) + 1.0 : S(ssp));
  }

  void calculate_TD(const Step &step, const DiagInfo &diag, const Stats &stats, double factor) override {
    bucket trSZ2; // Tr[S_z^2]
    for (const auto &[I, eig]: diag) {
      const Sspin ss    = I.get("SS");
      const double sumZ = calculate_Z(I, eig, factor);
      trSZ2 += sumZ * (ss * ss - 1) / 12.;
    }
    Sz2 = trSZ2 / stats.Z;
  }

  DECL;
  HAS_DOUBLET;
  HAS_TRIPLET;
};

#undef ISOSPINX
#define ISOSPINX(i, j, ch, factor) diag_offdiag_function(step, i, j, ch, t_matel(factor) * 2.0 * delta(step.N() + 1, ch), h, qq)

#undef ANOMALOUS
#define ANOMALOUS(i, j, ch, factor) offdiag_function(step, i, j, ch, 0, t_matel(factor) * kappa(step.N(), ch), h, qq, In, opch)

#undef OFFDIAG
#define OFFDIAG(i, j, ch, factor0) offdiag_function(step, i, j, ch, 0, t_matel(factor0) * xi(step.N(), ch), h, qq, In, opch)

#undef DIAG
#define DIAG(i, ch, number) diag_function(step, i, ch, number, zeta(step.N() + 1, ch), h, qq)

void SymmetrySPSU2LR::makematrix(Matrix &h, const Step &step, const Rmaxvals &qq, const Invar &I, const InvarVec &In, const Opch &opch) {
  my_assert(P.channels == 2);
  Sspin ss = I.get("SS");
#include "spsu2lr/spsu2lr-2ch-diag.dat"
#include "spsu2lr/spsu2lr-2ch-offdiag.dat"
#include "spsu2lr/spsu2lr-2ch-anomalous.dat"
#include "spsu2lr/spsu2lr-2ch-isospinx.dat"
}

#include "nrg-recalc-SPSU2LR.cc"
