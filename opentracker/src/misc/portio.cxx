#ifdef WIN32

#include <Windows.h>

BYTE inportb(UINT portid)
{
    unsigned char value;
    __asm mov edx,portid
    __asm in al,dx
    __asm mov value,al
    return value;
}

void outportb(UINT portid, BYTE value)
{
    __asm mov edx,portid
    __asm mov al,value
    __asm out dx,al
}

#endif

/* 
 * ------------------------------------------------------------
 *   End of portio.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
