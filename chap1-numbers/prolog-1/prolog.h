#ifndef __PROLOG_H__
#define __PROLOG_H__


class Atom {
    char *atomname;
public: Atom(char *s) : atomname(s) {}
    void print() { cout<<atomname; }
    bool eqatom(Atom *t) { return strcmp(atomname, t->atomname) == 0; }
};

class TermCons;

class Term {
public: virtual void print() = 0;
public: virtual bool unify(Term *) = 0;
public: virtual bool unify2(TermCons *) = 0;
public: virtual Term *copy() = 0;
};


class TermCons : public Term {
private:
    int arity;
    Atom *fsym;
    Term **args;
public:
    TermCons(Atom *f) : fsym(f), arity(0), args(NULL)
            { }
    TermCons(Atom *f, Term *a1) : fsym(f), arity(1), args(new Term*[1])
            { args[0]=a1; };
    TermCons(Atom *f, Term *a1, Term *a2)
            : fsym(f), arity(2), args(new Term*[2])
            { args[0]=a1, args[1]=a2; };
    TermCons(Atom *f, Term *a1, Term *a2, Term *a3)
            : fsym(f), arity(3), args(new Term*[3])
            { args[0]=a1, args[1]=a2, args[2]=a3; };
    void print() { fsym->print();
                   if (arity>0)
                   {   cout <<"(";
                       for (int i = 0; i<arity; )
                       {   args[i]->print();
                           if (++i < arity) cout << ",";
                       }
                       cout <<")";
                   }
                 }
    bool unify(Term *t) { return t->unify2(this); }
    Term *copy() { return copy2(); }
    TermCons *copy2() { return new TermCons(this); }
private:
    TermCons(TermCons *p)
            : fsym(p->fsym), arity(p->arity),
              args(p->arity==0 ? NULL : new Term*[p->arity])
            { for (int i=0; i<arity; i++) args[i] = p->args[i]->copy(); }
    bool unify2(TermCons *t) { 
      if (!(fsym->eqatom(t->fsym) && arity == t->arity))
          return false;
      for (int i = 0; i<arity; i++)
          if (!args[i]->unify(t->args[i])) return false;
      return true;
    }
};

class TermVar : public Term {
private:
    Term *instance;
    int varno;
    static int timestamp;
public:
    TermVar() : instance(this), varno(++timestamp) {}
    void print() { if (instance!=this) instance->print();
                   else cout<<"_"<<varno; };
    bool unify(Term *t);
    Term *copy();
    Term *reset() { instance = this; }
private:
    bool unify2(TermCons *t) { return this->unify(t); }
};


class Program;
class TermVarMapping;

class Goal {
private:
    TermCons *car;
    Goal *cdr;
public:
    Goal(TermCons *h, Goal *t) : car(h), cdr(t) {}
    Goal *copy() { return new Goal(car->copy2(),
                                   cdr==NULL ? NULL : cdr->copy()); }
    Goal *append(Goal *l) { return new Goal(car,
                                   cdr==NULL ? NULL : cdr->append(l)); }
    void print() { car->print();
                   if (cdr != NULL) { cout << "; ", cdr->print(); }
                 }
    void solve(Program *p, int level, TermVarMapping *map);
};

class Clause {
public:
    TermCons *head;
    Goal *body;
    Clause(TermCons *h, Goal *t) : head(h), body(t) {}
    Clause *copy() { 
        return new Clause(head->copy2(), body==NULL ? NULL : body->copy()); 
    }
    void print() { 
        cout << "Clause ";
        head->print();
        cout << " :- ";
        if (body==NULL) cout << "true";
        else body->print();
    }
};

class Program {
public:
    Clause *pcar;
    Program *pcdr;
    Program(Clause *h, Program *t) : pcar(h), pcdr(t) {}
};

class Trail {
private:
    TermVar *tcar;
    Trail *tcdr;
    static Trail *sofar;
    Trail(TermVar *h, Trail *t) : tcar(h), tcdr(t) {}
public:
    static Trail *Note() { return sofar; }
    static void Push(TermVar *x) { sofar = new Trail(x, sofar); }
    static void Undo(Trail *whereto)
    {   for (; sofar != whereto; sofar = sofar->tcdr)
            sofar->tcar->reset();
    }
};


class TermVarMapping {
private:
    TermVar **varvar;
    char **vartext;
    int size;
public:
    TermVarMapping(TermVar *vv[], char *vt[], int vs)
        :varvar(vv), vartext(vt), size(vs) {}
    void showanswer()
    {   if (size == 0) cout << "yes\n";
        else
        {   for (int i = 0; i < size; i++)
            {   cout << vartext[i] << " = "; varvar[i]->print(); cout << "\n";
            }
        }
    }
};


#endif

