#ifndef __RW_CONFIG_H__
#define __RW_CONFIG_H__

#define _RWCAT2(a, b)    a ## b
#define _RWCAT(a, b)     _RWCAT2 (a, b)
#define _RWEXPAND2(s)    s
#define _RWEXPAND(tag)   _RWCAT (_RWTAG_, _RWEXPAND2 (tag))

#define _RWTAG_mo 1
#if _RWTAG_mo == _RWEXPAND (_RWCONFIG)
#  define _RWCONFIG_mo 1
#endif

#define _RWTAG_mcg 2
#if _RWTAG_mcg == _RWEXPAND (_RWCONFIG)
#  define _RWCONFIG_mcg 1
#endif

#define _RWTAG_mso 3
#if _RWTAG_mso == _RWEXPAND (_RWCONFIG)
#  define _RWCONFIG_mso 1
#endif

#define _RWTAG_mscg 4
#if _RWTAG_mscg == _RWEXPAND (_RWCONFIG)
#  define _RWCONFIG_mscg 1
#endif

#endif
