/*
Copyright (c) 2016, Blue Brain Project
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "nmodlconf.h"

#include "modl.h"
#include "parse1.h"
#include "symbol.h"

List	*symlist[128];	/* symbol table: linked list
				first char gives which list to use,but*/

List *symlistlist;	 /* for a stack of local symbol lists */

void symbol_init() {
	int i;
	symlistlist = newlist();
	for (i=0; i<128; i++) {	/* more than we need */
		symlist[i] = newlist();
	}
}

Symbol *lookup(s)	/* find s in symbol table */
	char *s;
{
	Item *sp;

	ITERATE(sp, symlist[s[0]]) {
		if (strcmp(SYM(sp)->name, s) == 0) {
			return SYM(sp);
		}
	}
	return SYM0;	/* 0 ==> not found */
}

Symbol *checklocal(sym)
	Symbol *sym;
{
	Item *sp;
	List *sl;
	char *s;

	s = sym->name;
	/* look in local lists */
	ITERATE(sl, symlistlist)
	ITERATE(sp, LST(sl)) {
		if (strcmp(SYM(sp)->name+2, s) == 0) { /*get past _l*/
			return SYM(sp);
		}
	}
	return sym;
}
	
Symbol *install(s, t)	/* install s in the list symbol table with type t*/
	char *s;
	int t;
{
	Symbol *sp;
	List *sl;

	if (t == STRING) {
		sl = symlist[0];
	}else if (t == -1) {	/*install on top local list see below*/
		t = NAME;
		assert(symlistlist->next != symlistlist);
		sl = LST(symlistlist->next);
	}else{
		sl = symlist[s[0]];
	}
	sp = (Symbol *) emalloc(sizeof(Symbol));
	sp->name = stralloc(s, (char *)0);
	sp->type = t;
	sp->subtype = 0;
#if NMODL
	sp->nrntype = 0;
	sp->assigned_to_ = 0;
	sp->no_threadargs = 0;
#if CVODE
	sp->slist_info_ = (int*)0;
#endif
#endif
	sp->u.str = (char *)0;
	sp->used = 0;
	sp->usage = 0;
	sp->araydim = 0;
	sp->discdim = 0;
	sp->level = 100;	/* larger than any reasonable submodel level */
	Linsertsym(sl, sp); /*insert at head of list*/
	return sp;
}

void pushlocal()
{
	Item * q;
	q = linsertsym(symlistlist, SYM0); /*the type is irrelevant*/
	LST(q) = newlist();
}

void poplocal() /* a lot of storage leakage here for symbols we are guaranteed
	not to need */
{
	List *sl;
	Item *i, *j;

	assert(symlistlist->next != symlistlist);
	sl = LST(symlistlist->next);
	for (i = sl->next; i != sl; i = j) {
		j = i->next;
		delete(i);
	}
	delete(symlistlist->next);
}

Symbol *copylocal(s)
	Symbol *s;
{
	if (s->name[0] == '_') {
		Sprintf(buf, "%s", s->name);
	}else{
		Sprintf(buf, "_l%s", s->name);
	}
	return install(buf, -1);
}

