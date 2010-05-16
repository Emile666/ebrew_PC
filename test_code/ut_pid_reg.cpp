// ==========================================================================
// Filename    : $Id$
// Author      : E. van de Logt
// Purpose     : Unit Test file for the matrix/vector functions which are
//               used for the PID controller.
//               The Unit Test is organised into 3 main sections:
//               1) The Unit test-suite of the vector and matrix routines
//               2) The Unit test-suite of the system identification routines
//               3) The Integration test of the entire Self-Tuning controller
//
//               CuTest is used as Unit Test library.
//               File should be compiled with Borland C++ Builder 4.5!
// --------------------------------------------------------------------------
// $Log$
// Revision 1.2  2007/10/03 07:49:04  Emile
// - Header file added, file- and function-header comments added
// - No functional changes.
//
// Revision 1.1  2007/10/03 07:01:46  Emile
// Working Unit Test files for vector / matrix library added to CVS.
//
// ==========================================================================
#pragma hdrstop
#include <condefs.h>
#include <stdio.h>
#include <conio.h>
#include "ut_pid_reg.h"

//---------------------------------------------------------------------------
USEUNIT("..\PID_REG.C");
USEUNIT("CuTest.c");
//---------------------------------------------------------------------------
#pragma argsused

extern vector theta;  // from pid_reg.c: parameter estimate vector
extern vector d;      // from_pid_reg.c: regression vector
extern double phi;    // directional forgetting factor
extern double lambda; // parameter lambda
extern double eta;    // parameter eta (v)

double e     = 1e-6;  // max. deviation for all floating-point calculations
vector v1    = {  2.0,  3.0,  4.0 }; // default test-vector used in unit-tests
vector v2    = { 11.0, 12.0, 13.0 }; // default test-vector used in unit-tests

//---------------------------------------------------------------------------
// Start of all Unit-Test cases
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function vxv2s (vector multiply
             vector, returning a scalar).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_vxv2s(CuTest *tc)
{
   double x;

   printf("   Testcase 01: vxv2s, Vector x Vector To Scalar\n");
   x = vxv2s(v1,v2,1); CuAssertDblEquals(tc, 22.0,x,e);
   x = vxv2s(v1,v2,2); CuAssertDblEquals(tc, 58.0,x,e);
   x = vxv2s(v1,v2,3); CuAssertDblEquals(tc,110.0,x,e);
} // ut_vxv2s()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function vxv2m (vector multiply
             vector, returning a matrix).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_vxv2m(CuTest *tc)
{
  int i,j;
  matrix m1exp = { { 22.0, 24.0, 26.0},
                   { 33.0, 36.0, 39.0},
                   { 44.0, 48.0, 52.0}};
  matrix m1;

  printf("   Testcase 02: vxv2m, Vector x Vector To Matrix\n");
  vxv2m(v1,v2,&m1,1); CHECK_MATRIX(m1exp,m1,1);
  vxv2m(v1,v2,&m1,2); CHECK_MATRIX(m1exp,m1,2);
  vxv2m(v1,v2,&m1,3); CHECK_MATRIX(m1exp,m1,3);
} // ut_vxv2s()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function vxv2s (vector multiply
             vector, returning a scalar).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_vxm2v(CuTest *tc)
{
   int i;
   matrix m1    = { { 22.0, 24.0, 26.0},
                    { 33.0, 36.0, 39.0},
                    { 44.0, 48.0, 52.0}};
   vector v1exp = {  44.0 };
   vector v2exp = { 143.0, 156.0 };
   vector v3exp = { 319.0, 348.0, 377.0 };
   vector v2;

   printf("   Testcase 03: vxm2v, Vector Transposed x Matrix To Vector\n");
   vxm2v(v1,m1,&v2,1); CHECK_VECTOR(v1exp,v2,1);
   vxm2v(v1,m1,&v2,2); CHECK_VECTOR(v2exp,v2,2);
   vxm2v(v1,m1,&v2,3); CHECK_VECTOR(v3exp,v2,3);
} // ut_vxm2v()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function mxv2v (matrix multiply
             vector, returning a vector).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_mxv2v(CuTest *tc)
{
   int i;
   matrix m1    = { { 22.0, 24.0, 26.0},
                    { 33.0, 36.0, 39.0},
                    { 44.0, 48.0, 52.0}};
   vector v1exp = {  44.0 };
   vector v2exp = { 116.0, 174.0 };
   vector v3exp = { 220.0, 330.0, 440.0 };
   vector v2;

   printf("   Testcase 04: mxv2v, Matrix x Vector To Vector\n");
   mxv2v(m1,v1,&v2,1); CHECK_VECTOR(v1exp,v2,1);
   mxv2v(m1,v1,&v2,2); CHECK_VECTOR(v2exp,v2,2);
   mxv2v(m1,v1,&v2,3); CHECK_VECTOR(v3exp,v2,3);
} // ut_mxv2v()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function vxs2v (vector multiply
             by scalar, returning a vector).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_vxs2v(CuTest *tc)
{
   int i;
   vector v1exp = { 3.2 };
   vector v2exp = { 6.4, 6.0 };
   vector v3exp = { 3.2, 3.0, 2.0 };
   vector v1    = { 2.0, 3.0, 4.0};

   printf("   Testcase 05: vxs2v, Vector x Scalar To Vector\n");
   vxs2v(&v1,1.60,1); CHECK_VECTOR(v1exp,v1,1);
   vxs2v(&v1,2.00,2); CHECK_VECTOR(v2exp,v1,2);
   vxs2v(&v1,0.50,3); CHECK_VECTOR(v3exp,v1,3);
} // ut_vxs2v()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function vav2v (vector add
             vector, returning a vector).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_vav2v(CuTest *tc)
{
   int i;
   vector v1exp = { 14.2 };
   vector v2exp = { 25.2, 15.0 };
   vector v3exp = { 36.2, 18.0, 15.0 };
   vector v1    = { 3.2, 3.0, 2.0};

   printf("   Testcase 06: vav2v, Vector + Vector To Vector\n");
   vav2v(&v1,v2,1); CHECK_VECTOR(v1exp,v1,1);
   vav2v(&v1,v2,1); CHECK_VECTOR(v2exp,v1,1);
   vav2v(&v1,v2,1); CHECK_VECTOR(v3exp,v1,1);
} // ut_vav2v()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function mxs2m (matrix multiply
             by scalar, returning a matrix).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_mxs2m(CuTest *tc)
{
   int i,j;
   matrix m1    = { { 1.000, -2.500,  3.000},
                    {-3.500,  4.600, -5.700},
                    { 6.800, -7.900,  8.100}};
   matrix m1exp = { { 0.500, -2.500,  3.000},
                    {-3.500,  4.600, -5.700},
                    { 6.800, -7.900,  8.100}};
   matrix m2exp = { {-1.050,  5.250,  3.000},
                    { 7.350, -9.660, -5.700},
                    { 6.800, -7.900,  8.100}};
   matrix m3exp = { { 0.315, -1.575, -0.900},
                    {-2.205,  2.898,  1.710},
                    {-2.040,  2.370, -2.430}};

   printf("   Testcase 07: mxs2m, Matrix x Scalar To Matrix\n");
   mxs2m(&m1, 0.5,1); CHECK_MATRIX(m1exp,m1,1);
   mxs2m(&m1,-2.1,2); CHECK_MATRIX(m2exp,m1,2);
   mxs2m(&m1,-0.3,3); CHECK_MATRIX(m3exp,m1,3);
} // ut_mxs2m()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function mam2m (matrix add
             matrix, returning a matrix).
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_mam2m(CuTest *tc)
{
   int i,j;
   matrix m1    = { { 1.000, -2.500,  3.000},
                    {-3.500,  4.600, -5.700},
                    { 6.800, -7.900,  8.100}};
   matrix m2    = { {-2.100, -1.575,  8.564},
                    { 7.350, -9.660, -5.700},
                    {-2.040,  2.370, -2.430}};
   matrix m1exp = { {-1.100, -2.500,  3.000},
                    {-3.500,  4.600, -5.700},
                    { 6.800, -7.900,  8.100}};
   matrix m2exp = { {-3.200, -4.075,  3.000},
                    { 3.850, -5.060, -5.700},
                    { 6.800, -7.900,  8.100}};
   matrix m3exp = { {-5.300, -5.650, 11.564},
                    {11.200,-14.720,-11.400},
                    { 4.760, -5.530,  5.670}};

  printf("   Testcase 08: mam2m, Matrix + Matrix To Matrix\n");
  mam2m(&m1,m2,1); CHECK_MATRIX(m1exp,m1,1);
  mam2m(&m1,m2,2); CHECK_MATRIX(m2exp,m1,2);
  mam2m(&m1,m2,3); CHECK_MATRIX(m3exp,m1,3);
} // ut_mam2m()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function sys_id, which is the
             system identification function. Test is for order N=1
             with no directional forgetting.
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_sys_id1_nodf(CuTest *tc)
{
  int        i;
  pid_params p;
  p.ts = 1.0; p.kc = 1.0; p.ti = 1.0;  p.td = 1.0;

  double u[] = {-0.6918,  0.8580, 1.2540, -1.5937, -1.4410,  0.5711, -0.3999,  0.6900};
  double y[] = { 0.0071, -0.1252, 0.0583,  0.3070, -0.0744, -0.3420, -0.1839, -0.2539};
  // Values from Matlab indentdf_wrapper_N1.m
  double a1exp[] = {0.1008, -0.6848,-0.8660,-0.8116,-0.7993,-0.8370,-0.8552};
  double b1exp[] = {0.1798,  0.1757, 0.1993, 0.2007, 0.1985, 0.1997, 0.2020};

  printf("   Testcase 09: sys_id, System Identification (order N = 1, NODF)\n");
  // Book page 31, b1=0.1997, a1=-0.8669
  init_pid5(&p,1); // call init. with empty p struct and N == 1
  d[0] = -0.0071; // -y[1], make identical to Matlab program
  d[1] = -0.6918; //  u[1], see : Digital Self-Tuning controllers, page 39

  for (i = 1; i < 8; i++)
  {
     sys_id(u[i-1],y[i],1,NODF);
     CuAssertDblEquals(tc,a1exp[i-1],theta[0],1e-4); // a1[i]
     CuAssertDblEquals(tc,b1exp[i-1],theta[1],1e-4); // b1[i]
     CuAssertDblEquals(tc,1.0       ,phi     ,1e-4); // phi
  } // for i
} // ut_sys_id1_nodf()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function sys_id, which is the
             system identification function. Test is for order N=1
             with adapted directional forgetting.
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_sys_id1_adf(CuTest *tc)
{
  int        i;
  pid_params p;
  p.ts = 1.0; p.kc = 1.0; p.ti = 1.0;  p.td = 1.0;

  double u[] = {-0.6918,  0.8580, 1.2540, -1.5937, -1.4410,  0.5711, -0.3999,  0.6900};
  double y[] = { 0.0071, -0.1252, 0.0583,  0.3070, -0.0744, -0.3420, -0.1839, -0.2539};
  // Values from Matlab indentdf_wrapper_N1.m
  double a1exp[]  = {0.1008, -0.6848,-1.0051,-0.7766,-0.7834,-0.8705,-0.9070};
  double b1exp[]  = {0.1798,  0.1757, 0.1938, 0.1956, 0.1966, 0.2038, 0.2098};
  double phiexp[] = {0.0813,  0.1537, 0.1895, 0.1086, 0.2354, 0.1733, 0.2736};

  printf("   Testcase 10: sys_id, System Identification (order N = 1, ADF)\n");
  // Book page 31, b1=0.1997, a1=-0.8669
  init_pid5(&p,1); // call init. with empty p struct and N == 1
  d[0] = -0.0071; // -y[1], make identical to Matlab program
  d[1] = -0.6918; //  u[1], see : Digital Self-Tuning controllers, page 39

  for (i = 1; i < 8; i++)
  {
     sys_id(u[i-1],y[i],1,!NODF);
     CuAssertDblEquals(tc,a1exp[i-1] ,theta[0],1e-4); // a1[i]
     CuAssertDblEquals(tc,b1exp[i-1] ,theta[1],1e-4); // b1[i]
     CuAssertDblEquals(tc,phiexp[i-1],phi     ,1e-4); // ext. var. phi
  } // for i
} // ut_sys_id1_adf()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function sys_id, which is the
             system identification function. Test is for order N=2
             with no directional forgetting.
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_sys_id2_nodf(CuTest *tc)
{
  int        i;
  pid_params p;
  p.ts = 1.0; p.kc = 1.0; p.ti = 1.0;  p.td = 1.0;

  // system identification with 2nd order process
  // a1=-1.4253, a2=0.4966, b1=-0.1027, b2=0.1730 (see H5_vb.xls)
  double u[] = { -0.0740,  1.3174,  1.6804,  1.1934, -0.4183,  0.8779,  1.6688, -1.1308,
                 -0.8360, -0.4164,  0.2271, -1.4945,  0.6550, -1.3920, -0.9298,  0.6184,
                  0.7664, -1.0097,  1.3486, -0.7796,  1.0197, -1.4165,  1.6342 };
  double y[] = { -0.0949,  0.0322, -0.0551, -0.0391,  0.1397,  0.4680,  0.4351,  0.3682,
                  0.7136,  0.7245,  0.5573,  0.3392,  0.3994,  0.0751,  0.1649,  0.0524,
                 -0.2315, -0.3277, -0.1159, -0.3156, -0.0789, -0.1953,  0.0827 };

  d[0] = -y[1]; // -y[k-1], make identical to Matlab program
  d[1] = -y[0]; // -y[k-2]
  d[2] =  u[1]; //  u[k-1]
  d[3] =  u[0]; //  u[k-2]
  // Values from Matlab indentdf_wrapper_N2.m

  printf("   Testcase 11: sys_id, System Identification (order N = 2, NODF)\n");
  init_pid5(&p,2); // call init. with empty p struct and N == 2
  d[0] = -y[1]; // -y[k-1], make identical to identdf_wrapper_N2.m
  d[1] = -y[0]; // -y[k-2]
  d[2] =  u[0]; //  u[k-1]
  for (i = 2; i < 23; i++)
  {
     sys_id(u[i-1],y[i],2,NODF);
  } // for i
  CuAssertDblEquals(tc,-1.4232,theta[0],1e-4); // a1[i] value from Matlab
  CuAssertDblEquals(tc, 0.5000,theta[1],1e-4); // a2[i] value from Matlab
  CuAssertDblEquals(tc,-0.1026,theta[2],1e-4); // b1[i] value from Matlab
  CuAssertDblEquals(tc, 0.1734,theta[3],1e-4); // b2[i] value from Matlab
} // ut_sys_id2_nodf()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function calc_ultimate_gain,
             which is the routine to calculate the ultimate gain
             of a feedback control system.
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_ultim_gain_period(CuTest *tc)
{
   sys_id_params ps;
   int           err;

   printf("   Testcase 12: calc_ultimate_gain_period()\n");
   ps.N   = 1;
   theta[0] = 1.3254; // a1
   theta[1] = 0.0;    // b1
   err = calc_ultimate_gain_geriod(&ps,2.0);
   CuAssertIntEquals(tc,1     ,err);
   CuAssertDblEquals(tc,1.0000,ps.kpu,1e-4);
   CuAssertDblEquals(tc,4.0000,ps.tu,1e-4);

   theta[1] = 2.6291;    // b1
   err = calc_ultimate_gain_geriod(&ps,2.0);
   CuAssertIntEquals(tc,0      ,err);
   CuAssertDblEquals(tc,-0.1238,ps.kpu,1e-4);
   CuAssertDblEquals(tc, 4.0000,ps.tu,1e-4);

   ps.N = 2;
   theta[0] = -1.4891; // a1: this is Ga(z) from page 100 of "Digital ST controllers"
   theta[1] =  0.5488; // a2
   theta[2] =  0.0329; // b1
   theta[3] =  0.0269; // b2
   err = calc_ultimate_gain_geriod(&ps,2.0);
   CuAssertIntEquals(tc,0     ,err);
   CuAssertDblEquals(tc,16.7732,ps.kpu,1e-4);
   CuAssertDblEquals(tc,11.6027,ps.tu,1e-4);

   theta[0] = -1.4253; // a1: this is Gb(z) from page 100 of "Digital ST controllers"
   theta[1] =  0.4966; // a2
   theta[2] = -0.1017; // b1
   theta[3] =  0.1730; // b2
   err = calc_ultimate_gain_geriod(&ps,2.0);
   CuAssertIntEquals(tc,0      ,err);
   CuAssertDblEquals(tc, 2.9098,ps.kpu,1e-4);
   CuAssertDblEquals(tc,23.5184,ps.tu,1e-4);

   theta[0] = -3.3248; // a1: this is Gc(z) from page 100 of "Digital ST controllers"
   theta[1] =  1.6487; // a2: N=2 unstable poles
   theta[2] = -0.6624; // b1
   theta[3] =  0.0137; // b2
   err = calc_ultimate_gain_geriod(&ps,2.0);
   CuAssertIntEquals(tc,1     ,err);
   CuAssertDblEquals(tc,1.0000,ps.kpu,1e-4);
   CuAssertDblEquals(tc,4.0000,ps.tu,1e-4);

   theta[0] = -1.4253; // a1: this is Gb(z) from page 100 of "Digital ST controllers"
   theta[1] =  0.4966; // a2
   theta[2] =  0.1017; // b1 modified to get 2 real poles in overall transfer function
   theta[3] = -0.0536; // b2
   err = calc_ultimate_gain_geriod(&ps,2.0);
   CuAssertIntEquals(tc,0      ,err);
   CuAssertDblEquals(tc,18.8146,ps.kpu,1e-4);
   CuAssertDblEquals(tc, 4.0000,ps.tu,1e-4);

   ps.N = 3;
   theta[0] = -1.2747; // a1: this is Gd(z) from page 126 of "Digital ST controllers"
   theta[1] =  0.5361; // a2: 2 complex poles + 1 real pole
   theta[2] =  0.0302; // a3
   theta[3] =  0.0706; // b1
   theta[4] =  0.1416; // b2
   theta[5] =  0.0136; // b3
   err = calc_ultimate_gain_geriod(&ps,1.0);
   CuAssertIntEquals(tc,0     ,err);
   CuAssertDblEquals(tc,2.7297,ps.kpu,1e-4);
   CuAssertDblEquals(tc,6.5544,ps.tu,1e-4);

   theta[0] = -1.2747; // a1: this is Gd(z) from page 126 of "Digital ST controllers"
   theta[1] =  0.5361; // a2: pole at z=-1 + two real poles
   theta[2] =  0.0302; // a3
   theta[3] =  0.6749; // b1  modified to make poles real in overall transfer function!
   theta[4] = -0.3037; // b2
   theta[5] = -0.0517; // b3
   err = calc_ultimate_gain_geriod(&ps,1.0);
   CuAssertIntEquals(tc,0     ,err);
   CuAssertDblEquals(tc,3.0000,ps.kpu,5e-3);
   CuAssertDblEquals(tc,2.0000,ps.tu,5e-3);
} // ut_ultim_gain_period()
//---------------------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function tests the function calc_pid_parameters(),
             which calculates the PID parameters, based on the
             ultimate gain.
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void ut_calc_pid_parameters(CuTest *tc)
{
   sys_id_params ps;

   printf("   Testcase 13: calc_pid_parameters()\n");
   ps.kpu =  2.9098;
   ps.tu  = 23.5184;
   calc_pid_parameters(&ps,2.0);

   CuAssertDblEquals(tc, 1.5974,ps.kr,1e-4);
   CuAssertDblEquals(tc, 0.2969,ps.ki,1e-4);
   CuAssertDblEquals(tc, 2.5663,ps.kd,1e-4);
   CuAssertDblEquals(tc, 1.5974,ps.kc,1e-4);
   CuAssertDblEquals(tc,10.7592,ps.ti,1e-4);
   CuAssertDblEquals(tc, 3.2130,ps.td,1e-4);
} // ut_calc_pid_parameters()
//------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function contains the integration test for the
             self-tuning PID controller with order N = 2.
             It checks if the same response is obtained as from the
             Matlab / Simulink model.
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void it_pid_reg_N2(CuTest *tc)
{
   FILE          *f1;
   int           N;
   int           i;
   int           vrg;            // 1 = release controller
   pid_params    p;
   sys_id_params ps;
   double        wk;
   double        yk, yk_1, yk_2; // process output
   double        uk, uk_1, uk_2; // process input = PID output
   vector        u,y;
   vector        a1,a2,b1,b2;    // parameter estimates
   // The following data comes from the sys_id3 Simulink model
   vector        uexp  = { 1.3930, 3.0805, 0.0000, 0.0000,  0.4613,  1.0001 };
   vector        yexp  = { 0.0000,-0.1417,-0.2742, 0.2124,  0.4390,  1.0000 };
   vector        a1exp = { 0.1000, 0.1000, 0.0817,-0.0989, -1.4176, -1.4253 };
   vector        a2exp = { 0.2000, 0.2000, 0.2000, 0.8614,  0.5025,  0.4966 };
   vector        b1exp = { 0.1000,-0.1017,-0.1017,-0.1017, -0.1017, -0.1017 };
   vector        b2exp = { 0.2000, 0.2000, 0.0197, 0.0381,  0.1720,  0.1730 };

   printf("   Testcase 14: Integration Test pid_reg5(), N = 2\n");
   f1 = fopen("it_pid_log_N2.txt","w");
   N       = 2;   // simulate 2nd order model
   p.ts    = 2.0; // sample-period is 2 sec.
   p.kc    = 1.0; // initialize PID controller parameters (just in case)
   p.ti    = 2.0; //
   p.td    = 1.0; //
   p.k_lpf = 0.3; // Filtering of D-term

   init_pid5(&p,N);
   ps.N = N;
   vrg  = 0;                 // disable PID controller at start-up
   uk   = uk_1 = uk_2 = 0.0; // controller output, process input
   yk   = yk_1 = yk_2 = 0.0; // process output
   wk   = 0.0;               // set-point, reference value
   fprintf(f1,"i wk uk yk a1 a2 b1 b2 Kr Ki Kd Ti Td Kpu Tu pp pi pd\n");

   for (i = 0; i < 150; i++)
   {
      if (i ==  5)       vrg = 1;   // release controller at i = 5
      if (i == 10)       wk  = 1.0; // create step at i = 10
      else if (i ==  50) wk  = 0.5; // create step at i = 50
      else if (i == 100) wk  = 1.0; // create step at i = 100

      //------------------------------------------------------------------------
      //Now calculate the process model, which is Gb(z), with transfer function:
      //
      //                   -1      -2
      //               b1*z  + b2*z
      //       H(z) = -------------------
      //                      -1      -2
      //              1 + a1*z  + a2*z
      //
      // with b1 = -0.1017, b2 = 0.1730, a1 = -1.4253 and a2 = 0.4966
      //------------------------------------------------------------------------
      yk = 1.4253 * yk_1 - 0.4966 * yk_2 - 0.1017 * uk_1 + 0.1730 * uk_2;
      yk_2 = yk_1;  yk_1 = yk;

      pid_reg5(yk,&uk,wk, &p,vrg,&ps,1); // pid controller with self-tuning
      uk_2 = uk_1;  uk_1 = uk;

      if ((i >= 10) && (i <= 14))
      {
         y[i-10]  = yk;       u[i-10]  = uk;
         a1[i-10] = theta[0]; a2[i-10] = theta[1];
         b1[i-10] = theta[2]; b2[i-10] = theta[3];
      }
      else if (i == 149)
      {
         y[5]  = yk;       u[5]  = uk;
         a1[5] = theta[0]; a2[5] = theta[1];
         b1[5] = theta[2]; b2[5] = theta[3];
      } // if
      fprintf(f1,"%02d %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f\n",
              i,wk,uk,yk,theta[0],theta[1],theta[2],theta[3],ps.kr,ps.ki,ps.kd,ps.ti,ps.td,ps.kpu,ps.tu,p.pp,p.pi,p.pd);
   } // for i
   fclose(f1);
   e = 5e-2; // deviation allowed to pass test
   CHECK_VECTOR(uexp,u,6);
   CHECK_VECTOR(yexp,y,6);
   CHECK_VECTOR(a1exp,a1,6);
   CHECK_VECTOR(a2exp,a2,6);
   CHECK_VECTOR(b1exp,b1,6);
   CHECK_VECTOR(b2exp,b2,6);
} // it_pid_reg_N2()
//------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function contains the integration test for the
             self-tuning PID controller with order N = 3.
             It checks if the same response is obtained as from the
             Matlab / Simulink model.
    Variables:
        tc : pointer to the current test-suite
  Returns  : -
  ------------------------------------------------------------------*/
void it_pid_reg_N3(CuTest *tc)
{
   FILE          *f1;
   int           N;
   int           i;
   int           vrg;                  // 1 = release controller
   pid_params    p;
   sys_id_params ps;
   double        wk;
   double        yk, yk_1, yk_2, yk_3; // process output
   double        uk, uk_1, uk_2, uk_3; // process input = PID output
   vector        u,y;
   vector        a1,a2,a3,b1,b2,b3;    // parameter estimates
   // The following data comes from the sys_id3 Simulink model
   vector        uexp  = { 0.4304, 0.7813, 0.9586, 0.9947,  0.8344,  1.2922 };
   vector        yexp  = { 0.0000, 0.0304, 0.1548, 0.3652,  0.5982,  0.9997 };
   vector        a1exp = { 0.1000, 0.1000, 0.0973, 0.0956, -0.5313, -1.2743 };
   vector        a2exp = { 0.2000, 0.2000, 0.2000, 0.1995, -0.3625,  0.5356 };
   vector        a3exp = { 0.3000, 0.3000, 0.3000, 0.3000,  0.1247,  0.0304 };
   vector        b1exp = { 0.1000, 0.0706, 0.0706, 0.0706,  0.0706,  0.0706 };
   vector        b2exp = { 0.2000, 0.2000, 0.2385, 0.2383,  0.1941,  0.1416 };
   vector        b3exp = { 0.3000, 0.3000, 0.3000, 0.3072,  0.1223,  0.0137 };

   printf("   Testcase 15: Integration Test pid_reg5(), N = 3\n");
   f1 = fopen("it_pid_log_N3.txt","w");
   N       = 3;   // simulate 3rd order model, Example 4.5, page 126
   p.ts    = 1.0; // sample-period is 1 sec.
   p.kc    = 1.0; // initialize PID controller parameters (just in case)
   p.ti    = 2.0; //
   p.td    = 1.0; //
   p.k_lpf = 0.09; // Filtering of D-term
   init_pid5(&p,N);
   ps.N = N;
   vrg  = 0;                        // disable PID controller at start-up
   uk   = uk_1 = uk_2 = uk_3 = 0.0; // controller output, process input
   yk   = yk_1 = yk_2 = yk_3 = 0.0; // process output
   wk   = 0.0;                      // set-point, reference value
   fprintf(f1,"i wk uk yk a1 a2 a3 b1 b2 b3 Kr Ki Kd Ti Td Kpu Tu\n");

   for (i = 0; i < 150; i++)
   {
      if (i ==  5)       vrg = 1;   // release controller at i = 5
      if (i == 10)       wk  = 1.0; // create step at i = 10
      else if (i ==  50) wk  = 0.5; // create step at i = 50
      else if (i == 100) wk  = 1.0; // create step at i = 100

      //------------------------------------------------------------------------
      // Now calculate the process model Gd(z) of example 4.5 at page 126, with
      // the following transfer function:
      //
      //                      -1      -2      -3
      //                  b1*z  + b2*z  + b3*z
      //       H(z) = ----------------------------
      //                      -1      -2      -3
      //              1 + a1*z  + a2*z  + a3*z
      //
      // with b1 =  0.0706, b2 = 0.1416, b3 = 0.0136
      //      a1 = -1.2747, a2 = 0.5361, a3 = 0.0302
      //------------------------------------------------------------------------
      yk  = 1.2747 * yk_1 - 0.5361 * yk_2 - 0.0302 * yk_3;
      yk += 0.0706 * uk_1 + 0.1416 * uk_2 + 0.0136 * uk_3;
      yk_3 = yk_2;  yk_2 = yk_1; yk_1 = yk;

      pid_reg5(yk,&uk,wk, &p,vrg,&ps,1); // pid controller with self-tuning
      uk_3 = uk_2;  uk_2 = uk_1; uk_1 = uk;

      if ((i >= 10) && (i <= 14))
      {
         y[i-10]  = yk;       u[i-10]  = uk;
         a1[i-10] = theta[0]; a2[i-10] = theta[1];
         a3[i-10] = theta[2]; b1[i-10] = theta[3];
         b2[i-10] = theta[4]; b3[i-10] = theta[5];
      }
      else if (i == 149)
      {
         y[5]  = yk;       u[5]  = uk;
         a1[5] = theta[0]; a2[5] = theta[1];
         a3[5] = theta[2]; b1[5] = theta[3];
         b2[5] = theta[4]; b3[5] = theta[5];
      } // if
      fprintf(f1,"%02d %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f\n",
              i, wk, uk, yk, theta[0], theta[1], theta[2], theta[3], theta[4], theta[5], ps.kr, ps.ki, ps.kd, ps.ti, ps.td, ps.kpu, ps.tu);
   } // for i
   fclose(f1);
   e = 5e-3; // deviation allowed to pass test
   CHECK_VECTOR(uexp,u,6);
   CHECK_VECTOR(yexp,y,6);
   CHECK_VECTOR(a1exp,a1,6);
   CHECK_VECTOR(a2exp,a2,6);
   CHECK_VECTOR(a3exp,a3,6);
   CHECK_VECTOR(b1exp,b1,6);
   CHECK_VECTOR(b2exp,b2,6);
   CHECK_VECTOR(b3exp,b3,6);
} // it_pid_reg_N3()
//------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function contains the test-suite for the vector and
             matrix calculation functions.
             It makes calls to the various test-functions.
  Variables: -
  Returns  : pointer to current CuSuite object
  ------------------------------------------------------------------*/
CuSuite *vector_matrix_suite(void)
{
   CuSuite* suite = CuSuiteNew();

   printf("Start of Unit Test Suite 1: Vector and Matrix calculations\n");
   SUITE_ADD_TEST(suite, ut_vxv2s); // vector x vector to scalar
   SUITE_ADD_TEST(suite, ut_vxv2m); // vector x vector to matrix
   SUITE_ADD_TEST(suite, ut_vxm2v); // vector x matrix to vector
   SUITE_ADD_TEST(suite, ut_mxv2v); // matrix x vector to vector
   SUITE_ADD_TEST(suite, ut_vxs2v); // vector x scalar to vector
   SUITE_ADD_TEST(suite, ut_vav2v); // matrix + vector to vector
   SUITE_ADD_TEST(suite, ut_mxs2m); // matrix x scalar to matrix
   SUITE_ADD_TEST(suite, ut_mam2m); // matrix + matrix to matrix
   return suite;
} // vector_matrix_suite()
//------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function contains the test-suite for the system-
             identification functions.
             It makes calls to the various test-functions.
  Variables: -
  Returns  : pointer to current CuSuite object
  ------------------------------------------------------------------*/
CuSuite *sys_id_suite(void)
{
   CuSuite* suite = CuSuiteNew();

   printf("Start of Unit Test Suite 2: System Identification Functions\n");
   SUITE_ADD_TEST(suite, ut_sys_id1_nodf); // system identification function
   SUITE_ADD_TEST(suite, ut_sys_id1_adf);
   SUITE_ADD_TEST(suite, ut_sys_id2_nodf);
   SUITE_ADD_TEST(suite, ut_ultim_gain_period);
   SUITE_ADD_TEST(suite, ut_calc_pid_parameters);
   return suite;
} // sys_id_suite()
//------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function contains the integration test-suite for
             the entire self-tuning controller.
  Variables: -
  Returns  : pointer to current CuSuite object
  ------------------------------------------------------------------*/
CuSuite *integration_test_suite(void)
{
   CuSuite* suite = CuSuiteNew();

   printf("Start of Integration Test Suite: Entire Self-Tuning Controller\n");
   SUITE_ADD_TEST(suite, it_pid_reg_N2); // 2nd order system, test controller
   SUITE_ADD_TEST(suite, it_pid_reg_N3); // 3rd order system, test controller
   return suite;
} // integration_test_suite()
//------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : This function is the entry-point for all the Unit and
             Integration tests of the Self-Tuning controller library.
             It makes calls to the various test-suites.
  Variables: -
  Returns  : -
  ------------------------------------------------------------------*/
void run_all_tests(void)
{
   CuString *output = CuStringNew();
   CuSuite*  suite  = CuSuiteNew();

   CuSuiteAddSuite(suite, vector_matrix_suite());
   CuSuiteAddSuite(suite, sys_id_suite());
   CuSuiteAddSuite(suite, integration_test_suite());

   CuSuiteRun(suite);
   CuSuiteSummary(suite, output);
   CuSuiteDetails(suite, output);
   printf("%s\n", output->buffer);
} // run_all_tests()
//------------------------------------------------------------

/*------------------------------------------------------------------
  Purpose  : Main function, entry point of unit-tests.
  Variables:
    argc   : Argument count, 1= no arguments, 2= one argument, etc.
    argv[] : Array of strings containing the command-line arguments.
             Not used in this function.
  Returns  : 0 (indicating "all is well" to the operating-system)
  ------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  run_all_tests(); // run all the unit- integration tests
  printf("Press any key...");
  getch();
  return 0; // return-value
} //main()
//------------------------------------------------------------

