#include <stdio.h>
#include <errno.h>
#include "simio.h"
static void __inline__ doopen(PSIMIO psimio) {
  __asm__(".pword 0xDAC000" :: "a" (psimio) : "memory");
}

int __attribute__((__weak__, __section__(".libc")))
open(const char *name, int access, int mode)
{
  SIMIO simio;
  register PSIMIO psimio asm("w0") = &simio;

  simio.method = SIM_OPEN;
  simio.u.open.name = name;
  simio.u.open.access = access;
  simio.u.open.mode = mode;
  simio.u.open.handle = _FD_INVALID;
  doopen(psimio);

  if (simio.u.open.handle == _FD_INVALID) {
#ifdef __C30_LEGACY_LIBC__
    errno = EFOPEN;
#else
    errno = EBADF;
#endif
  }
  return(simio.u.open.handle);
}

