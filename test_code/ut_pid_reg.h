#ifndef UT_PID_REG_H
#define UT_PID_REG_H
// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Header for Unit Test for the matrix/vector functions which
//               are used for the PID controller.
//               The Unit Test is organised into 3 main sections:
//               1) The Unit test-suite of the vector and matrix routines
//               2) The Unit test-suite of the system identification routines
//               3) The Integration test of the entire Self-Tuning controller
//
//               CuTest is used as Unit Test library.
//               File should be compiled with Borland C++ Builder 4.5!
// --------------------------------------------------------------------------
// $Log$
// Revision 1.1  2007/10/03 07:53:31  Emile
// - Header file added to CVS.
//
// ==========================================================================
#include "../pid_reg.h"
#include "CuTest.h"

//---------------------------------------------------------------
// The define CHECK_MATRIX checks for every double in the matrix
// whether or not it is the expected value. This macro is used
// in the functions ut_vxv2m(), ut_mxs2m() and ut_max2m().
//---------------------------------------------------------------
#define CHECK_MATRIX(mexp,mact,N)                               \
             for (i = 0; i < N; i++)                            \
               for (j = 0; j < N; j++)                          \
                 CuAssertDblEquals(tc,mexp[i][j],mact[i][j],e)

//---------------------------------------------------------------
// The define CHECK_MATRIX checks for every double in the vector
// whether or not it is the expected value. This macro is used
// in the functions ut_vxm2m(), ut_mxv2v(), ut_vxs2v(), 
// ut_vav2v(), it_pid_reg_N2() and it_pid_reg_N3().
//---------------------------------------------------------------
#define CHECK_VECTOR(vexp,vact,N)                      \
             for (i = 0; i < N; i++)                   \
               CuAssertDblEquals(tc,vexp[i],vact[i],e)

//---------------------------------------------------
// List of all Unit- and Integration Test functions
//---------------------------------------------------
void    ut_vxv2s(CuTest *tc);       // Unit Test functions
void    ut_vxv2m(CuTest *tc);
void    ut_vxm2v(CuTest *tc);
void    ut_mxv2v(CuTest *tc);
void    ut_vxs2v(CuTest *tc);
void    ut_vav2v(CuTest *tc);
void    ut_mxs2m(CuTest *tc);
void    ut_mam2m(CuTest *tc);
void    ut_sys_id1_nodf(CuTest *tc);
void    ut_sys_id1_adf(CuTest *tc);
void    ut_sys_id2_nodf(CuTest *tc);
void    ut_ultim_gain_period(CuTest *tc);
void    ut_calc_pid_parameters(CuTest *tc);
void    ut_ebrew_slope_limiter(CuTest *tc);
void    ut_ebrew_sample_delay(CuTest *tc);

void    it_pid_reg_N2(CuTest *tc);  // Integration Test functions
void    it_pid_reg_N3(CuTest *tc);
void    it_pid_regs_Gb(void);

CuSuite *vector_matrix_suite(void); // Test suite functions
CuSuite *sys_id_suite(void);
CuSuite *integration_test_suite(void);
CuSuite *ebrew_test_suite(void);

void    run_all_tests(void);        // Entry-point for all tests

#endif /* UT_PID_REG_H */
