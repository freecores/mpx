#ifndef _MIPS_STDARG_H_
#define	_MIPS_STDARG_H_

#define _BSD_VA_LIST_           char *          /* va_list */

typedef _BSD_VA_LIST_	va_list;

#define	va_start(ap, last) \
	((ap) = (va_list)__builtin_next_arg(last))


#define	va_arg(ap, T)							\
	(((T *)(							\
	    (ap) += (/*CONSTCOND*/ __alignof__(T) <= sizeof(int)	\
		? sizeof(int) : ((long)(ap) & 4) + sizeof(T))		\
 	))[-1])

#define	va_end(ap)

#define	__va_copy(dest, src)	((dest) = (src))

#endif /* !_MIPS_STDARG_H_ */