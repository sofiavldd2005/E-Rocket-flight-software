//
// Prerelease License - for engineering feedback and testing purposes
// only. Not for sale.
// File: rtGetNaN.cpp
//
// MATLAB Coder version            : 26.1
// C/C++ source code generated on  : 02-Feb-2026 16:07:18
//

// Abstract:
//       MATLAB for code generation function to initialize non-finite, NaN
// Include Files
#include "rtGetNaN.h"
#include "rt_nonfinite.h"

// Function: rtGetNaN
// ======================================================================
//  Abstract:
// Initialize rtNaN needed by the generated code.
//  NaN is initialized as non-signaling. Assumes IEEE.
real_T rtGetNaN(void)
{
  return rtNaN;
}

// Function: rtGetNaNF
// =====================================================================
//  Abstract:
//  Initialize rtNaNF needed by the generated code.
//  NaN is initialized as non-signaling. Assumes IEEE
real32_T rtGetNaNF(void)
{
  return rtNaNF;
}

//
// File trailer for rtGetNaN.cpp
//
// [EOF]
//
