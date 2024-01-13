/*
 * Copyright (c) 2004  Hiroo Ono <hiroo+sj3 at oikumene.gcd.org>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $Id: sj3.h,v 1.1 2004/07/02 18:31:25 hiroo Exp $
 */

#ifndef _SJ3_H_
#define _SJ3_H_  1

#include <stdio.h>
#include <stdarg.h>
#include "sj_struct.h"

#ifndef _WCHARH // "wchar16.h" not included
typedef unsigned short wchar16_t;
#endif
#if !defined(SJ3_SJ2_H_) && !defined(WORDENT_DEFINED) // "sj2.h" not included
#include "sjtool.h"
struct wordent {
	wchar16_t word_str[MAXWORD];
};
#endif

/* cmpstr.c */
int cmpstr(unsigned char *, unsigned char *);

/* code.c */
void codeconv(wchar16_t, unsigned int *);
unsigned short CheckCcode(wchar16_t);
unsigned short strtocode(void);
void ChangeCode(void);
void SetCode(int);
int GetCcode(void);

/* codecnv.c */
int init_code(void);
void cnvcode(unsigned char *);
void printout_mb(FILE *, unsigned char *);
void printout(FILE *, unsigned char *);
void normal_out(char *, ...);
void error_out(char *, ...);

/* common.c */
#ifdef SJ3_CONVERSION_H_ // "conversion.h" included
Conversion *AllocConversion(void);
void FreeConversion(Conversion *);
void SetConversion(Conversion *);
void PushConversion(Conversion *);
void PopConversion(void);
Conversion *GetConversion(void);
#endif
void InitConversion(void);
void DefaultConversion(void);
void EndConversion(void);
int wcbyte(wchar16_t *);

/* comuni.c */
void socket_init(void);
void open_socket(void);
void close_socket(void);
void communicate(void);
void put_flush(void);
void put_byte(int);
void put_work(int);
void put_int(int);
unsigned char *put_string(unsigned char *);
unsigned char *put_ndata(unsigned char *, int);
void get_buf(void);
int get_byte(void);
int get_word(void);
int get_nstring(unsigned char *, int);
unsigned char get_ndata(unsigned char *, int);

/* conv.c */
void convert_stat(void);
void inputprocess(void);
wchar16_t inkey(void);
int sequence(wchar16_t *, int *);
int parse_escape(wchar16_t *, int *, int);
wchar16_t write_pseq(int);

/* dictdisp.c */
void dictdisp(char *output);

/* dictmake.c */
void  dictmake(char *input);

/* display.c */
void checkterm(void);
int outc(int);
int erroutc(int);
void start_guideline(void);
void end_guideline(void);
void put_space(int);
void Backspace(void);
int master_flush(void);
int ThroughIntr(void);
void under_in(void);
void under_out(void);
void reverse_in(void);
void reverse_out(void);
int beep(void);
void CursorSet(unsigned short, unsigned short);
void RegionSet(int, int);
void CursorRead(unsigned short *, unsigned short *);
int CRP(int *, int *);
void unget_inkey2(wchar16_t *, int);
void unget_key(int);
wchar16_t CRI(void);
void Clear_EOL(void);
void StartGuide(unsigned short *, unsigned short *);
void EndGuide(unsigned short, unsigned short);
void Flush(void);
int Cscroll(void);

/* douon.c */
#ifdef SJ3_SJ3LIBSJ3_H_	// "sj3libsj3.h" included
int SJ_getdouon(wchar16_t *, struct douon_sj3 *, int, int, int);
#endif
void exec_douon(void);
void wrap_douon(int);
int Cdouon(wchar16_t *, int);
int Dselect(int);
void Gdouon(wchar16_t *, int, int, int);
int Sdouon(int);
int Rdouon(int);
int Bkanji(wchar16_t *);
#ifdef _SJ3LIB_H_ // "sj3lib.h" included
void PushGD(int, struct studyrec *);
#endif
void cl_gakusyuu(void);
void check_and_gakusyuu(int, int);
void go_gakusyuu(int);

/* edit.c */
void push_obj(int, int);
void pop_obj(int, int);
int Bbig(void);
int Bsmall(int);
void Bdelete(void);
void Chain(int, int);
int exec_edit(void);
int getnum(wchar16_t[], int *);
int Replace(wchar16_t *, wchar16_t *, unsigned short *, int, int);
int Substitute(wchar16_t *, wchar16_t *, unsigned short *, int *, int);
int Insert(wchar16_t *, wchar16_t *, unsigned short *, int);
int EditDel(wchar16_t *, wchar16_t *, unsigned short *, int, int);
void chhstr(wchar16_t *, wchar16_t *, unsigned short *, int);
void Print_EOL(wchar16_t *);
void backspace(int);

/* etc.c */
int exec_etc(int);
void Ssbun(int);
int etc_map(int);
void helplevel(void);
void disp_version(int);
void exec_sjrc(void);
void change_weight(void);
suseconds_t UsecWeight(int);
int dconnect(int);
void set_dcflag(int);
int IsDcflag(void);
int reconnect(void);
int guide_print_pause(wchar16_t *, wchar16_t *);

/* eucmessage.c */
void init_wcvtkey(void);
void init_comstr(void);
void init_hin_val(void);
void init_funcs(void);
void init_mode_val(void);
void init_code_val(void);
int init_messages(void);

/* funckey.c */
void mk_cvtkey(char *term);
char *getkey2(char *istr, char *ostr);
void clear_ukeys(void);
void set_func(struct wordent[]);
wchar16_t eval_keys(wchar16_t *s, wchar16_t *p);
void clear_key(void);
void clear_commkeys(void);
int eval_key(wchar16_t *s);
void clear_etckeys(void);
int set_etckeys(struct wordent[]);
wchar16_t parse_command(wchar16_t *s, int *count);

/* fuzoku.c */
void setclrec(JREC *, unsigned char *, TypeCnct);
void srchfzk(JREC *, unsigned char *, TypeCnct, int);

/* henkan.c */
void exec_henkan(void);
void addBun(int, int);
void delspan(int);
void delBun(int, int);
int IsHex(wchar16_t);
void Getcode(wchar16_t *, wchar16_t *);
int load_bun(int, wchar16_t *, unsigned char);
int load_bun2(int, wchar16_t *, wchar16_t *, unsigned short *);
int getistr(wchar16_t *, wchar16_t *, int);
void exec_muhenkan(int);
int Bstr(int, wchar16_t *);
int mode_muhenkan(int, int);
void Bchange(int);
void Bdisp(int);
void Rdisp(void);
void AfterAllPrint(int, int);
void Bright(void);
void Bleft(void);
void Blast(void);
void Btop(void);

/* hinsi.c */
char *hns2str(int);
int str2hns(char *);

/* init.c */
void seg_count(DICT *);
void mkidxtbl(DICT *);
void initwork(void);

/* kigou.c */
int kigou(void);
int IsRedraw(int);
int IsCKey(int);
void guide_kigou(int, int, int);
void pkigou(int);
void input_kigou(int, int, int);

/* libif.c */
#ifdef SJ3_SJ3LIBSJ3_H_	// "sj3libsj3.h" included
int SJ2_henkan(wchar16_t *, struct bunsetu_sj3 *, wchar16_t *, int);
int SJ2_getdouon(wchar16_t *, struct douon_sj3 *, int, int, int);
#endif
int sjislen(unsigned char *, int);
int euclen(unsigned char *, int);
#ifdef _SJ3LIB_H_ // "sj3lib.h" included
void SJ2_study(struct studyrec *);
void SJ2_clstudy(wchar16_t *, wchar16_t *, struct studyrec *);
#endif
int SJ2_toroku(wchar16_t *, wchar16_t *, int);
int SJ2_syoukyo(wchar16_t *, wchar16_t *, int);
void sj_check_error(int);
int SJ2_reconnect(void);
void SJ2_henkan_end(void);
int SJ2_henkan_init(void);
int sj3_autocon(void);

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

/* romaji.c */
int exec_romaji(wchar16_t);
void move_cur(int);
unsigned short addten(unsigned short, unsigned short);
unsigned short getipos(unsigned short pos);
unsigned short getipos2(unsigned short *, unsigned short);
int sj_rkconv(wchar16_t *, wchar16_t *, int);
int sj_rkconv2(wchar16_t *, wchar16_t *, unsigned short *, int);
int sj_rkconv(wchar16_t *, wchar16_t *, int);
int sj_rkconv2(wchar16_t *, wchar16_t *, unsigned short *, int);
int exec_romaji2(wchar16_t, wchar16_t *, wchar16_t *, unsigned short *, int, int);

/* selclrec.c */
void selclrec(void);

/* screen.c */
void vbackchar(int);
void wrap_off(void);
void wrap_on(void);
void Vput_space(void);
void VBackspace(void);
void master_out(wchar16_t *, int);
void master_write(wchar16_t *, int);
void vprintU(wchar16_t *, char);
void vprintR(wchar16_t *, char);
void IprintU(wchar16_t *, int);
void IprintR(wchar16_t *, int);
void printU(wchar16_t *);
void printR(wchar16_t *);
void Csave(void);
void SetVcol(int);
void Cload(void);
void Cclear(int);
void Cgoto(int);
void ClearToMax(void);
void Ccheck(void);
void spaceR(wchar16_t *);
void clear_guide_line(void);
void print_guide_line(void);
void set_guide_line(int);
void disp_mode(void);
void TopGuide(void);
void guide_print(wchar16_t *, wchar16_t *);

/* sj2code.c */
int sj2cd_chr(unsigned char *, unsigned char *);
int sj2cd_str(unsigned char *, unsigned char *, int);

/* sj3.c */
int main(int argc, char **argv);
void makecore(int);
void init(char **argv);
void init_env(void);
void parsearg(int argc, char **argv);
void usage(void);
void sjinit(void);
void setshellname(void);
void getfixtty(void);
void get_ttymode(void);
void getmaster(void);
void set_jmode(void);
void fixtty(void);
void setdev(void);
void forkshell(void);
void sj3_setenv(char *ename, char *eval, char *buf);
void shellprocess(void);
void getslave(void);
void execcmd(char *cmd, char **ap);
void fail(int);
void done(int);
void done2(void);
void done3(void);
void exitprocess(int);
void suspend(void);
void clearutmpentry(void);
void onwinch(int);
void SetRegion(void);
void set_tty_size(void);
void setsjserv(char *hname);
char *chmyhname(char *hname);

/* sj3ver.c */
void print_version(void);

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

/* sjgetchar.c */
wchar16_t SJ_getchar(void);
void output_master(void);
void set_eucmode(void);
int SJ_write(wchar16_t *, int);
void write_stdout(unsigned char *, int);
int SJ_read(wchar16_t *, int);
void SJ_warnning(wchar16_t *);
void SJ_print(wchar16_t *);
void SJ_through(wchar16_t *, int);

/* sjrc.c */
int getsjrc(void);
void setrc(char *, FILE *);
int much(char *, char *);
int getword(char *, struct wordent[]);
int IsTerminator(unsigned char); // -> sjrc2.c
int isTerminator(unsigned char); // -> sjrc2.c
int IsEscape(unsigned char); // -> sjrc2.c
int IsDelimitor(unsigned char); // -> sjrc2.c
void set_dict(struct wordent[]); // -> sjrc2.c
void set_server(struct wordent[]); // -> sjrc2.c

/* sjrc2.c */
void sjrc_init(void);
void getsjrc2(void);
int setrc2(char *);
void getsjrk(int);
int match(wchar16_t *, wchar16_t *);
int getword2(unsigned char *, struct wordent[]);
int IsTerminator(unsigned char); // -> sjrc.c
int isTerminator(unsigned char); // -> sjrc.c
int IsEscape(unsigned char); // -> sjrc.c
int isDelimitor(unsigned char); // -> sjrc.c
void set_forkshell(struct wordent[]);
void set_intr(struct wordent[]);
void set_delkey(struct wordent[]);
int isintr(unsigned char);
void set_goto(struct wordent[]);
int IsGoto(unsigned char);
void set_trap(struct wordent[]);
int IsTrap(unsigned char);
int mode_key(int mode);
void set_init_mode(struct wordent[]);
void set_helpmenu(struct wordent[]);
void set_defcode(struct wordent[]);
void set_muhenkan(struct wordent[]);
void set_muedit(struct wordent[]);
void set_m_toggle(struct wordent[]);
void set_silent(struct wordent[]);
void set_bstudy(struct wordent[]);
int is_bstudy(void);
int eval_muhenkan(void);
int value_muhenkan(void);
void set_guide(struct wordent[]);
void set_flush_conversion(struct wordent[]);
void set_dict(struct wordent[]); // -> sjrc.c
void set_server(struct wordent[]); // -> sjrc.c

/* stat_conv.c */
void save_span(int);
void save_span_p(void);
int isspaned(void);
void buffers_clear(void);
void pointer_clear(void);
void StoreSaveSet(void);
void RestoreSaveSet(void);
int history_empty(void);
void History(void);
void ClearBuffer(void);
void SetCflag(void);
void ClearCflag(void);
int IsCflag(void);
void exec_kettei(void);
void set_rkebell(struct wordent[]);
void ChangeBuffMode(void);
int BuffYes(void);
int IsHenkan(void);
int IsMuhenkan(void);
void SetMuhenkan(void);
void SetHenkan(void);
void ResetHenkan(void);
void all_set_up(void);
void stat_conv_on(void);
void stat_init(void);
int get_pseq(void);
void stat_conv(void);
void save_obuf(void);
void restore_obuf(void);
void exec_ctrl(int);
void exec_mode(int);
int toggle_mode(int);
int eval_mode(int);
int IsESC(wchar16_t);
void Strncpy(wchar16_t *, wchar16_t *, int);
int is_bsdel(int);
void clear_del(void);
void set_bs(int);
void set_del(int);
int is_bs(int);
int is_del(int);
void SaveConversion(void);
int AnotherConversion(void);

/* term.c */
void set_news(void);

/* terminat.c */
int terminate(TypeCnct, unsigned char *);

/* toroku.c */
int exec_toroku(void);
int getyomi(wchar16_t *, int, wchar16_t *, wchar16_t *);
int gethinshi(void);
int getcheck(wchar16_t *, wchar16_t *, wchar16_t *, wchar16_t *);
int getkstr(wchar16_t *, int, wchar16_t *);
int getbunsetu(wchar16_t *, wchar16_t *);
int exec_syoukyo(void);

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

#endif /* _SJ3_H_ */

