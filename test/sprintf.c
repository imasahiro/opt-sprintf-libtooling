#include <stdio.h>
#include <qrintf.h>

int main(int argc, char const* argv[])
{
    char buf[128];
    _qrintf_chk_finalize(_qrintf_chk_d(_qrintf_chk_s_len(_qrintf_chk_d(_qrintf_chk_s_len(_qrintf_chk_init(buf, 128), "argc=", 5), argc), " - 20=", 6), 20));
    fprintf(stderr, "%s\n", buf);
    return 0;
}
