#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/acct.h>
#include "myapue.h"

#define FMT "%-*.*s e = %6ld, chars = %7ld, %c %c %c %c\n"

static unsigned long
compt2ulong(comp_t comptime) /* convert comp_t to unsigned long */
{
    unsigned long val;
    int exp;

    val = comptime & 0x1fff; /* 13-bit fraction */
    exp = (comptime >> 13) & 7; /* 3-bit exponent (0-7) */
    while (exp-- > 0)
        val *= 8;
    return(val);
}

int main(int argc, char **argv)
{
    struct acct acdata;
    FILE *fp;

    if (argc !=2 )
        err_quit("usage: pracct filename");
    if ((fp = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);
    while ((fread(&acdata, sizeof(acdata), 1, fp)) == 1) {
        printf(FMT, (int)sizeof(acdata.ac_comm), 
                (int)sizeof(acdata.ac_comm), acdata.ac_comm,
                compt2ulong(acdata.ac_etime),
                compt2ulong(acdata.ac_io),
                acdata.ac_flag & ACORE ? 'D' : ' ', /* dumped core */
                acdata.ac_flag & AXSIG ? 'X' : ' ', /* killed by signal */
                acdata.ac_flag & AFORK ? 'F' : ' ', /* fork'd but not exec'd */
                acdata.ac_flag & ASU   ? 'S' : ' ');/* used super-user */
    }
    if (ferror(fp))
        err_sys("read error");

    return 0;
}
