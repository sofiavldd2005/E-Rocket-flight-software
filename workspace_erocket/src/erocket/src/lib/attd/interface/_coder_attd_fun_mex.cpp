//
// Prerelease License - for engineering feedback and testing purposes
// only. Not for sale.
// File: _coder_attd_fun_mex.cpp
//
// MATLAB Coder version            : 26.1
// C/C++ source code generated on  : 02-Feb-2026 16:07:18
//

// Include Files
#include "_coder_attd_fun_mex.h"
#include "_coder_attd_fun_api.h"

// Function Definitions
//
// Arguments    : int32_T nlhs
//                mxArray *plhs[]
//                int32_T nrhs
//                const mxArray *prhs[]
// Return Type  : void
//
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&attd_fun_atexit);
  attd_fun_initialize();
  unsafe_attd_fun_mexFunction(nlhs, plhs, nrhs, prhs);
  attd_fun_terminate();
}

//
// Arguments    : void
// Return Type  : emlrtCTX
//
emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, nullptr, 1,
                           nullptr, "windows-1252", true);
  return emlrtRootTLSGlobal;
}

//
// Arguments    : int32_T nlhs
//                mxArray *plhs[1]
//                int32_T nrhs
//                const mxArray *prhs[27]
// Return Type  : void
//
void unsafe_attd_fun_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                 const mxArray *prhs[27])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 27) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 27, 4,
                        8, "attd_fun");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 8,
                        "attd_fun");
  }
  // Call the function.
  attd_fun_api(prhs, &outputs);
  // Copy over outputs to the caller.
  emlrtReturnArrays(1, &plhs[0], &outputs);
}

//
// File trailer for _coder_attd_fun_mex.cpp
//
// [EOF]
//
