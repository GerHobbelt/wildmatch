/*
 * Copyright (c), 2016 David Aguilar
 * Based on the fnmatch implementation from OpenBSD.
 *
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)fnmatch.h	8.1 (Berkeley) 6/2/93
 *	$OpenBSD: fnmatch.h,v 1.4 1997/09/22 05:25:32 millert Exp $
 */
#ifndef _WILDMATCH_H_
#define _WILDMATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WM_MATCH 0 /* Match. */
#define WM_NOMATCH 1 /* Match failed. */
/* Note: any other non-zero return value indicates an error result. */

#define WM_NOESCAPE 0x01 /* Disable backslash escaping. */
#define WM_PATHNAME 0x02 /* Slash must be matched by slash. */
#define WM_PERIOD 0x04 /* Period must be matched by leading period. */
#define WM_LEADING_DIR 0x08 /* Ignore /<tail> after Imatch. */
#define WM_CASEFOLD 0x10 /* Case insensitive search. */
#define WM_PREFIX_DIRS 0x20 /* Unused */
#define WM_WILDSTAR 0x40 /* Double-asterisks ** matches slash too. */
#define WM_KSH_BRACKETS 0x80 /* Accept KSH style (x|y|z) and [+?@!](a|b|c) expressions. */
#define WM_BRACES 0x100 /* Accept {x,y,z} expressions. */
#define WM_NEGATION 0x200 /* !-prefixed wildcard expressions invert the result, a la gitignore. Extended: also applies to {} and {} subexpressions */
#define WM_ALT_SUBEXPR_SEPARATOR 0x400 /* Accept alternative () and {} subexpression separators: accept any in the set , | ; : */
#define WM_TILDE 0x800 /* Accept ~/ and ~user/ paths, using the provided mapper callback function. Default mapping expands to the symtem's user/home paths. */
#define WM_EXPAND_VARS 0x1000 /* Expand any $x$ and %x% variables, using the provided mapper callback function. Default mapping expands any environment variables. */
#define WM_CLASSES 0x2000 /* Accept named character class [:class:], equivalence class [=c=] and collating cymbol class [.c.] elements in any [] set expression. */
#define WM_UCS2 0x4000 /* Accept NTFS/Windows UCS2 characters which are legal but are also broken UTF16 higher plane encodes. */
#define WM_UNICODE 0x8000 /* Accept/assume the locale to be the full range UTF8/Unicode codepoint space. Requires the availability of the ICU library. */
#define WM_EXPAND_FALLBACK_TO_DEFAULT 0x10000 /* When the userland mapper for the WM_EXPAND_VARS or WM_TILDE flags did not resolve the particle, the task is forwarded to the default mapper. This allows userland programmers to code mappers which only serve to 'tweak' desired behaviours otherwise provided by the default mapper, thus reducing code duplication across libraries/applications and effect bugs due to disparity. */

#define WM_IGNORECASE WM_CASEFOLD
#define WM_FILE_NAME WM_PATHNAME
#define WM_EXTMATCH WM_KSH_BRACKETS /* FNM_EXTMATCH: GNU fnmatch extended patterns */

/*
 * wildmatch is an extension of function fnmatch(3) as specified in
 * POSIX 1003.2-1992, section B.6.
 *
 * Compares a filename or pathname to a pattern.
 *
 * wildmatch is fnmatch-compatible by default.  Its new features are enabled
 * by passing WM_WILDSTAR in flags, which makes ** match across path
 * boundaries.  WM_WILDSTAR implies WM_PATHNAME and WM_PERIOD.
 *
 * The WM_ flags are named the same as their FNM_ fnmatch counterparts
 * and are compatible in behavior to fnmatch(3) in the absence of WM_WILDSTAR.
 */

int wildmatch(const char *pattern, const char *string, int flags);

/*
 * The type of the user-provided mapper callback function.
 *
 * The callback is supposed to return WM_MATCH when the `param` has been expanded (a.k.a. 'resolved') by it,
 * WM_NOMATCH when no expansion/resolution has occurred, or a non-zero, negative integer error return value
 * using the `-1 * errno` error codes.
 */
typedef int wm_mapper_callback_t(char *dst, size_t dstsize, const char *param, int flags);

/*
 * The 'extended' version of wildmatch() above, which includes support for the WM_EXPAND_VARS and WM_TILDE flags
 * using a userland-provided mapper/resolver function.
 *
 * Note: when the WM_EXPAND_VARS or WM_TILDE flags are passed to the `wildmatch()` API above, a default mapper/resolver
 * is used. `wildmatch_ex()` enables userland code to specify a customized mapper instead.
 */
int wildmatch_ex(const char *pattern, const char *string, int flags, wm_mapper_callback_t mapper);

/*
 * Normalize your wildcard expression to use the default subexpression seaprators, etc.;
 * also resolves duplicate subexpressions such as **\**\ and reduces a\.\b and a\c\..\b to a\b
 * and cleans up set expressions which contain implicit or explict / slashes while WM_PATHNAME is used.
 */
int normalize_wildmatch(char *dst, size_t dstsize, const char *pattern, int flags);

/*
 * Translates your wildcard expression to an equivalent regular expression.
 */
int translate_wildmatch(char *dst, size_t dstsize, const char *pattern, int flags);

#ifdef __cplusplus
}
#endif
#endif
