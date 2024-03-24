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
 * $SonyRCSfile: sjgetchar.c,v $  
 * $SonyRevision: 1.5 $ 
 * $SonyDate: 1997/01/23 11:26:31 $
 */



#include "wchar16.h"
#include "kctype.h"
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>
#include "sj2.h"
#include "select.h"
#include "sj3.h"
#include "utf8.h"

#if !defined(FREAD) && defined(O_RDONLY)
#define		FREAD		(O_RDONLY+1)
#define		FWRITE		(O_WRONLY+1)
#endif

static wchar16_t  backup = (wchar16_t) 0;

extern int	current_locale;
extern int	utf8_convert;
extern int	master;

wchar16_t SJ_getchar(void)
{
	SELECT_FD	ifds;
	wchar16_t		c;

	if (backup) {
		if (SJ_read (&c, 1) > 0)
			return (c);
	}
	for (;;) {
		FD_ZERO (&ifds);
		FD_SET (STDIN, &ifds);
		FD_SET (master, &ifds);
		if (select (master + 1, &ifds, 0, 0, 0) <= 0) {
			if (errno == EINTR) {
				errno = 0;
				continue;
			} else
				return (EOF);
		}

		if (FD_ISSET (STDIN, &ifds)) {
			if (SJ_read (&c, 1) > 0) {
				if (FD_ISSET (master, &ifds)) {
					output_master();
				}
				return (c);
			}
			return (EOF);
		}
		if (FD_ISSET (master, &ifds)) {
			output_master();
		}
	}
}

void output_master(void)
{
	int	n, m;
	char		outbuf[BUFSIZ];
	int		f;
	struct termios	tc, tc1;
	extern int	master;

	if ((n = read (master, outbuf, BUFSIZ)) > 0) {
		m = outbuf[0];
		if (m == TIOCPKT_DATA) {
		
			write_stdout((unsigned char *)&outbuf[1], n - 1);
		
			fflush (stdout);
			return;
		}
		else if (m & (TIOCPKT_FLUSHREAD | TIOCPKT_FLUSHWRITE)) {
			switch (m & (TIOCPKT_FLUSHREAD | TIOCPKT_FLUSHWRITE)) {
			case TIOCPKT_FLUSHREAD:
				f = TCIFLUSH;
				break;
			case TIOCPKT_FLUSHWRITE:
				f = TCOFLUSH;
				break;
			default:
				f = TCIOFLUSH;
				break;
			}
			fflush (stdout);
			tcflush (STDOUT, f);
		}
		else if (m & TIOCPKT_NOSTOP) {
			tcgetattr(1, &tc);
			tc.c_cc[VSTOP] = 0;
			tc.c_cc[VSTART] = 0;
			tcsetattr(STDOUT, TCSANOW, &tc);
		}
		else if (m & TIOCPKT_DOSTOP) {
			tcgetattr(master, &tc1);
			tcgetattr(STDOUT, &tc);
			tc.c_cc[VSTOP] = tc1.c_cc[VSTOP];
			tc.c_cc[VSTART] = tc1.c_cc[VSTART];
			tcsetattr(STDOUT, TCSANOW, &tc);
		}
	}
}

int SJ_write(wchar16_t *s, int n)
{
	int	val;
	wchar16_t wb[BUFFLENGTH];
	unsigned char	buff[BUFFLENGTH * 4];

	wsncpy(wb, s, n);
	wb[n] = 0;
	val = utf8_convert ?
		wcstoutf8((char *) buff, wb, (size_t) n * 4) :
		wcstombs((char *) buff, wb, (size_t) n * 3);
	if ((val == 0) && n) {
		int i;
		val = n;
		for (i = 0; i <= n; i++)
		  buff[i] = 0;
	}
	if (val > 0) 
	  val = write(master, buff, val);
	return(val);
}

static int mbsize(unsigned char *c)
{
	if (utf8_convert) {
		return utf8toucs_char(c, 0, NULL);
	} else if (current_locale == LC_CTYPE_EUC) {
		if (iseuc3byte(*c)) return 3;
		else if (iseuc2byte(*c)) return 2;
		else return 1;
	} else {
		if (issjis1(*c)) return 2;
		else return 1;
	}
}

static int mblength(unsigned char *s, int n, int *r)
{
	int i, p, q;

	for (i = 0; i < n; i += p) {
		p = mbsize(s + i);
		q = n - i;
		if (p > q) break;
	}

	if (r != NULL) *r = i;
	return p - q;
}

void write_stdout(unsigned char *s, int n)
{
	static int remain = 0, len = 0;
	static unsigned char buff[8];
	int rsz;

	if (remain) {
		memcpy(buff + len, s, rsz = (remain < n) ? remain : n);
		remain -= rsz;
		len += rsz;
		if (!remain) fwrite(buff, len, 1, stdout);
		s += rsz;
		n -= rsz;
	}

	if (n <= 0) return;
	remain = mblength(s, n, &rsz);
	fwrite(s, rsz, 1, stdout);

	if (remain) memcpy(buff, s + rsz, len = n - rsz);
}


int SJ_read(wchar16_t *s, int n)
{
	unsigned char buff[BUFFLENGTH];
	wchar16_t wcbuff[BUFFLENGTH];
	int i, count, remain;
	size_t wnum;
	extern int current_locale;

	if (backup) {
		*s = backup;
		backup = 0;
		return (1);
	}
	if ((count = read(STDIN, buff, n)) <= 0)
	        return (count);
	remain = mblength(buff, count, NULL);
	if (remain) {
		if ((i = read(STDIN, &buff[count], remain)) > 0)
		        count += i;
	}
	buff[count] = 0;
	wnum = utf8_convert ?
		utf8towcs((wchar16_t *) wcbuff, (char *) buff, count) :
		mbstowcs((wchar16_t *) wcbuff, (char *) buff, count);
	if ((wnum == 0) && count) {
		wnum = count;
		for ( i = 0; i <= count; i++)
		        wcbuff[i] = (wchar16_t) 0;
	}
	if (wnum > n) {
		backup = wcbuff[--wnum];
	}
        memcpy((void *) s, (void *) wcbuff, wnum * sizeof(wchar16_t));     
	return(wnum);
}

void SJ_warnning(wchar16_t *s)
{
	SJ_print (s);
}

void SJ_print(wchar16_t *s)
{
	unsigned char buff[BUFFLENGTH * 4];
	size_t num;

	num = utf8_convert ?
		wcstoutf8((char *) buff, s, (size_t ) (BUFFLENGTH * 4)) :
		wcstombs((char *) buff, s, (size_t ) (BUFFLENGTH * 3));
	if ((int) num > 0) 
	  printf("%s", buff);
}

void SJ_through(wchar16_t *s, int n)
{
	SJ_write (s, n);
}

