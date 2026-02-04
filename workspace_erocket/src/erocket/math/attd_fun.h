//
// Prerelease License - for engineering feedback and testing purposes
// only. Not for sale.
// File: attd_fun.h
//
// MATLAB Coder version            : 26.1
// C/C++ source code generated on  : 02-Feb-2026 16:07:18
//

#ifndef ATTD_FUN_H
#define ATTD_FUN_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void attd_fun(double phi_ddot, double phi_dot, double phi_v,
                     double ey_dddot, double ey_ddot, double ey_dot, double ey,
                     double ey_int_v, double ez_dddot, double ez_ddot,
                     double ez_dot, double ez, double ez_int_v,
                     double ddyd_ddot, double ddyd_dot, double ddyd_v,
                     double ddzd_ddot, double ddzd_dot, double ddzd_v, double m,
                     double u1, double kpy, double kpz, double kdy, double kdz,
                     double kiy, double kiz, double out1[6]);

#endif
//
// File trailer for attd_fun.h
//
// [EOF]
//
