// SciTE - Scintilla based Text Editor
// LexPython.cxx - lexer for Python
// Copyright 1998-2000 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include <stdio.h> 
#include <stdarg.h> 

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"

static void classifyWordPy(unsigned int start, unsigned int end, WordList &keywords, StylingContext &styler, char *prevWord) {
	char s[100];
	bool wordIsNumber = isdigit(styler[start]);
	for (unsigned int i = 0; i < end - start + 1 && i < 30; i++) {
		s[i] = styler[start + i];
		s[i + 1] = '\0';
	}
	char chAttr = SCE_P_IDENTIFIER;
	if (0 == strcmp(prevWord, "class"))
		chAttr = SCE_P_CLASSNAME;
	else if (0 == strcmp(prevWord, "def"))
		chAttr = SCE_P_DEFNAME;
	else if (wordIsNumber)
		chAttr = SCE_P_NUMBER;
	else if (keywords.InList(s))
		chAttr = SCE_P_WORD;
	styler.ColourTo(end, chAttr);
	strcpy(prevWord, s);
}

static bool IsPyComment(StylingContext &styler, int pos, int len) {
	return len>0 && styler[pos]=='#';
}

static void ColourisePyDoc(unsigned int startPos, int length, int initStyle, 
						   WordList *keywordlists[], StylingContext &styler) {

	// Python uses a different mask because bad indentation is marked by oring with 32
	styler.StartAt(startPos, 127);
	
	WordList &keywords = *keywordlists[0];
	
	//Platform::DebugPrintf("Python coloured\n");
	bool fold = styler.GetPropSet().GetInt("fold");
	int whingeLevel = styler.GetPropSet().GetInt("tab.timmy.whinge.level");
	char prevWord[200];
	prevWord[0] = '\0';
	if (length == 0)
		return ;
	int lineCurrent = styler.GetLine(startPos);
	int spaceFlags = 0;
	// TODO: Need to check previous line for indentation for both folding and bad indentation
	int indentCurrent = styler.IndentAmount(lineCurrent, &spaceFlags, IsPyComment);

	int state = initStyle & 31;
	char chPrev = ' ';
	char chPrev2 = ' ';
	char chNext = styler[startPos];
	char chNext2 = styler[startPos];
	styler.StartSegment(startPos);
	int lengthDoc = startPos + length;
	bool atStartLine = true;
	for (int i = startPos; i <= lengthDoc; i++) {
	
		if (atStartLine) {
			if (whingeLevel == 1) {
				styler.SetFlags((spaceFlags & wsInconsistent) ? 64 : 0, state);
			} else if (whingeLevel == 2) {
				styler.SetFlags((spaceFlags & wsSpaceTab) ? 64 : 0, state);
			} else if (whingeLevel == 3) {
				styler.SetFlags((spaceFlags & wsSpace) ? 64 : 0, state);
			} else if (whingeLevel == 4) {
				styler.SetFlags((spaceFlags & wsTab) ? 64 : 0, state);
			}
			atStartLine = false;
		}
		
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		chNext2 = styler.SafeGetCharAt(i + 2);
		
		if ((ch == '\r' && chNext != '\n') || (ch == '\n') || (i == lengthDoc)) {
			if ((state == SCE_P_DEFAULT) || (state == SCE_P_TRIPLE) || (state == SCE_P_TRIPLEDOUBLE)) {
				// Perform colourisation of white space and triple quoted strings at end of each line to allow
				// tab marking to work inside white space and triple quoted strings
				styler.ColourTo(i, state);
			}

			int lev = indentCurrent;
			int indentNext = styler.IndentAmount(lineCurrent + 1, &spaceFlags, IsPyComment);
			if (!(indentCurrent & SC_FOLDLEVELWHITEFLAG)) {
				// Only non whitespace lines can be headers
				if ((indentCurrent & SC_FOLDLEVELNUMBERMASK) < (indentNext & SC_FOLDLEVELNUMBERMASK)) {
					lev |= SC_FOLDLEVELHEADERFLAG;
				} else if (indentNext & SC_FOLDLEVELWHITEFLAG) {
					// Line after is blank so check the next - maybe should continue further?
					int spaceFlags2 = 0;
					int indentNext2 = styler.IndentAmount(lineCurrent + 2, &spaceFlags2, IsPyComment);
					if ((indentCurrent & SC_FOLDLEVELNUMBERMASK) < (indentNext2 & SC_FOLDLEVELNUMBERMASK)) {
						lev |= SC_FOLDLEVELHEADERFLAG;
					}
				}
			}
			indentCurrent = indentNext;
			if (fold) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			atStartLine = true;
		}

		if (styler.IsLeadByte(ch)) {
			chNext = styler.SafeGetCharAt(i + 2);
			chPrev = ' ';
			chPrev2 = ' ';
			i += 1;
			continue;
		}

		if (state == SCE_P_STRINGEOL) {
			if (ch != '\r' && ch != '\n') {
				styler.ColourTo(i - 1, state);
				state = SCE_P_DEFAULT;
			}
		}
		if (state == SCE_P_DEFAULT) {
			if (iswordstart(ch)) {
				styler.ColourTo(i - 1, state);
				state = SCE_P_WORD;
			} else if (ch == '#') {
				styler.ColourTo(i - 1, state);
				state = chNext == '#' ? SCE_P_COMMENTBLOCK : SCE_P_COMMENTLINE;
			} else if (ch == '\"') {
				styler.ColourTo(i - 1, state);
				if (chNext == '\"' && chNext2 == '\"') {
					i += 2;
					state = SCE_P_TRIPLEDOUBLE;
					ch = ' ';
					chPrev = ' ';
					chNext = styler.SafeGetCharAt(i + 1);
				} else {
					state = SCE_P_STRING;
				}
			} else if (ch == '\'') {
				styler.ColourTo(i - 1, state);
				if (chNext == '\'' && chNext2 == '\'') {
					i += 2;
					state = SCE_P_TRIPLE;
					ch = ' ';
					chPrev = ' ';
					chNext = styler.SafeGetCharAt(i + 1);
				} else {
					state = SCE_P_CHARACTER;
				}
			} else if (isoperator(ch)) {
				styler.ColourTo(i - 1, state);
				styler.ColourTo(i, SCE_P_OPERATOR);
			}
		} else if (state == SCE_P_WORD) {
			if (!iswordchar(ch)) {
				classifyWordPy(styler.GetStartSegment(), i - 1, keywords, styler, prevWord);
				state = SCE_P_DEFAULT;
				if (ch == '#') {
					state = chNext == '#' ? SCE_P_COMMENTBLOCK : SCE_P_COMMENTLINE;
				} else if (ch == '\"') {
					if (chNext == '\"' && chNext2 == '\"') {
						i += 2;
						state = SCE_P_TRIPLEDOUBLE;
						ch = ' ';
						chPrev = ' ';
						chNext = styler.SafeGetCharAt(i + 1);
					} else {
						state = SCE_P_STRING;
					}
				} else if (ch == '\'') {
					if (chNext == '\'' && chNext2 == '\'') {
						i += 2;
						state = SCE_P_TRIPLE;
						ch = ' ';
						chPrev = ' ';
						chNext = styler.SafeGetCharAt(i + 1);
					} else {
						state = SCE_P_CHARACTER;
					}
				} else if (isoperator(ch)) {
					styler.ColourTo(i, SCE_P_OPERATOR);
				}
			}
		} else {
			if (state == SCE_P_COMMENTLINE || state == SCE_P_COMMENTBLOCK) {
				if (ch == '\r' || ch == '\n') {
					styler.ColourTo(i - 1, state);
					state = SCE_P_DEFAULT;
				}
			} else if (state == SCE_P_STRING) {
				if ((ch == '\r' || ch == '\n') && (chPrev != '\\')) {
					styler.ColourTo(i - 1, state);
					state = SCE_P_STRINGEOL;
				} else if (ch == '\\') {
					if (chNext == '\"' || chNext == '\'' || chNext == '\\') {
						i++;
						ch = chNext;
						chNext = styler.SafeGetCharAt(i + 1);
					}
				} else if (ch == '\"') {
					styler.ColourTo(i, state);
					state = SCE_P_DEFAULT;
				}
			} else if (state == SCE_P_CHARACTER) {
				if ((ch == '\r' || ch == '\n') && (chPrev != '\\')) {
					styler.ColourTo(i - 1, state);
					state = SCE_P_STRINGEOL;
				} else if (ch == '\\') {
					if (chNext == '\"' || chNext == '\'' || chNext == '\\') {
						i++;
						ch = chNext;
						chNext = styler.SafeGetCharAt(i + 1);
					}
				} else if (ch == '\'') {
					styler.ColourTo(i, state);
					state = SCE_P_DEFAULT;
				}
			} else if (state == SCE_P_TRIPLE) {
				if (ch == '\'' && chPrev == '\'' && chPrev2 == '\'') {
					styler.ColourTo(i, state);
					state = SCE_P_DEFAULT;
				}
			} else if (state == SCE_P_TRIPLEDOUBLE) {
				if (ch == '\"' && chPrev == '\"' && chPrev2 == '\"') {
					styler.ColourTo(i, state);
					state = SCE_P_DEFAULT;
				}
			}
		}
		chPrev2 = chPrev;
		chPrev = ch;
	}
	if (state == SCE_P_WORD) {
		classifyWordPy(styler.GetStartSegment(), lengthDoc, keywords, styler, prevWord);
	} else {
		styler.ColourTo(lengthDoc, state);
	}
}

static LexerModule lmPython(SCLEX_PYTHON, ColourisePyDoc);
