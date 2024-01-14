/*
 * Copyright (c) 2023 FUJIMI-IM project
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef SJ3_SJ3PRIV_H_
#define SJ3_SJ3PRIV_H_

#include "sj_struct.h"

#ifndef _WCHARH // "wchar16.h" not included
typedef unsigned short wchar16_t;
#endif

/* cmpstr.c */
int cmpstr(unsigned char *, unsigned char *);

/* fuzoku.c */
void setclrec(JREC *, unsigned char *, TypeCnct);
void srchfzk(JREC *, unsigned char *, TypeCnct, int);

/* init.c */
void seg_count(DICT *);
void mkidxtbl(DICT *);
void initwork(void);

/* main.c */
int make_lockfile(void);
int erase_lockfile(void);
void server_terminate(void);

/* memory2.c */
JREC *free_jlst(JREC *);
CLREC *free_clst(CLREC *, int);
void free_clall(CLREC *);
void free_jall(JREC *);
void freework(void);

/* mk2claus.c */
void mk2claus(void);

/* mkbunset.c */
void mkbunsetu(void);
CLREC *argclrec(int);

/* mvmemd.c */
void mvmemd(unsigned char *, unsigned char *, int);

/* mvmemi.c */
void mvmemi(unsigned char *, unsigned char *, int);

/* rk_conv.c */
void sj3_rkcode(int);
int sj3_rkinit2(char *, int);
int rcode_sjis_init(void);
int rcode_euc_init(void);
int sj3_rkinit(char *);
int sj3_rkinit_euc(char *);
int sj3_rkinit_mb(char *);
int sj3_rkinit_sub(char *, int(*)(void));
char *getkey(char *, wchar16_t *, int *);
char *rkgetyomi(char *, wchar16_t *, int *);
void cltable(void);
int chk_rstr(wchar16_t *, wchar16_t *, int, int);
#ifdef SJ3_RK_H_ // "rk.h" included
RkTablW16 *rkalloc(void);
RkTablW16 *mktable(wchar16_t *, int);
#endif
int stradd(wchar16_t **, wchar16_t *, int);
int kstradd(wchar16_t **, wchar16_t *, int);
void sj3_rkebell(int);
void sj3_rkclear(void);
void sj3_rkreset(void);
void sj3_rkconvc(wchar16_t, unsigned int *);
int sj3_rkconv2(wchar16_t *, unsigned int *, int);
int sj3_rkconv(unsigned char *, unsigned char *);
int sj3_rkconv_euc(unsigned char *, unsigned char *);
int sj3_rkconv_mb(unsigned char *, unsigned char *);
int sj3_rkconv_w16(wchar16_t *, wchar16_t *);
int rkmatch(wchar16_t *, wchar16_t *, int);

/* selclrec.c */
void selclrec(void);

/* sj2code.c */
int sj2cd_chr(unsigned char *, unsigned char *);
int sj2cd_str(unsigned char *, unsigned char *, int);

/* sj3_rkcv.c */
void setl_hktozh(void);
void mkkigou(void);
unsigned short sj_addten(unsigned short, unsigned short);
unsigned short sj_han2zen(unsigned short);
int sj3_hantozen(unsigned char *, unsigned char *);
int sj3_hantozen_euc(unsigned char *, unsigned char *);
int sj3_hantozen_mb(unsigned char *, unsigned char *);
int sj3_hantozen_w16(wchar16_t *, wchar16_t *);
int sj_hantozen(wchar16_t *, wchar16_t *, int);
unsigned short sj_zen2han(unsigned short);
int sj3_zentohan(unsigned char *, unsigned char *);
int sj3_zentohan_euc(unsigned char *, unsigned char *);
int sj3_zentohan_mb(unsigned char *, unsigned char *);
int sj3_zentohan_w16(wchar16_t *, wchar16_t *);
int sj_zentohan(wchar16_t *, wchar16_t *, int);
wchar16_t sj_tokata(wchar16_t);
wchar16_t sj_tohira(wchar16_t);
void sj_htok(wchar16_t *, wchar16_t *);
void sj_ktoh(wchar16_t *, wchar16_t *);

/* terminat.c */
int terminate(TypeCnct, unsigned char *);

/* wc16_str.c */
int sj3_iswcntrl16(wchar16_t);
int sj3_iswupper16(wchar16_t);
int sj3_iswdigit16(wchar16_t);
int sj3_iswxdigit16(wchar16_t);
int sj3_wslen16(wchar16_t *);
int sj3_wscmp16(wchar16_t *, wchar16_t *);
int sj3_wsncmp16(wchar16_t *, wchar16_t *, int);
wchar16_t *sj3_wscpy16(wchar16_t *, wchar16_t *);
wchar16_t *sj3_wsncpy16(wchar16_t *, wchar16_t *, int);
wchar16_t *sj3_wscat16(wchar16_t *, wchar16_t *);
wchar16_t sj3_euc2wc16(unsigned int);
wchar16_t sj3_sjis2wc16(unsigned int);
unsigned int sj3_wc2euc16(wchar16_t);
unsigned int sj3_wc2sjis16(wchar16_t);
int sj3_mbstowcs16(wchar16_t *, unsigned char *, int);
int sj3_wcstombs16(unsigned char *, wchar16_t *, int);

#endif /* SJ3_SJ3PRIV_H_ */
