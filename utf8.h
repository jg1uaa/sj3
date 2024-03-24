// SPDX-License-Identifier: MIT

#include "wchar16.h"

int utf8toucs_char(unsigned char *s, int len, int *ucs);
int ucstoutf8_char(unsigned char *s, int len, int ucs);
int utf8towcs(wchar16_t *ws, unsigned char *mb, int n);
int wcstoutf8(unsigned char *mb, wchar16_t *ws, int n);
