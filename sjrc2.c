/*
 * Copyright (c) 1994  Sony Corporation
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
 * $SonyRCSfile: sjrc.c,v $  
 * $SonyRevision: 1.4 $ 
 * $SonyDate: 1997/01/23 11:20:54 $
 */



#include <string.h>
#include "wchar16.h"
#include "kctype.h"
#include "common.h"
#include "key.h"
#include <locale.h>
#include "Paths.h"
#include "sj3.h"

int		vflag = 1;
extern int	shell_flag;

char		RCfile[LONGLENGTH];
extern char	shellprog[];
extern char	home[];
extern char	term_name[];
extern int      current_locale;

#define GUIDELEN	80

wchar16_t	guide_sj2[GUIDELEN];
wchar16_t	guide_buffer[GUIDELEN];
wchar16_t	guide_unbuffer[GUIDELEN];
wchar16_t	guide_hira[GUIDELEN];
wchar16_t	guide_zkata[GUIDELEN];
wchar16_t	guide_hkata[GUIDELEN];
wchar16_t	guide_halpha[GUIDELEN];
wchar16_t	guide_zalpha[GUIDELEN];
wchar16_t	guide_shift[GUIDELEN];
wchar16_t	guide_jis[GUIDELEN];
wchar16_t	guide_kuten[GUIDELEN];
wchar16_t	guide_etc[GUIDELEN];
wchar16_t	guide_etcfunc[GUIDELEN];
wchar16_t	guide_function[GUIDELEN];

wchar16_t guide_euc[GUIDELEN];
wchar16_t guide_euc2[GUIDELEN];
wchar16_t guide_jis2[GUIDELEN];
wchar16_t guide_kuten2[GUIDELEN];
static char	*rcfile = ".sjrc";
static char	*rkfile = ".sjrk";

static int	intr_code = 0x03;
static char	goto_code[LONGLENGTH];
static int	goto_num = 0;
static char	trap_code[LONGLENGTH];
static int	trap_num = 0;
static int	def_muhenkan	= KEY_HALPHA;
static int	def_muedit	= KEY_ZHIRA;
static int	muhenkan_toggle = 1;
static int	bstudy = 1;

char **dict_list = NULL;
int  dict_num = 0;

#define SJ3_NO		-1
#define SJ3_SJIS	0
#define SJ3_EUC		1


static int user_euc = 0;
static int file_code = SJ3_NO;	
static int locale_code = SJ3_SJIS;
extern wchar16_t *Key, *Escape, *Sjxkey, *Etckey, *Intr, *Bskey, *Delkey, 
               *Setnormal, *Throughnext, *Initialmode, *Helpmode, 
               *Defaultcode, *Muhenkanmode, *Muhenkaninedit, *Muhenkantoggle, 
               *Guide, *Forkshell, *Execshell, *Dictionary, *Userdic, 
               *Maindic, *Romaji, *Bstrudy, *Silent, *Flushafterconversion, 
               *Rkerrbell, *Server;
extern wchar16_t *Halpha,	*Zalpha, *Hkata, *Zkata, *Hira;
extern wchar16_t *Shiftjis, *Sjis, *Euc, *Euc2, *Jis, *Jis2, *Kuten, *Kuten2;

extern struct functbl funcs[];
extern struct valtbl mode_val[], code_val[];

#define TOLOWER(c) (isupper(c) ? tolower(c) : (c))	

static void sjset_code(void)
{
	char *loc;

        if (current_locale == LC_CTYPE_EUC) {
		locale_code = SJ3_EUC;
		user_euc = 1;
		file_code = SJ3_NO;
	} else
		user_euc = 0;
}
	
void sjrc_init(void)
{
	clear_del ();
	clear_key ();
	intr_code = 0x03;
	goto_num = 0;
	trap_num = 0;
	def_muhenkan = KEY_HALPHA;
	def_muedit = KEY_ZHIRA;
	muhenkan_toggle = 1;
	bstudy = 1;
	flush_conversion = 0;
	if (dict_list) {
		if (dict_list[0]) free(dict_list[0]);
		free(dict_list);
		dict_list = NULL;
	}
	dict_num = 0;
}



void getsjrc2(void)
{
	char *p;
	char *getenv();

	sjset_code();
	mk_cvtkey(term_name);
	RCfile[0] = '\0';
	if ((p = getenv("SJRC")) != NULL && *p != '\0') {
		if (*p != '/') {
			strcpy(RCfile, home);
			strcat(RCfile, "/");
		}
		strcat(RCfile, p);
		if (setrc2(RCfile) == TRUE)
			return;
	} 
	if (home[0] != '\0') {
		strcpy(RCfile, home);
		strcat(RCfile, "/");
		strcat(RCfile, rcfile);
		if (setrc2(RCfile) == TRUE)
			return;
	}
        strcpy(RCfile, DEFRCFILE);
        if (setrc2(RCfile) == TRUE)
                return;

	RCfile[0] = '\0';
}



int setrc2(char *file)
{
	wchar16_t	*p;
	char		line[MAXLINE];
	struct wordent	word[WORDN];
	struct functbl *functp;
	FILE *fd, *fopen();

	if (vflag > 1)
		aprintf ("sjrc file is %s\n", file);

	if ((fd = fopen(RCfile, "r")) == NULL) 
		return(FALSE);
	sjrc_init ();
	while (fgets (line, MAXLINE, fd) != NULL) {
		if (getword2(line, word) <= 0)
			continue;
		functp = funcs;
		p = word[0].word_str;
		while (functp->keyword != NULL) {
			if (match(p, functp->keyword)) {
				(*(functp->func))(word);
				break;;
			}
			functp++;
		}
	}
	fclose(fd);
	return(TRUE);
}



void getsjrk(int erase)
{
	char *p;
	int i;
	char *getenv();
	char RKfile[LONGLENGTH];

	RKfile[0] = '\0';
	sj3_rkcode(1);
	if ((p = getenv("SJRK")) != NULL && *p != '\0') {
		if (*p != '/') {
			strcpy(RKfile, home);
			strcat(RKfile, "/");
		}
		strcat(RKfile, p);
		if ((i = sj3_rkinit2(RKfile, erase)) == TRUE) {
			if (!oasys)	
				mkkigou();
			return;
		}
		if (i == FALSE)
			done3();
	} 
	if (home[0] != '\0') {
		strcpy(RKfile, home);
		strcat(RKfile, "/");
		strcat(RKfile, rkfile);
		if ((i = sj3_rkinit2(RKfile, erase)) == TRUE) {
			if (!oasys)	
				mkkigou();
			return;
		}
		if (i == FALSE)
			done3();
	}
	sj3_rkcode(0);
	if ((i = sj3_rkinit2(DEFRKFILE, erase)) != TRUE) {
		if (i == 1) {
			fprintf(stderr, "Warning cannot open rule file %s\n\r",
				DEFRKFILE);
			(void)sj3_rkinit2("/dev/null", erase);
		} else
			done3();
	}
	if (!oasys)	
		mkkigou();
}



int match(wchar16_t *s1, wchar16_t *s2)
{
	wchar16_t c1, c2;

	if (s1 == NULL)
		return(0);
	while (*s2 != '\0') {
		c1 = *s1++;
		c2 = *s2++;
		if (TOLOWER(WcLowByte(c1)) != TOLOWER(WcLowByte(c2)))
			return(0);
	}
	return(1);
}


int getword2(unsigned char *s, struct wordent word[])
{
	unsigned char	c;
	wchar16_t *w;
	wchar16_t wcbuf[MAXLINE*2];
	wchar16_t ttmp[MAXLINE];
	unsigned char  buf[MAXLINE*2];
	unsigned char *p;
	int	i, wcount;
	unsigned short cc;

	if (IsDelimitor(*s)) {
		s++;
	} else {
		p = s;
		while(!IsDelimitor(*s)) {
			if (IsTerminator(*s) || isTerminator(*s))
				break;
			s++;
		}
		*s++ = '\0';
                if (file_code == locale_code) {
			(void) mbstowcs(wcbuf, (char *)p, MAXLINE*2);
		} else {
			if (file_code == SJ3_SJIS) {
				(void) sjistoeuc(buf, MAXLINE*2, (unsigned char *)p, MAXLINE);
			} else if (file_code == SJ3_EUC) {
				(void) euctosjis(buf, MAXLINE*2, (unsigned char *)p, MAXLINE);
			} else {
				(void) strcpy(buf, p);
			}
			(void) mbstowcs(wcbuf, (char *)buf, MAXLINE*2);
		}
                (void) mbstowcs(ttmp, term_name, MAXLINE);
		if (match(wcbuf, ttmp) == 0)
			return(0);
	}
	i = wcount = 0;
	w = word[wcount].word_str;
	while (*s != '\0') {
		c = *s++;
		if (file_code == SJ3_SJIS) {
			if (issjis1(c) && issjis2(*s)) {
				if (i < MAXWORD - 1) {
					cc = (c << 8) + *s;
					*w++ = sjis2euc(cc);
					i ++ ;
				}
				s++;
				continue;
			}
		} else if (file_code == SJ3_EUC) {
			if (iseuc(c) && iseuc(*s)) {
				if (i < MAXWORD - 1) {
					*w++ = (c << 8) + (*s & 0xff);
					i ++;
				}
				s++;
				continue;
			} 
			if (iseuckana(c) && iskana2(*s))
				c = *s++;
                        if (IsEUCHojo(c) && iseuc(*s) && iseuc(s[1])) {
                                cc = WcSetX0212(*s, s[1]);
				*w++ = cc;
                                s += 2;
				continue;
			}
		} else {
			if (issjis1(c) && issjis2(*s)) {
				file_code = SJ3_SJIS;
				s--;
				continue;
			}
			if ((iseuc(c) && iseuc(*s)) || 
				(iseuckana(c) && iskana2(*s)) || 
			        (IsEUCHojo(c) && iseuc(*s) && iseuc(s[1]))) {
				file_code = SJ3_EUC;
				s--;
				continue;
			}
		}
		if (isTerminator(c))
			break;
		if (IsDelimitor(c)) {
			if (i > 0) {
				if (++wcount >= WORDN - 1)
					break;
				*w = '\0';
				i = 0;
				w = word[wcount].word_str;
			}
			continue;
		}
		if (IsEscape(c))
			c = *s++;
		if (IsTerminator(c))
			break;
		if (i < MAXWORD - 1) {
			*w++ = c;
			i++;
		}
	}
	*w = '\0';
	if (word[wcount].word_str[0] != '\0')
		word[++wcount].word_str[0] = '\0';
	return(wcount);
}

int IsTerminator(unsigned char c)
{
	if (c == '\n')
		return (1);
	else
		return (0);
}

int isTerminator(unsigned char c)
{
	if (c == '#')
		return (1);
	else
		return (0);
}

int IsEscape(unsigned char c)
{
	if (c == '\\')
		return (1);
	else
		return (0);
}

int IsDelimitor(unsigned char c)
{
	if (c == ' ' || c == '\t' || c == '.')
		return (1);
	else
		return (0);
}

void set_forkshell(struct wordent word[])
{
	if (word[1].word_str[0] != '\0' && shellprog[0] == '\0') {
/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
		strcpy (shellprog, (char *)word[1].word_str);
		shell_flag++;
	}
}

void set_intr(struct wordent word[])
{
	int	c;

	if (word[1].word_str[0] != '\0')
		if ((c = eval_key (word[1].word_str)) != -1)
			intr_code = c;
}

void set_delkey(struct wordent word[])
{
	int	c;

	if (word[1].word_str[0] != '\0')
		if ((c = eval_key (word[1].word_str)) != -1)
			set_del (c);
}

int isintr(unsigned char c)
{
	if (c == intr_code)
		return (1);
	else
		return (0);
}

void set_goto(struct wordent word[])
{
	int	c;

	if (word[1].word_str[0] != '\0')
		if ((c = eval_key (word[1].word_str)) != -1)
			goto_code[goto_num ++] = c;
}

int IsGoto(unsigned char c)
{
	int	i;

	for (i = 0 ; i < goto_num ; i ++) {
		if (c == goto_code[i])
			return (1);
	}
	return (0);
}

void set_trap(struct wordent word[])
{
	int	c;

	if (word[1].word_str[0] != '\0')
		if ((c = eval_key (word[1].word_str)) != -1)
			trap_code[trap_num ++] = c;
}

int IsTrap(unsigned char c)
{
	int	i;

	for (i = 0 ; i < trap_num ; i ++) {
		if (c == trap_code[i])
			return (1);
	}
	return (0);
}

int mode_key(int mode)
{
	switch (mode) {
	case MODE_HALPHA:
		return (KEY_HALPHA);
		break;
	case MODE_ZALPHA:
		return (KEY_ZALPHA);
		break;
	case MODE_HKATA:
		return (KEY_HKATA);
		break;
	case MODE_ZKATA:
		return (KEY_ZKATA);
		break;
	case MODE_ZHIRA:
	default:
		return (KEY_ZHIRA);
		break;
	}
}

int	Direct = 0;

void set_init_mode(struct wordent word[])
{
	Conversion	*cv;
	int	i, j;
	wchar16_t	*p;

	cv = GetConversion ();
	j = 1;
	p = word[j].word_str;
	while (*p != '\0') {
		if (match (p, WCBuff))
			cv->BuffMode = BUFF;
		else if (match (p, WCUnbuff))
			cv->BuffMode = UNBUFF;
		else if (match (p, WCDirect))
			Direct = 1;
		else {
			for (i = 0 ; i < 5 ; i ++) {
				if (match (p, mode_val[i].keyword)) {
					cv->Imode = mode_val[i].ivalue;
					break;
				}
			}
		}
		p = word[++j].word_str;
	}
}

void set_helpmenu(struct wordent word[])
{
	if (match (word[1].word_str, WCOff))
		HHlevel = Hlevel = 1;
	else if (match (word[1].word_str, WCOn))
		HHlevel = Hlevel = 2;
}

void set_defcode(struct wordent word[])
{
	wchar16_t *p;
	int i;
	
	p = word[1].word_str;
	for (i = 0; i < 5; i++) {
		if (match(p, code_val[i].keyword)) {
			SetCode(code_val[i].ivalue);
			break;
		}
	}
}

void set_muhenkan(struct wordent word[])
{
	int	i;
	wchar16_t	*p;

	p = word[1].word_str;
	for (i = 0 ; i < 5 ; i ++) {
		if (match (p, mode_val[i].keyword)) {
			def_muhenkan = mode_key (mode_val[i].ivalue);
			break;
		}
	}
}

void set_muedit(struct wordent word[])
{
	int	i;
	wchar16_t	*p;

	p = word[1].word_str;
	for (i = 0 ; i < 5 ; i ++) {
		if (match (p, mode_val[i].keyword)) {
			def_muedit = mode_key (mode_val[i].ivalue);
			break;
		}
	}
}

void set_m_toggle(struct wordent word[])
{
	if (match (word[1].word_str, WCOn))
		muhenkan_toggle = 1;
	if (match (word[1].word_str, WCOff))
		muhenkan_toggle = 0;
}

void set_silent(struct wordent word[])
{
	vflag = 0;
}

void set_bstudy(struct wordent word[])
{
	if (match (word[1].word_str, WCOn))
		bstudy = 1;
	if (match (word[1].word_str, WCOff))
		bstudy = 0;
}

int is_bstudy(void)
{
	return (bstudy);
}

int eval_muhenkan(void)
{
	Conversion	*cv;

	cv = GetConversion ();
	if (muhenkan_toggle && (KEY_ZHIRA != mode_key (cv->Imode)))
		return (KEY_ZHIRA);
	else
		return (def_muhenkan);
}

int value_muhenkan(void)
{
	return (def_muedit);
}

void set_guide(struct wordent word[])
{
	int	i;

	if (*word[2].word_str == '\0') {
		return;
	}
	else if (match (word[1].word_str, WCSj2)) {
		wscpy (guide_sj2, word[2].word_str);
		WCSJrun = guide_sj2;
	}
	else if (match (word[1].word_str, WCBuffer)) {
		wscpy (guide_buffer, word[2].word_str);
		WCMode_buff = guide_buffer;
	}
	else if (match (word[1].word_str, WCUnbuffer)) {
		wscpy (guide_unbuffer, word[2].word_str);
		WCMode_unbuff = guide_unbuffer;
	}
	else if (match (word[1].word_str, WCHira)) {
		wscpy (guide_hira, word[2].word_str);
		WCMode_Zhira = guide_hira;
	}
	else if (match (word[1].word_str, WCZkata)) {
		wscpy (guide_zkata, word[2].word_str);
		WCMode_Zkata = guide_zkata;
	}
	else if (match (word[1].word_str, WCHkata)) {
		wscpy (guide_hkata, word[2].word_str);
		WCMode_Hkata = guide_hkata;
	}
	else if (match (word[1].word_str, WCHalpha)) {
		wscpy (guide_halpha, word[2].word_str);
		WCMode_Halpha = guide_halpha;
	}
	else if (match (word[1].word_str, WCZalpha)) {
		wscpy (guide_zalpha, word[2].word_str);
		WCMode_Zalpha = guide_zalpha;
	}
	else if (match (word[1].word_str, WCShift)) {
		wscpy (guide_shift, word[2].word_str);
		WCGCshift = guide_shift;
	}
	else if (match (word[1].word_str, WCJis)) {
		wscpy (guide_jis, word[2].word_str);
		WCGCjis = guide_jis;
	}
	else if (match (word[1].word_str, WCJis2)) {
		wscpy (guide_jis2, word[2].word_str);
		WCGCjis2 = guide_jis2;
	}
	else if (match (word[1].word_str, WCKuten)) {
		wscpy (guide_kuten, word[2].word_str);
		WCGCkuten = guide_kuten;
	}
	else if (match (word[1].word_str, WCKuten2)) {
		wscpy (guide_kuten2, word[2].word_str);
		WCGCkuten2 = guide_kuten2;
	}
	else if (match (word[1].word_str, WCEuc)) {
		wscpy (guide_euc, word[2].word_str);
		WCGCeuc = guide_euc;
	}
	else if (match (word[1].word_str, WCEuc2)) {
		wscpy (guide_euc2, word[2].word_str);
		WCGCeuc2 = guide_euc2;
	}
	else if (match (word[1].word_str, WCEtc)) {
		wscpy (guide_etc, word[2].word_str);
		WCGEtc = guide_etc;
	}
	else if (match (word[1].word_str, WCFuncetc)) {
		i = 2;
		*guide_etcfunc = '\0';
		while (*word[i].word_str != '\0') {
			if (i == 2)
				wscat (guide_etcfunc, WCSpace2);
			else
				wscat (guide_etcfunc, WCSpace);
			wscat (guide_etcfunc, word[i].word_str);
			i ++;
		}
		WCGetc = guide_etcfunc;
	}
	else if (match (word[1].word_str, WCFunction)) {
		i = 2;
		*guide_function = '\0';
		while (*word[i].word_str != '\0') {
			if (i == 2)
				wscat (guide_function, WCSpace2);
			else
				wscat (guide_function, WCSpace);
			wscat (guide_function, word[i].word_str);
			i ++;
		}
		WCGmode = guide_function;
	}
}

void set_flush_conversion(struct wordent word[])
{
	if (match (word[1].word_str, WCOn))
		flush_conversion = 1;
	if (match (word[1].word_str, WCOff))
		flush_conversion = 0;
}

void set_server(struct wordent word[])
{
        int i, j, size;
        char *p;
        char *sbuf;
        extern char *cur_serv, **serv_list;
	char host[LONGLENGTH];
   
        if (cur_serv != NULL && *cur_serv != '\0')
                return;
        size = 0; 
        i = 0;
        while (word[i + 1].word_str[0] != '\0') {
                if (i >= MAXSERVER)
                        break;
                size += wslen(word[++i].word_str);  
                size++;
        }
        if ((sbuf = malloc(size)) == NULL) {
                fprintf (stderr, "set_server: can't alloc memory\n\r");
                done3();
        }
        p = sbuf;
        for (j = 0; j < i; j++) {
                serv_list[j] = p;
		(void) wcstombs(host, word[j+1].word_str, LONGLENGTH);
                strcpy(p, host);
                size = strlen(p) + 1;
                p += size;
        }
        cur_serv = serv_list[0];
}

#define DICT_SUFFIX ".dic"

void set_dict(struct wordent word[])
{
        int i, j, size, suffix_len;
        char *p;
        char *sbuf;
	char dict[LONGLENGTH];

	suffix_len = strlen(DICT_SUFFIX);
        size = 0; 
        i = 0;
        while (word[i + 1].word_str[0] != '\0') {
                size += wslen(word[++i].word_str) + suffix_len;
                size++;
        }
        if ((sbuf = malloc(size)) == NULL) {
                fprintf (stderr, "set_dict: can't alloc memory\n\r");
                done3();
        }
	if ((dict_list = (char **)malloc(sizeof(char *) * i)) == NULL) {
		free(sbuf);
                fprintf (stderr, "set_dict: can't alloc memory\n\r");
                done3();
        }
	dict_num = i;
        p = sbuf;
        for (j = 0; j < i; j++) {
                dict_list[j] = p;
		(void) wcstombs(dict, word[j+1].word_str, LONGLENGTH);
                strcpy(p, dict);
		strcat(p, DICT_SUFFIX);
                size = strlen(p) + 1;
                p += size;
        }

}
