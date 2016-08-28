
/* prolog.c: a simple Prolog interpreter written in C++,                */
/*           including an example test run as main().                   */
/* Copyright (c) Alan Mycroft, University of Cambridge, 2000.           */
/* http://www.cl.cam.ac.uk/~am21/research/funnel/prolog.c               */


#include "std_lib_facilities.h"

#include "prolog.h"

void indent(int n)
{
    for (int i = 0; i<n; i++) cout << "    ";
}



int TermVar::timestamp = 0;

Trail *Trail::sofar = NULL;

bool TermVar::unify(Term *t)
{
    if (instance!=this) return instance->unify(t);
    Trail::Push(this);
    instance = t;
    return true;
}
Term *TermVar::copy()
{
    if (instance==this) {
        Trail::Push(this);
        instance = new TermVar();
    }
    return instance;
}

void Goal::solve(Program *p, int level, TermVarMapping *map)
{
    indent(level);
    cout << "solve@"  << level << ": ";
    this->print();
    cout << "\n";
    for (Program *q = p; q != NULL; q = q->pcdr) {
        Trail *t = Trail::Note();
        Clause *c = q->pcar->copy();
        Trail::Undo(t);
        indent(level);
        cout << "  try:";
        c->print();
        cout << "\n";
        if (car->unify(c->head)) {
            Goal *gdash = c->body==NULL ? cdr : c->body->append(cdr);
            if (gdash == NULL) map->showanswer();
            else gdash->solve(p, level+1, map);
        } else {
            indent(level);
            cout << "  nomatch.\n";
        }
        Trail::Undo(t);
    }
}

