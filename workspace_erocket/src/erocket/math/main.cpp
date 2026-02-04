//
// Prerelease License - for engineering feedback and testing purposes
// only. Not for sale.
// File: main.cpp
//
// MATLAB Coder version            : 26.1
// C/C++ source code generated on  : 02-Feb-2026 16:07:18
//

/*************************************************************************/
/* This automatically generated example C++ main file shows how to call  */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

// Include Files
#include "main.h"
#include "attd_fun.h"
#include "attd_fun_initialize.h"
#include "attd_fun_terminate.h"
#include "rt_nonfinite.h"
#include <iostream>   // Needed for std::cout

// Function Declarations
static double argInit_real_T();

// Function Definitions
//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : int argc
//                char **argv
// Return Type  : int
//
int main(int, char **)
{
  // Initialize the application.
  // You do not need to do this more than one time.
  attd_fun_initialize();
  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_attd_fun();
  // Terminate the application.
  // You do not need to do this more than one time.
  attd_fun_terminate();
  return 0;
}

//
// Arguments    : void
// Return Type  : void
//
void main_attd_fun()
{
    double out1[6];

    // Initialize function 'attd_fun' input arguments with dummy values
    double phi_ddot = 0.1;
    double phi_dot  = 0.2;
    double phi_v    = 0.3;

    double ey_dddot = 0.01;
    double ey_ddot  = 0.02;
    double ey_dot   = 0.03;
    double ey       = 0.04;
    double ey_int_v = 0.05;

    double ez_dddot = 0.015;
    double ez_ddot  = 0.025;
    double ez_dot   = 0.035;
    double ez       = 0.045;
    double ez_int_v = 0.055;

    double ddyd_ddot = 0.2;
    double ddyd_dot  = 0.1;
    double ddyd_v    = 0.05;

    double ddzd_ddot = 0.25;
    double ddzd_dot  = 0.15;
    double ddzd_v    = 0.06;

    double m  = 1.2;
    double u1 = 9.8;

    double kpy = 2.0; double kpz = 2.2;
    double kdy = 0.5; double kdz = 0.6;
    double kiy = 0.1; double kiz = 0.12;

    // Call the entry-point 'attd_fun'
    attd_fun(phi_ddot, phi_dot, phi_v,
             ey_dddot, ey_ddot, ey_dot, ey, ey_int_v,
             ez_dddot, ez_ddot, ez_dot, ez, ez_int_v,
             ddyd_ddot, ddyd_dot, ddyd_v,
             ddzd_ddot, ddzd_dot, ddzd_v,
             m, u1, kpy, kpz, kdy, kdz, kiy, kiz,
             out1);

    // Print the outputs
    std::cout << "out1 = [ ";
    for (int i = 0; i < 6; ++i) {
        std::cout << out1[i] << " ";
    }
    std::cout << "]" << std::endl;
}


//
// File trailer for main.cpp
//
// [EOF]
//
