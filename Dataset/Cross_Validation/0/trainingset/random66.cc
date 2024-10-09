/*
+--------------------------------------------------------------------------+
| CHStone : a suite of benchmark programs for C-based High-Level Synthesis |
| ======================================================================== |
|                                                                          |
| * Collected and Modified : Y. Hara, H. Tomiyama, S. Honda,               |
|                            H. Takada and K. Ishii                        |
|                            Nagoya University, Japan                      |
|                                                                          |
| * Remark :                                                               |
|    1. This source code is modified to unify the formats of the benchmark |
|       programs in CHStone.                                               |
|    2. Test vectors are added for CHStone.                                |
|    3. If "main_result" is 0 at the end of the program, the program is    |
|       correctly executed.                                                |
|    4. Please follow the copyright of each benchmark program.             |
+--------------------------------------------------------------------------+
*/
/*============================================================================

This C header file is part of the SoftFloat IEC/IEEE Floating-point Arithmetic
Package, Release 2b.

Written by John R. Hauser.  This work was made possible in part by the
International Computer Science Institute, located at Suite 600, 1947 Center
Street, Berkeley, California 94704.  Funding was partially provided by the
National Science Foundation under grant MIP-9311980.  The original version
of this code was written as part of a project to build a fixed-point vector
processor in collaboration with the University of California at Berkeley,
overseen by Profs. Nelson Morgan and John Wawrzynek.  More information
is available through the Web page `http://www.cs.berkeley.edu/~jhauser/
arithmetic/SoftFloat.html'.

THIS SOFTWARE IS DISTRIBUTED AS IS, FOR FREE.  Although reasonable effort has
been made to avoid it, THIS SOFTWARE MAY CONTAIN FAULTS THAT WILL AT TIMES
RESULT IN INCORRECT BEHAVIOR.  USE OF THIS SOFTWARE IS RESTRICTED TO PERSONS
AND ORGANIZATIONS WHO CAN AND WILL TAKE FULL RESPONSIBILITY FOR ALL LOSSES,
COSTS, OR OTHER PROBLEMS THEY INCUR DUE TO THE SOFTWARE, AND WHO FURTHERMORE
EFFECTIVELY INDEMNIFY JOHN HAUSER AND THE INTERNATIONAL COMPUTER SCIENCE
INSTITUTE (possibly via similar legal warning) AGAINST ALL LOSSES, COSTS, OR
OTHER PROBLEMS INCURRED BY THEIR CUSTOMERS AND CLIENTS DUE TO THE SOFTWARE.

Derivative works are acceptable, even for commercial purposes, so long as
(1) the source code for the derivative work includes prominent notice that
the work is derivative, and (2) the source code includes prominent notice with
these four paragraphs for those parts of this code that are retained.

=============================================================================*/

/*----------------------------------------------------------------------------
| Software IEC/IEEE floating-point types.
*----------------------------------------------------------------------------*/
typedef unsigned int float32;
typedef unsigned long long float64;

/*----------------------------------------------------------------------------
| Software IEC/IEEE floating-point underflow tininess-detection mode.
*----------------------------------------------------------------------------*/
#define float_tininess_after_rounding   0
#define float_tininess_before_rounding  1

/*----------------------------------------------------------------------------
| Software IEC/IEEE floating-point rounding mode.
*----------------------------------------------------------------------------*/
#define float_round_nearest_even  0
#define float_round_to_zero       1
#define float_round_up            2
#define float_round_down          3

/*----------------------------------------------------------------------------
| Software IEC/IEEE floating-point exception flags.
*----------------------------------------------------------------------------*/
#define float_flag_inexact     1
#define float_flag_divbyzero   2
#define float_flag_underflow   4
#define float_flag_overflow    8
#define float_flag_invalid    16

#define N 46
const float64 a_input[N] = {
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC000000000000000ULL,	/* -2.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0xFFF0000000000000ULL,	/* -inf */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xC000000000000000ULL		/* -2.0 */
};

const float64 b_input[N] = {
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0x7FF0000000000000ULL,	/* inf */
  0x7FF0000000000000ULL,	/* inf */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC000000000000000ULL,	/* -2.0 */
  0xFFF0000000000000ULL,	/* -inf */
  0xFFF0000000000000ULL,	/* -inf */
  0x8000000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xFFF0000000000000ULL,	/* -inf */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xC000000000000000ULL,	/* -2.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL		/* 1.5 */
};

const float64 z_output[N] = {
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x400C000000000000ULL,	/* 3.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x400C000000000000ULL,	/* 3.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x0000000000000000ULL,	/* 0.0 */
  0x4004000000000000ULL,	/* 2.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC00C000000000000ULL,	/* -3.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC00C000000000000ULL,	/* -3.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0x8000000000000000ULL,	/* -0.0 */
  0xC004000000000000ULL,	/* -2.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FFFFFFFFFFFFFFFULL,	/* nan */
  0x0000000000000000ULL,	/* 0.0 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFE0000000000000ULL,	/* -0.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FE0000000000000ULL,	/* 0.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FFFFFFFFFFFFFFFULL,	/* nan */
  0x0000000000000000ULL,	/* 0.0 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FE0000000000000ULL,	/* 0.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFE0000000000000ULL		/* -0.5 */
};
typedef int flag;
typedef int int8;
typedef int int16;

/*----------------------------------------------------------------------------
| Each of the following `typedef's defines a type that holds integers
| of _exactly_ the number of bits specified.  For instance, for most
| implementation of C, `bits16' and `sbits16' should be `typedef'ed to
| `unsigned short int' and `signed short int' (or `short int'), respectively.
*----------------------------------------------------------------------------*/
typedef unsigned short int bits16;
typedef unsigned int bits32;
typedef unsigned long long int bits64;
typedef signed long long int sbits64;

/*----------------------------------------------------------------------------
| The `LIT64' macro takes as its argument a textual integer literal and
| if necessary ``marks'' the literal as having a 64-bit integer type.
| For example, the GNU C Compiler (`gcc') requires that 64-bit literals be
| appended with the letters `LL' standing for `long long', which is `gcc's
| name for the 64-bit integer type.  Some compilers may allow `LIT64' to be
| defined as the identity macro:  `#define LIT64( a ) a'.
*----------------------------------------------------------------------------*/
#define LIT64( a ) a##LL
int8 float_exception_flags = 0;
int dut ()
{
  int main_result, result;
  int i;
  float64 x1, x2;
  main_result = 0;
  for (i = 0; i < N; i++)
    {
        float64 result;
        x1 = a_input[i];
        x2 = b_input[i];
        flag aSign, bSign;
        aSign = x1 >> 63;
        bSign = x2 >> 63;
        if (aSign == bSign)
        {
            int16 aExp, bExp, zExp;
            bits64 aSig, bSig, zSig;
            int16 expDiff;

            aSig = x1 & LIT64 (0x000FFFFFFFFFFFFF);
            aExp = (x1 >> 52) & 0x7FF;
            bSig = x2 & LIT64 (0x000FFFFFFFFFFFFF);
            bExp = (x2 >> 52) & 0x7FF;
            expDiff = aExp - bExp;
            aSig <<= 9;
            bSig <<= 9;
            if (0 < expDiff)
            {
                if (aExp == 0x7FF)
                {
                    if (aSig)
                    {
                        flag aIsNaN, aIsSignalingNaN, bIsNaN, bIsSignalingNaN;
                        aIsNaN = (LIT64 (0xFFE0000000000000) < (bits64) (x1 << 1));
                        aIsSignalingNaN = (((x1 >> 51) & 0xFFF) == 0xFFE) && (x1 & LIT64 (0x0007FFFFFFFFFFFF));
                        bIsNaN = (LIT64 (0xFFE0000000000000) < (bits64) (x2 << 1));
                        bIsSignalingNaN = (((x2 >> 51) & 0xFFF) == 0xFFE) && (x2 & LIT64 (0x0007FFFFFFFFFFFF));
                        x1 |= LIT64 (0x0008000000000000);
                        x2 |= LIT64 (0x0008000000000000);
                        if (aIsSignalingNaN | bIsSignalingNaN)
                            float_exception_flags |= float_flag_invalid;
                        result =  bIsSignalingNaN ? x2 : aIsSignalingNaN ? x1 : bIsNaN ? x2 : x1;
                    }
                    result =  x1;
                }
                if (bExp == 0)
                    --expDiff;
                else
                    bSig |= LIT64 (0x2000000000000000);
                bits64 z;
                if (expDiff == 0)
                {
                    z = bSig;
                }
                else if (expDiff < 64)
                {
                    z = (bSig >> expDiff) | ((bSig << ((-expDiff) & 63)) != 0);
                }
                else
                {
                    z = (bSig != 0);
                }
                bSig = z;
                zExp = aExp;
            }
            else if (expDiff < 0)
            {
                if (bExp == 0x7FF)
                {
                    if (bSig)
                    {
                        flag aIsNaN, aIsSignalingNaN, bIsNaN, bIsSignalingNaN;
                        aIsNaN = (LIT64 (0xFFE0000000000000) < (bits64) (x1 << 1));
                        aIsSignalingNaN = (((x1 >> 51) & 0xFFF) == 0xFFE) && (x1 & LIT64 (0x0007FFFFFFFFFFFF));
                        bIsNaN = (LIT64 (0xFFE0000000000000) < (bits64) (x2 << 1));
                        bIsSignalingNaN = (((x2 >> 51) & 0xFFF) == 0xFFE) && (x2 & LIT64 (0x0007FFFFFFFFFFFF));
                        x1 |= LIT64 (0x0008000000000000);
                        x2 |= LIT64 (0x0008000000000000);
                        if (aIsSignalingNaN | bIsSignalingNaN)
                            float_exception_flags |= float_flag_invalid;
                        result = bIsSignalingNaN ? x2 : aIsSignalingNaN ? x1 : bIsNaN ? x2 : x1;
                    }
                    result = (((bits64) aSign) << 63) + (((bits64) 0x7FF) << 52) + 0;
                }
                if (aExp == 0)
                    ++expDiff;
                else
                {
                    aSig |= LIT64 (0x2000000000000000);
                }
                bits64 z;
                if (expDiff == 0)
                {
                    z = aSig;
                }
                else if (-expDiff < 64)
                {
                    z = (aSig >> (-expDiff)) | ((aSig << ((expDiff) & 63)) != 0);
                }
                else
                {
                    z = (aSig != 0);
                }
                aSig = z;
                zExp = bExp;
            }
            else
            {
                if (aExp == 0x7FF)
                {
                    if (aSig | bSig)
                    {
                        flag aIsNaN, aIsSignalingNaN, bIsNaN, bIsSignalingNaN;
                        aIsNaN = (LIT64 (0xFFE0000000000000) < (bits64) (x1 << 1));
                        aIsSignalingNaN = (((x1 >> 51) & 0xFFF) == 0xFFE) && (x1 & LIT64 (0x0007FFFFFFFFFFFF));
                        bIsNaN = (LIT64 (0xFFE0000000000000) < (bits64) (x2 << 1));
                        bIsSignalingNaN = (((x2 >> 51) & 0xFFF) == 0xFFE) && (x2 & LIT64 (0x0007FFFFFFFFFFFF));
                        x1 |= LIT64 (0x0008000000000000);
                        x2 |= LIT64 (0x0008000000000000);
                        if (aIsSignalingNaN | bIsSignalingNaN)
                            float_exception_flags |= float_flag_invalid;
                        result = bIsSignalingNaN ? x2 : aIsSignalingNaN ? x1 : bIsNaN ? x2 : x1;
                    }
                    result = x1;
                }
                if (aExp == 0)
                    result = ((((bits64) aSign) << 63) + (((bits64) 0) << 52) + (aSig + bSig)) >> 9;
                zSig = LIT64 (0x4000000000000000) + aSig + bSig;
                zExp = aExp;
            }
            aSig |= LIT64 (0x2000000000000000);
            zSig = (aSig + bSig) << 1;
            --zExp;
            if ((sbits64) zSig < 0)
            {
                zSig = aSig + bSig;
                ++zExp;
            }
            result = (((bits64) aSign) << 63) + (((bits64) zExp) << 52) + zSig;
        }
        main_result += (result != z_output[i]);
    }
    return main_result;
}
