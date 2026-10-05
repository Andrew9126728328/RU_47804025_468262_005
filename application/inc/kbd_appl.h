#ifndef __KBD_APPL_H__
#define __KBD_APPL_H__

typedef struct appl_kbd_s
{
	int (*start)(void);
}appl_kbd_t;

extern const appl_kbd_t appl_kbd;
#endif /* __KBD_APPL_H__ */
