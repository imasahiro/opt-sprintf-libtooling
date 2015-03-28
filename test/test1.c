#include <stdio.h>
#include <qrintf.h>

int main(int argc, char const* argv[])
{
    char buf[128];
#define PRINTF(BUF, BUFLEN, FMT, VAL) \
    snprintf(BUF, BUFLEN, FMT, VAL)
    _qrintf_chk_finalize(_qrintf_chk_c(_qrintf_chk_init(buf, sizeof(buf)), 'Z'));
    // _qrintf_chk_finalize(_qrintf_chk_c(_qrintf_chk_init(buf, 128), 'z'));
    return 0;
}
