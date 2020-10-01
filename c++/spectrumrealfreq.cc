// Real-frequency spectral function
class SpectrumRealFreq : public Spectrum {
  private:
  Bins fspos, fsneg; // Full spectral information
  void mergeNN2(spCS_t);
  void mergeCFS(spCS_t);
  void weight_report();
  void trim();
  void savebins();
  void continuous();
  public:
  SpectrumRealFreq(const string &_opname, const string &_filename, SPECTYPE _spectype) : Spectrum(_opname, _filename, _spectype){};
  void merge(spCS_t) override;
  ~SpectrumRealFreq() override;
};

SpectrumRealFreq::~SpectrumRealFreq() {
  cout << "Spectrum: " << opname << " " << spectype->name() << " ->"; // appended in savebins() & continuous()
  trim();
  savebins();
  continuous();
  cout << endl;
  weight_report();
}

/* Merge the spectrum for a finite Wilson chain into the "true" NRG
 spectrum. For complete Fock space NRG calculation, the merging tricks are
 not necessary: we just collect all the delta peaks from all iterations. */
void SpectrumRealFreq::merge(spCS_t cs) {
  if (spectype->merge() == "NN2") return mergeNN2(cs);
  if (spectype->merge() == "CFS") return mergeCFS(cs);
  my_error("should not be reached");
}

// Spectrum merging for complete Fock space calculation.
void SpectrumRealFreq::mergeCFS(spCS_t cs) {
  auto csb = dynamic_pointer_cast<ChainSpectrumBinning>(cs);
  nrglog('*', "weight=" << csb->total_weight());
  fspos.merge(csb->spos);
  fsneg.merge(csb->sneg);
}

// energy scale factor that is exponentiated
// N/N+1 patching is recommended for larger values of Lambda,
// so as to reduce the upper limit of the patching window to
// omega_N*goodE*Lambda, i.e., by a factor of Lambda
// compared to the N/N+2 patching where the upper limit is
// omega_N*goodE*Lambda^2.
double getfactor() {
  double factor;
  if (P.channels == 1 && !P.NN1)
    factor = P.Lambda; // N/N+2 patching
  else
    factor = sqrt(P.Lambda); // N/N+1 patching
  return factor;
}

double getE0() { return P.goodE; }

// Note: in the current iteration, spectral peaks in the [0:Emin] are
// discarded.
double getEmin() { return getE0(); }

// The "peak" energy of the "window function" in the patching procedure.
double getEx() { return getE0() * getfactor(); }

double getEmax() { return getE0() * sqr(getfactor()); }

// Window function: f(0)=0, f(1)=1.
inline double fnc_linear(double x) { return x; }

inline double fnc_tanh_0(double x) { return tanh(P.NNtanh * (x - 0.5)); }

inline double fnc_tanh(double x) {
  const double f0 = fnc_tanh_0(0.0);
  const double fx = fnc_tanh_0(x);
  const double f1 = fnc_tanh_0(1.0);
  return (fx - f0) / (f1 - f0);
}

inline double fnc(double x) {
  if (P.NNtanh > 0.0)
    return fnc_tanh(x);
  else
    return fnc_linear(x);
}

inline double windowfunction(double E, double Emin, double Ex, double Emax) {
  // Exception 1
  if (E <= Ex && LAST_ITERATION()) return 1.0;
  // Exception 2
  if (E >= Ex && FIRST_ITERATION()) return 1.0;
  // Exception 3
  if (P.ZBW) return 1.0;
  if (E <= Emin || E >= Emax) return 0.0;
  if (Emin < E && E <= Ex) return fnc((E - Emin) / (Ex - Emin));
  if (Ex < E && E < Emax) return 1.0 - fnc((E - Ex) / (Emax - Ex));
  my_assert_not_reached();
}

// Here we perform the actual merging of data using the N/N+2 scheme.
// Note that we use a windowfunction (see above) to accomplish the
// smooth combining of data.
void mergeNN2half(Bins &fullspec, const Bins &cs) {
  double Emin = step.scale() * getEmin(); // p
  double Ex   = step.scale() * getEx();   // p Lambda
  double Emax = step.scale() * getEmax(); // p Lambda^2
  if (P.ZBW) {                          // override for zero bandwidth calculation
    Emin = 0;
    Emax = std::numeric_limits<double>::max(); // infinity
  }
  const size_t len = fullspec.bins.size();
  my_assert(len == cs.bins.size()); // We require equivalent bin sets!!
  for (size_t i = 0; i < len; i++) {
    const double energy   = cs.bins[i].first;
    const t_weight weight = cs.bins[i].second;
    if (Emin < energy && energy < Emax && weight != 0.0) {
      const double factor = (P.NN2avg ? 0.5 : 1.0);
      fullspec.bins[i].second += factor * weight * windowfunction(energy, Emin, Ex, Emax);
    }
  }
}

// Spectrum merging using the N/N+n patching. One has to be careful about
// the specifics, in particular the values of Emin, Ex and Emax. The
// current choice seems to be working quite all right.
// See R. Bulla, T. A. Costi, D. Vollhardt, Phys. Rev. B 64, 045103 (2001).

void SpectrumRealFreq::mergeNN2(spCS_t cs) {
  auto csb = dynamic_pointer_cast<ChainSpectrumBinning>(cs);

  nrglog('*', "weight=" << csb->total_weight());
  if (!step.N_for_merging()) return;
  mergeNN2half(fspos, csb->spos);
  mergeNN2half(fsneg, csb->sneg);
}

const double IMAG_TOLERANCE = 1e-10;

void SpectrumRealFreq::weight_report() {
  auto fmt = [](t_weight x) -> string {
    if (abs(x.imag()) < IMAG_TOLERANCE)
      return to_string(x.real());
    else
      return to_string(x);
  };
  const t_weight twneg = fsneg.total_weight();
  const t_weight twpos = fspos.total_weight();
  cout << "[" << opname << "]"
       << " pos=" << fmt(twpos) << " neg=" << fmt(twneg) << " sum= " << fmt(twpos + twneg) << endl;
  // Spectral moments from delta-peaks
  const t_weight mom1 = moment(fsneg.bins, fspos.bins, 1);
  const t_weight mom2 = moment(fsneg.bins, fspos.bins, 2);
  const t_weight mom3 = moment(fsneg.bins, fspos.bins, 3);
  const t_weight mom4 = moment(fsneg.bins, fspos.bins, 4);
  const t_weight f    = fd_fermi(fsneg.bins, fspos.bins, P.T);
  const t_weight b    = fd_bose(fsneg.bins, fspos.bins, P.T);
  cout << "mu1=" << fmt(mom1) << " mu2=" << fmt(mom2) << " mu3=" << fmt(mom3) << " mu4=" << fmt(mom4) << endl;
  cout << "f=" << fmt(f) << " b=" << fmt(b) << endl;
}

/* Here we set the lowest frequency at which we will evaluate the spectral
 density. If the value is not predefined in the parameters file, use the
 smallest scale from the calculation multiplied by P.broaden_min_ratio. */

double get_broaden_min() { return (P.broaden_min <= 0.0 ? P.broaden_min_ratio * P.last_step_scale() : P.broaden_min); }

// Energy mesh for spectral functions
std::vector<double> make_mesh() {
  const double broaden_min = get_broaden_min();
  std::vector<double> vecE; // Energies on the mesh
  for (double E = P.broaden_max; E > broaden_min; E /= P.broaden_ratio) vecE.push_back(E);
  return vecE;
}

// Save binary raw (binned) spectral function. If using complex
// numbers and P.reim==true, we save triplets (energy,real part,imag
// part).
void SpectrumRealFreq::savebins() {
  if (!P.savebins) return;
  string fn = filename + ".bin";
  cout << " " << fn;
  ofstream Fbins = safe_open(fn, true); // true=binary!
  for (const auto &i : fspos.bins) {
    const double w = WEIGHT(i).real();
    const double e = ENERGY(i);
    my_assert(e > 0.0);
    Fbins.write((char *)&e, sizeof(double));
    Fbins.write((char *)&w, sizeof(double));
    if (P.reim) {
      const double wi = WEIGHT(i).imag();
      Fbins.write((char *)&wi, sizeof(double));
    }
  }
  for (const auto &i : fsneg.bins) {
    const double w = WEIGHT(i).real();
    const double e = -ENERGY(i);
    my_assert(e < 0.0); // attention!
    Fbins.write((char *)&e, sizeof(double));
    Fbins.write((char *)&w, sizeof(double));
    if (P.reim) {
      const double wi = WEIGHT(i).imag();
      Fbins.write((char *)&wi, sizeof(double));
    }
  }
}

void SpectrumRealFreq::trim() {
  fspos.trim();
  fsneg.trim();
}

void SpectrumRealFreq::continuous() {
  if (!P.broaden) return;
  const double alpha  = P.alpha;
  const double omega0 = (P.omega0 < 0.0 ? P.omega0_ratio * P.T : P.omega0);
  Spikes densitypos, densityneg;
  std::vector<double> vecE = make_mesh(); // Energies on the mesh
  for (double E : vecE) {
    weight_bucket valpos, valneg;
    for (const auto &i : fspos.bins) {
      const t_weight w = WEIGHT(i);
      const double e   = ENERGY(i);
      my_assert(e > 0.0);
      valpos += w * BR_NEW(E, e, alpha, omega0);
      valneg += w * BR_NEW(-E, e, alpha, omega0);
    }
    for (const auto &i : fsneg.bins) {
      const t_weight w = WEIGHT(i);
      const double e   = ENERGY(i);
      my_assert(e > 0.0); // attention!
      valneg += w * BR_NEW(-E, -e, alpha, omega0);
      valpos += w * BR_NEW(E, -e, alpha, omega0);
    }
    densitypos.push_back(make_pair(E, valpos));
    densityneg.push_back(make_pair(-E, valneg));
  }
  sort(begin(densityneg), end(densityneg), sortfirst());
  sort(begin(densitypos), end(densitypos), sortfirst());
  string fn = filename + ".dat";
  cout << " " << fn;
  ofstream Fdensity = safe_open(fn);
  save_densfunc(Fdensity, densityneg, P.reim);
  save_densfunc(Fdensity, densitypos, P.reim);
}
