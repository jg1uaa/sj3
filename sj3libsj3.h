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
 * $SonyRCSfile: sj3libsj3.h,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1996/05/27 10:52:05 $
 */


#ifndef SJ3_SJ3LIBSJ3_H_
#define SJ3_SJ3LIBSJ3_H_

#include "wchar16.h"
#include "sj3lib.h"



typedef	struct bunsetu_sj3 {
	int		srclen;				
	int		destlen;			
	wchar16_t       *srcstr;			
	wchar16_t 	*deststr;			
	struct studyrec	dcid;				
} SJ3_BUNSETU_SJ3;

#ifdef valid
#undef valid
#endif

typedef	struct douon_sj3 {
	wchar16_t  	ddata[SJ3_BUNSETU_KANJI];	
	int		dlen;				
	SJ3_STUDYREC	dcid;				
	int             wlen;                           
 	char		valid;
} SJ3_DOUON_SJ3;


#endif /* SJ3_SH3LIBSJ3_H_ */
