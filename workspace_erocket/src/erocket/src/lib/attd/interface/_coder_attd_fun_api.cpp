//
// Prerelease License - for engineering feedback and testing purposes
// only. Not for sale.
// File: _coder_attd_fun_api.cpp
//
// MATLAB Coder version            : 26.1
// C/C++ source code generated on  : 02-Feb-2026 16:07:18
//

// Include Files
#include "_coder_attd_fun_api.h"
#include "_coder_attd_fun_mex.h"

// Variable Definitions
emlrtCTX emlrtRootTLSGlobal{nullptr};

emlrtContext emlrtContextGlobal{
    true,                                                 // bFirstTime
    false,                                                // bInitialized
    131690U,                                              // fVersionInfo
    nullptr,                                              // fErrorFunction
    "attd_fun",                                           // fFunctionName
    nullptr,                                              // fRTCallStack
    false,                                                // bDebugMode
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, // fSigWrd
    nullptr                                               // fSigMem
};

// Function Declarations
static real_T b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

static void emlrtExitTimeCleanupDtorFcn(const void *r);

static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *b_nullptr,
                               const char_T *identifier);

static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId);

static const mxArray *emlrt_marshallOut(real_T u[6]);

// Function Definitions
//
// Arguments    : const emlrtStack &sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T
//
static real_T b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims{0};
  real_T ret;
  emlrtCheckBuiltInR2012b((emlrtConstCTX)&sp, msgId, src, "double", false, 0U,
                          (const void *)&dims);
  ret = *static_cast<real_T *>(emlrtMxGetData(src));
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const void *r
// Return Type  : void
//
static void emlrtExitTimeCleanupDtorFcn(const void *r)
{
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *b_nullptr
//                const char_T *identifier
// Return Type  : real_T
//
static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *b_nullptr,
                               const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = emlrt_marshallIn(sp, emlrtAlias(b_nullptr), &thisId);
  emlrtDestroyArray(&b_nullptr);
  return y;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T
//
static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = b_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : real_T u[6]
// Return Type  : const mxArray *
//
static const mxArray *emlrt_marshallOut(real_T u[6])
{
  static const int32_T iv[2]{0, 0};
  static const int32_T iv1[2]{1, 6};
  const mxArray *m;
  const mxArray *y;
  void *existingData;
  y = nullptr;
  m = emlrtCreateNumericArray(2, (const void *)&iv[0], mxDOUBLE_CLASS, mxREAL);
  existingData = emlrtMxGetData((mxArray *)m);
  if (existingData != (void *)&u[0]) {
    emlrtFreeMex(existingData);
  }
  emlrtMxSetData((mxArray *)m, &u[0]);
  emlrtSetDimensions((mxArray *)m, &iv1[0], 2);
  emlrtAssign(&y, m);
  return y;
}

//
// Arguments    : const mxArray * const prhs[27]
//                const mxArray **plhs
// Return Type  : void
//
void attd_fun_api(const mxArray *const prhs[27], const mxArray **plhs)
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  real_T(*out1)[6];
  real_T ddyd_ddot;
  real_T ddyd_dot;
  real_T ddyd_v;
  real_T ddzd_ddot;
  real_T ddzd_dot;
  real_T ddzd_v;
  real_T ey;
  real_T ey_dddot;
  real_T ey_ddot;
  real_T ey_dot;
  real_T ey_int_v;
  real_T ez;
  real_T ez_dddot;
  real_T ez_ddot;
  real_T ez_dot;
  real_T ez_int_v;
  real_T kdy;
  real_T kdz;
  real_T kiy;
  real_T kiz;
  real_T kpy;
  real_T kpz;
  real_T m;
  real_T phi_ddot;
  real_T phi_dot;
  real_T phi_v;
  real_T u1;
  st.tls = emlrtRootTLSGlobal;
  out1 = (real_T(*)[6])mxMalloc(sizeof(real_T[6]));
  // Marshall function inputs
  phi_ddot = emlrt_marshallIn(st, emlrtAliasP(prhs[0]), "phi_ddot");
  phi_dot = emlrt_marshallIn(st, emlrtAliasP(prhs[1]), "phi_dot");
  phi_v = emlrt_marshallIn(st, emlrtAliasP(prhs[2]), "phi_v");
  ey_dddot = emlrt_marshallIn(st, emlrtAliasP(prhs[3]), "ey_dddot");
  ey_ddot = emlrt_marshallIn(st, emlrtAliasP(prhs[4]), "ey_ddot");
  ey_dot = emlrt_marshallIn(st, emlrtAliasP(prhs[5]), "ey_dot");
  ey = emlrt_marshallIn(st, emlrtAliasP(prhs[6]), "ey");
  ey_int_v = emlrt_marshallIn(st, emlrtAliasP(prhs[7]), "ey_int_v");
  ez_dddot = emlrt_marshallIn(st, emlrtAliasP(prhs[8]), "ez_dddot");
  ez_ddot = emlrt_marshallIn(st, emlrtAliasP(prhs[9]), "ez_ddot");
  ez_dot = emlrt_marshallIn(st, emlrtAliasP(prhs[10]), "ez_dot");
  ez = emlrt_marshallIn(st, emlrtAliasP(prhs[11]), "ez");
  ez_int_v = emlrt_marshallIn(st, emlrtAliasP(prhs[12]), "ez_int_v");
  ddyd_ddot = emlrt_marshallIn(st, emlrtAliasP(prhs[13]), "ddyd_ddot");
  ddyd_dot = emlrt_marshallIn(st, emlrtAliasP(prhs[14]), "ddyd_dot");
  ddyd_v = emlrt_marshallIn(st, emlrtAliasP(prhs[15]), "ddyd_v");
  ddzd_ddot = emlrt_marshallIn(st, emlrtAliasP(prhs[16]), "ddzd_ddot");
  ddzd_dot = emlrt_marshallIn(st, emlrtAliasP(prhs[17]), "ddzd_dot");
  ddzd_v = emlrt_marshallIn(st, emlrtAliasP(prhs[18]), "ddzd_v");
  m = emlrt_marshallIn(st, emlrtAliasP(prhs[19]), "m");
  u1 = emlrt_marshallIn(st, emlrtAliasP(prhs[20]), "u1");
  kpy = emlrt_marshallIn(st, emlrtAliasP(prhs[21]), "kpy");
  kpz = emlrt_marshallIn(st, emlrtAliasP(prhs[22]), "kpz");
  kdy = emlrt_marshallIn(st, emlrtAliasP(prhs[23]), "kdy");
  kdz = emlrt_marshallIn(st, emlrtAliasP(prhs[24]), "kdz");
  kiy = emlrt_marshallIn(st, emlrtAliasP(prhs[25]), "kiy");
  kiz = emlrt_marshallIn(st, emlrtAliasP(prhs[26]), "kiz");
  // Invoke the target function
  attd_fun(phi_ddot, phi_dot, phi_v, ey_dddot, ey_ddot, ey_dot, ey, ey_int_v,
           ez_dddot, ez_ddot, ez_dot, ez, ez_int_v, ddyd_ddot, ddyd_dot, ddyd_v,
           ddzd_ddot, ddzd_dot, ddzd_v, m, u1, kpy, kpz, kdy, kdz, kiy, kiz,
           *out1);
  // Marshall function outputs
  *plhs = emlrt_marshallOut(*out1);
}

//
// Arguments    : void
// Return Type  : void
//
void attd_fun_atexit()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtPushHeapReferenceStackR2021a(&st, false, nullptr,
                                    (void *)&emlrtExitTimeCleanupDtorFcn,
                                    nullptr, nullptr, nullptr);
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  attd_fun_xil_terminate();
  attd_fun_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
void attd_fun_initialize()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, nullptr);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
void attd_fun_terminate()
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

//
// File trailer for _coder_attd_fun_api.cpp
//
// [EOF]
//
