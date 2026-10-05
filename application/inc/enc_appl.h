#ifndef __ENC_APPL_H__
#define __ENC_APPL_H__

typedef struct appl_enc_s
{
	int (*start)(void);
}appl_enc_t;

extern const appl_enc_t appl_enc;
#endif /* __ENC_APPL_H__ */
