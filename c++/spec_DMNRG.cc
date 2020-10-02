// OPTIMIZATION NOTE: the inner loop should involve the last index.

class SPEC_DMNRG : public SPEC {
  public:
  spCS_t make_cs(const BaseSpectrum &) override { return make_shared<ChainSpectrumBinning>(); }
  void calc(const Step &step, const Eigen &, const Eigen &, const Matrix &, const Matrix &, const BaseSpectrum &, t_factor, spCS_t, const Invar &,
            const Invar &, DensMatElements &) override;
  string name() override { return "DMNRG"; }
  string merge() override { return "NN2"; }
  string rho_type() override { return "rho"; }
};

void SPEC_DMNRG::calc(const Step &step, const Eigen &diagIp, const Eigen &diagI1, const Matrix &op1II, const Matrix &op2II, const BaseSpectrum &bs, t_factor spinfactor,
                      spCS_t cs, const Invar &Ip, const Invar &I1, DensMatElements &rho) {
  double sign = (bs.mt == matstype::bosonic ? S_BOSONIC : S_FERMIONIC);
  double Emin = getEmin(); // used in optimization
  double Emax = getEmax();
  if (P.ZBW) {
    Emin = 0;
    Emax = std::numeric_limits<double>::max(); // infinity
  }
  const Matrix &rhoNIp = rho[Ip];
  const Matrix &rhoNI1 = rho[I1];
  auto dimp            = min(rhoNIp.size1(), diagIp.getnr());
  auto dim1            = min(rhoNI1.size1(), diagI1.getnr());
  for (size_t rm = 0; rm < dimp; rm++) {
    const t_eigen Em = diagIp.value(rm);
    for (size_t rj = 0; rj < dim1; rj++) {
      const t_eigen Ej = diagI1.value(rj);
      DELTA d;
      d.energy          = Ej - Em;
      const double absE = abs(d.energy);
      if (absE < Emin || absE > Emax) // does not contribute
        continue;
      weight_bucket sumA;
      for (size_t ri = 0; ri < dimp; ri++) sumA += op2II(rj, ri) * rhoNIp(rm, ri); // rm <-> ri, rho symmetric
      t_weight weightA = t_weight(sumA) * CONJ_ME(op1II(rj, rm));
      weight_bucket sumB;
      for (size_t ri = 0; ri < dim1; ri++) sumB += CONJ_ME(op1II(ri, rm)) * rhoNI1(rj, ri); // non-optimal
      t_weight weightB = t_weight(sumB) * op2II(rj, rm);
      d.weight         = spinfactor * (weightA + (-sign) * weightB);
      cs->add(step.scale() * d.energy, d.weight);
    }
  }
}

class SPEC_DMNRGmats : public SPEC {
  public:
  spCS_t make_cs(const BaseSpectrum &bs) override { return make_shared<ChainSpectrumMatsubara>(bs.mt); }
  void calc(const Step &step, const Eigen &, const Eigen &, const Matrix &, const Matrix &, const BaseSpectrum &, t_factor, spCS_t, const Invar &,
            const Invar &, DensMatElements &) override;
  string name() override { return "DMNRGmats"; }
  string rho_type() override { return "rho"; }
};

void SPEC_DMNRGmats::calc(const Step &step, const Eigen &diagIp, const Eigen &diagI1, const Matrix &op1II, const Matrix &op2II, const BaseSpectrum &bs,
                          t_factor spinfactor, spCS_t cs, const Invar &Ip, const Invar &I1, DensMatElements &rho) {
  double sign = (bs.mt == matstype::bosonic ? S_BOSONIC : S_FERMIONIC);
  auto csm   = dynamic_pointer_cast<ChainSpectrumMatsubara>(cs);
  const Matrix &rhoNIp = rho[Ip];
  const Matrix &rhoNI1 = rho[I1];
  auto dimp            = min(rhoNIp.size1(), diagIp.getnr());
  auto dim1            = min(rhoNI1.size1(), diagI1.getnr());
  for (size_t rm = 0; rm < dimp; rm++) {
    const t_eigen Em = diagIp.value(rm);
    for (size_t rj = 0; rj < dim1; rj++) {
      const t_eigen Ej = diagI1.value(rj);
      DELTA d;
      d.energy = Ej - Em;
      weight_bucket sumA;
      for (size_t ri = 0; ri < dimp; ri++) sumA += op2II(rj, ri) * rhoNIp(rm, ri); // rm <-> ri, rho symmetric
      t_weight weightA = t_weight(sumA) * CONJ_ME(op1II(rj, rm));
      weight_bucket sumB;
      for (size_t ri = 0; ri < dim1; ri++) sumB += CONJ_ME(op1II(ri, rm)) * rhoNI1(rj, ri); // non-optimal
      t_weight weightB = t_weight(sumB) * op2II(rj, rm);
      d.weight         = spinfactor * (weightA + (-sign) * weightB);
      for (size_t n = 1; n < P.mats; n++) csm->add(n, d.weight / (cmpl(0, ww(n, bs.mt)) - step.scale() * d.energy));
      if (abs(d.energy) > WEIGHT_TOL || bs.mt == matstype::fermionic)
        csm->add(size_t(0), d.weight / (cmpl(0, ww(0, bs.mt)) - step.scale() * d.energy));
      else // bosonic w=0 && E1=Ep case
        csm->add(size_t(0), spinfactor * (-weightA / t_weight(P.T)));
    }
  }
}
