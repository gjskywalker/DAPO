
typedef short word;    /* 16 bit signed int    */
typedef long longword; /* 32 bit signed int    */
#define MIN_WORD ((-32767) - 1)
#define MAX_WORD (32767)
unsigned char bitoff[256] = {
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
void dut(word *s /* [0..159]     IN/OUT  */)
/*
 *  The goal is to compute the array L_ACF[k].  The signal s[i] must
 *  be scaled in order to avoid an overflow situation.
 */
{
  int k, i;

  word temp;
  word smax;
  word scalauto, n;
  word *sp;
  word sl;

  /*  Search for the maximum.
   */
  smax = 0;
  for (k = 0; k <= 159; k++)
  {
    temp = s[k] < 0 ? (s[k] == MIN_WORD ? MAX_WORD : -s[k]) : s[k];
    if (temp > smax)
      smax = temp;
  }

  /*  Computation of the scaling factor.
   */
  if (smax == 0)
    scalauto = 0;
  else
  {
    longword a = (longword)smax << 16;
    if (a < 0)
    {
      if (a <= -1073741824)
        a = 0;
      a = ~a;
    }

    a = a & 0xffff0000 ? (a & 0xff000000 ? -1 + bitoff[0xFF & (a >> 24)] : 7 + bitoff[0xFF & (a >> 16)]) : (a & 0xff00 ? 15 + bitoff[0xFF & (a >> 8)] : 23 + bitoff[0xFF & a]);
    scalauto = 4 - a; /* sub(4,..) */
  }

  if (scalauto > 0 && scalauto <= 4)
  {
    n = scalauto;
    for (k = 0; k <= 159; k++)
    {
      word a = s[k];
      word b = 16384 >> (n - 1);
      longword prod;
      if (b == MIN_WORD && a == MIN_WORD)
        s[k] = MAX_WORD;
      else
      {
        prod = (longword)a * (longword)b + 16384;
        prod >>= 15;
        s[k] = prod & 0xFFFF;
      }
    }
  }
}