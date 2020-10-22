// tridiag.h - Tridiagonalisation code
// Copyright (C) 2009-2020 Rok Zitko

#ifndef _tridiag_h_
#define _tridiag_h_

#include "mp.h"

template<typename S>
class Tridiag {
   const Params &P;
 public:
   void tridiag_ch(int alpha, Coef<S> &coef);
   Tridiag(Coef<S> &coef, const Params &P) : P(P) {
     my_assert(P.coefchannels >= 1);
     for (unsigned int alpha = 0; alpha < P.coefchannels; alpha++) tridiag_ch(alpha, coef);
   }
};
   
// Fix normalization of u_{n,m}, v_{n,m} to 1. IMPORTANT: pass by reference!
inline void fix_norm(vmpf &up, vmpf &um, const unsigned int mMAX) {
  // Constants
  my_mpf mpZERO, mpONE;
  mpf_set_str(mpONE, "1.e0", 10);

  my_mpf sum, temp, tempsq;

  mpf_set(sum, mpZERO);
  for (unsigned int m = 0; m <= mMAX; m++) {
    mpf_mul(tempsq, up[m], up[m]);
    mpf_add(sum, sum, tempsq);
    mpf_mul(tempsq, um[m], um[m]);
    mpf_add(sum, sum, tempsq);
  }
  mpf_sqrt(temp, sum);

  for (unsigned int m = 0; m <= mMAX; m++) {
    mpf_div(up[m], up[m], temp);
    mpf_div(um[m], um[m], temp);
  }
}

// Tridiagonalisation of the discretization coefficients. Multiple precision arithmetics library GMP is required.
template<typename S>
void Tridiag<S>::tridiag_ch(int alpha, Coef<S> &coef) {
  std::cout << "Tridiagonalisation, ch=" << alpha << ".";
  std::cout << " Using GMP version " << gmp_version << std::endl;

  const unsigned int mMAX = coef.em.max(alpha);
  std::cout << "mMAX=" << mMAX << std::endl;

  my_assert(coef.ep.max(alpha) == mMAX);
  my_assert(coef.u0p.max(alpha) == mMAX);
  my_assert(coef.u0m.max(alpha) == mMAX);

  mpf_set_default_prec(P.preccpp);
  std::cout << "Using precision of " << P.preccpp << " digits." << std::endl;

  // Constants
  my_mpf mpZERO;

  // Temporary MP variables
  my_mpf temp, tempsq, sum;

  my_mpf mpxi;   // xi
  my_mpf xi2;    // xi^2
  my_mpf mpzeta; // zeta

  my_mpf xi_prev, xi2_prev; // values in previous iteration

  vmpf up(mMAX + 1);
  vmpf up_prev(mMAX + 1);
  vmpf up_prev2(mMAX + 1);

  vmpf um(mMAX + 1);
  vmpf um_prev(mMAX + 1);
  vmpf um_prev2(mMAX + 1);

  vmpf ep1(mMAX + 1);
  vmpf em1(mMAX + 1);

  vmpf ep2(mMAX + 1);
  vmpf em2(mMAX + 1);

  for (unsigned int m = 0; m <= mMAX; m++) {
    // Only real parameters are supported in this code [8.10.2009]
    mpf_set_d(up_prev[m], check_real(coef.u0p(m, alpha)));
    mpf_set_d(um_prev[m], check_real(coef.u0m(m, alpha)));
    mpf_set_d(ep1[m], check_real(coef.ep(m, alpha)));
    mpf_set_d(em1[m], check_real(coef.em(m, alpha)));
    mpf_mul(ep2[m], ep1[m], ep1[m]);
    mpf_mul(em2[m], em1[m], em1[m]);
  }

  fix_norm(up_prev, um_prev, mMAX);

  for (unsigned int n = 0; n <= P.Nmax; n++) {
    // Calculate zeta_n, xi2_n and xi_n
    mpf_set(mpzeta, mpZERO);
    mpf_set(xi2, mpZERO);
    for (unsigned int m = 0; m <= mMAX; m++) {
      // up_prev = u^+_{n,m}
      mpf_mul(tempsq, up_prev[m], up_prev[m]);
      mpf_mul(temp, tempsq, ep2[m]);
      mpf_add(xi2, xi2, temp);
      mpf_mul(temp, tempsq, ep1[m]);
      mpf_add(mpzeta, mpzeta, temp);

      // um_prev = u^-_{n,m}
      mpf_mul(tempsq, um_prev[m], um_prev[m]);
      mpf_mul(temp, tempsq, em2[m]);
      mpf_add(xi2, xi2, temp);
      mpf_mul(temp, tempsq, em1[m]);
      mpf_sub(mpzeta, mpzeta, temp);
    }

    // subtract xi^2_{n-1}
    mpf_sub(xi2, xi2, xi2_prev);

    // subtract zeta^2_n
    mpf_mul(tempsq, mpzeta, mpzeta);
    mpf_sub(xi2, xi2, tempsq);

    mpf_sqrt(mpxi, xi2);

    // compute u_{n+1,m}, v_{n+1,m}
    for (unsigned int m = 0; m <= mMAX; m++) {
      // zeta=zeta_n
      mpf_sub(temp, ep1[m], mpzeta);
      // up_prev[m]=u_{n,m}
      mpf_mul(up[m], temp, up_prev[m]);

      // xi_prev=xi_{n-1}, up_prev2=u_{n-1,m}
      mpf_mul(temp, xi_prev, up_prev2[m]);
      mpf_sub(up[m], up[m], temp);

      // xi=xi_n
      mpf_div(up[m], up[m], mpxi);

      mpf_neg(temp, em1[m]);
      mpf_sub(temp, temp, mpzeta);
      mpf_mul(um[m], temp, um_prev[m]);
      mpf_mul(temp, xi_prev, um_prev2[m]);
      mpf_sub(um[m], um[m], temp);
      mpf_div(um[m], um[m], mpxi);
    }

    fix_norm(up, um, mMAX);

    // Recalculate xi, xi2
    mpf_set(sum, mpZERO);
    for (unsigned int m = 0; m <= mMAX; m++) {
      mpf_mul(temp, up[m], up_prev[m]);
      mpf_mul(temp, temp, ep1[m]);
      mpf_add(sum, sum, temp);
      mpf_mul(temp, um[m], um_prev[m]);
      mpf_mul(temp, temp, em1[m]);
      mpf_sub(sum, sum, temp);
    }
    mpf_set(mpxi, sum);
    mpf_mul(xi2, mpxi, mpxi);

    // Save results
    double dxi       = mpf_get_d(mpxi);
    double dzeta     = mpf_get_d(mpzeta);
    double coef_xi   = dxi * P.bandrescale;
    double coef_zeta = dzeta * P.bandrescale;

    coef.xi.setvalue(n, alpha, coef_xi);
    coef.zeta.setvalue(n, alpha, coef_zeta);

    std::cout << "  xi(" << n << ")=" << HIGHPREC(dxi) << std::endl;
    std::cout << "zeta(" << n << ")=" << HIGHPREC(dzeta) << std::endl;

    // Store results from previous iteration
    mpf_set(xi_prev, mpxi);
    mpf_set(xi2_prev, xi2);

    for (unsigned int m = 0; m <= mMAX; m++) {
      mpf_set(um_prev2[m], um_prev[m]);
      mpf_set(up_prev2[m], up_prev[m]);
      mpf_set(um_prev[m], um[m]);
      mpf_set(up_prev[m], up[m]);
    }
  }
}

#endif // _tridiag_h_
