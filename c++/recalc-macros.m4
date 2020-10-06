// m4 macros for nrg-recalc-*.cc files
// Rok Zitko, rok.zitko@ijs.si, 2007-2015

// m4 comment: $2 is length, $3,... are quantum numbers
define(`RECALC_TAB', {
  nrglog('f', "RECALC(fn=" << $1 << ", len=" << $2 << ", Iop=" << $3 << ")");
  if (diag.count(Ip)) {
    struct Recalc recalc_table[] = {
#include $1
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == $2);
    cnew[Twoinvar(I1, Ip)] = recalc_general(diag, qsrmax, cold, I1, Ip, recalc_table, $2, $3);
  }
})

define(`RECALC_F_TAB', {
  nrglog('f', "RECALC_F(fn=" << $1 << ", ch=" << $2 << ", len=" << $3 << ")");
  if (diag.count(I1)) {
    struct Recalc_f recalc_table[] = {
#include $1
    };
    BOOST_STATIC_ASSERT(ARRAYLENGTH(recalc_table) == $3);
    recalc_f(diag, qsrmax, opch[$2][0], I1, Ip, recalc_table, $3);
  }
})

define(`ONETWO', 
  switch (P.channels) {
  case 1: { $1 } break;
  case 2: { $2 } break;
  default: my_assert_not_reached();
  })

define(`ONE23', 
  switch (P.channels) {
  case 1: { $1 } break;
  case 2: { $2 } break;
  case 3: { $3 } break;
  default: my_assert_not_reached();
  })
  
define(`ONE234', 
  switch (P.channels) {
  case 1: { $1 } break;
  case 2: { $2 } break;
  case 3: { $3 } break;
  case 4: { $4 } break;
  default: my_assert_not_reached();
})

define(`ONE2345', 
  switch (P.channels) {
  case 1: { $1 } break;
  case 2: { $2 } break;
  case 3: { $3 } break;
  case 4: { $4 } break;
  case 5: { $5 } break;
  default: my_assert_not_reached();
})
