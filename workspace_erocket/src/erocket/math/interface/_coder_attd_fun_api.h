//
// Prerelease License - for engineering feedback and testing purposes
// only. Not for sale.
// File: _coder_attd_fun_api.h
//
// MATLAB Coder version            : 26.1
// C/C++ source code generated on  : 02-Feb-2026 16:07:18
//

#ifndef _CODER_ATTD_FUN_API_H
#define _CODER_ATTD_FUN_API_H

// Include Files
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
void attd_fun(real_T phi_ddot, real_T phi_dot, real_T phi_v, real_T ey_dddot,
              real_T ey_ddot, real_T ey_dot, real_T ey, real_T ey_int_v,
              real_T ez_dddot, real_T ez_ddot, real_T ez_dot, real_T ez,
              real_T ez_int_v, real_T ddyd_ddot, real_T ddyd_dot, real_T ddyd_v,
              real_T ddzd_ddot, real_T ddzd_dot, real_T ddzd_v, real_T m,
              real_T u1, real_T kpy, real_T kpz, real_T kdy, real_T kdz,
              real_T kiy, real_T kiz, real_T out1[6]);

void attd_fun_api(const mxArray *const prhs[27], const mxArray **plhs);

void attd_fun_atexit();

void attd_fun_initialize();

void attd_fun_terminate();

void attd_fun_xil_shutdown();

void attd_fun_xil_terminate();

#endif
//
// File trailer for _coder_attd_fun_api.h
//
// [EOF]
//
