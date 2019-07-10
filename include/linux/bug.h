#ifndef _LINUX_BUG_H
#define _LINUX_BUG_H

#include <vsprintf.h> /* for panic() */
#include <linux/build_bug.h>
#include <linux/compiler.h>
#include <linux/printk.h>
#include <linux/kconfig.h>

#if defined(CONFIG_TPL_BUILD) || defined(CONFIG_SPL_BUILD)
/*
 * In case of TPL/SPL use a short format not including __FILE__
 * to reduce image size
 */
#define BUG_WARN_LOC_FMT	"%d@%s()"
#define BUG_WARN_LOC_ARGS	__LINE__, __func__
#else
#define BUG_WARN_LOC_FMT	"%s:%d/%s()"
#define BUG_WARN_LOC_ARGS	__FILE__, __LINE__, __func__
#endif

#define BUG() do { \
	printk("BUG at "BUG_WARN_LOC_FMT"!\n", BUG_WARN_LOC_ARGS);	\
	panic("BUG!"); \
} while (0)

#define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)

#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		printk("WARNING at "BUG_WARN_LOC_FMT"!\n", BUG_WARN_LOC_ARGS); \
	unlikely(__ret_warn_on);					\
})

#define WARN(condition, format...) ({                   \
	int __ret_warn_on = !!(condition);              \
	if (unlikely(__ret_warn_on))                    \
		printf(format);                  \
	unlikely(__ret_warn_on);                    \
})

#define WARN_ON_ONCE(condition)	({				\
	static bool __warned;					\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once && !__warned)) {		\
		__warned = true;				\
		WARN_ON(1);					\
	}							\
	unlikely(__ret_warn_once);				\
})

#define WARN_ONCE(condition, format...) ({          \
	static bool __warned;     \
	int __ret_warn_once = !!(condition);            \
								\
	if (unlikely(__ret_warn_once && !__warned)) {       \
		__warned = true;                \
		WARN(1, format);                \
	}                           \
	unlikely(__ret_warn_once);              \
})

#endif	/* _LINUX_BUG_H */
