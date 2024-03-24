// SPDX-License-Identifier: MIT

#include <string.h>
#include "utf8.h"

extern const unsigned short ucs2tow16[65536];
extern const unsigned short w16toucs2[65536];

int utf8toucs_char(unsigned char *s, int len, int *ucs)
{
	int i, m, n, l, h, u = -1;

	if (*s >= 0x00 && *s <= 0x7f) {
		m = 0x7f; n = 1; l = 0x00000000; h = 0x0000007f;
	} else if (*s >= 0xc0 && *s <= 0xdf) {
		m = 0x1f; n = 2; l = 0x00000080; h = 0x000007ff;
	} else if (*s >= 0xe0 && *s <= 0xef) {
		m = 0x0f; n = 3; l = 0x00000800; h = 0x0000ffff;
	} else if (*s >= 0xf0 && *s <= 0xf7) {
		m = 0x07; n = 4; l = 0x00010000; h = 0x001fffff;
	} else if (*s >= 0xf8 && *s <= 0xfb) {
		m = 0x03; n = 5; l = 0x00200000; h = 0x03ffffff;
	} else if (*s >= 0xfc && *s <= 0xfd) {
		m = 0x01; n = 6; l = 0x04000000; h = 0x7fffffff;
	} else {
		m = 0x00; n = 1; l = 0x00000001; h = 0x00000001;
	}

	if (ucs == NULL) return n;
	if (len < n) {
		n = len;
		goto fin;
	}

	u = *s++ & m;
	for (i = 1; i < n; i++) {
		u <<= 6;
		if ((*s & 0xc0) != 0x80) goto fin;
		u |= *s++ & 0x3f;
	}
	u = (u >= l && u <= h) ? u : -1;
fin:
	*ucs = u;
	return n;
}

int ucstoutf8_char(unsigned char *s, int len, int ucs)
{
	int i, m, n, v;

	if (ucs >= 0x00000000 && ucs <= 0x0000007f) {
		m = 0x7f; n = 1; v = 0x00;
	} else if (ucs >= 0x00000080 && ucs <= 0x000007ff) {
		m = 0x1f; n = 2; v = 0xc0;
	} else if (ucs >= 0x00000800 && ucs <= 0x0000ffff) {
		m = 0x0f; n = 3; v = 0xe0;
	} else if (ucs >= 0x00010000 && ucs <= 0x001fffff) {
		m = 0x07; n = 4; v = 0xf0;
	} else if (ucs >= 0x00200000 && ucs <= 0x03ffffff) {
		m = 0x03; n = 5; v = 0xf8;
	} else if (ucs >= 0x04000000 && ucs <= 0x7fffffff) {
		m = 0x01; n = 6; v = 0xfc;
	}

	if (s == NULL) goto fin;
	if (len < n) return 0;

	*s++ = v | ((ucs >> ((n - 1) * 6)) & m);
	for (i = 1; i < n; i++)
		*s++ = 0x80 | ((ucs >> ((n - i - 1) * 6)) & 0x3f);
fin:
	return n;
}

int utf8towcs(wchar16_t *ws, unsigned char *mb, int n)
{
#define REPLACEMENT_EUC	0xa2ae

	int i, r, ucs, sz;
	wchar16_t w;

	for (i = 0, r = strlen(mb); (i < n) && (r > 0); i++, r -= sz) {
		sz = utf8toucs_char(mb, r, &ucs);
		w = ((ucs >= 0) &&
		     (ucs < (sizeof(ucs2tow16) / sizeof(unsigned short)))) ?
			ucs2tow16[ucs] : 0;
		if (!w) w = REPLACEMENT_EUC;
		*ws++ = w;
		mb += sz;
	}
	if (!*mb) *ws = 0;

	return i;
}

int wcstoutf8(unsigned char *mb, wchar16_t *ws, int n)
{
#define REPLACEMENT_UCS	0xfffd

	int i, r, ucs, sz;

	for (i = 0, r = n; *ws && (i < n); i += sz, r -= sz) {
		ucs = w16toucs2[*ws++];
		if ((sz = ucs ? ucstoutf8_char(mb, r, ucs) : 0)) {
			mb += sz;
			continue;
		}
		if (!(sz = ucstoutf8_char(mb, r, REPLACEMENT_UCS)))
			break;
	}
	if (!*ws) *mb = '\0';

	return i;
}
