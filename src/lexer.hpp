#ifndef LEXER_HPP
#define LEXER_HPP

#include <DiTL/hash/table.hpp>

enum { NUMBER = 256, ID = 257, TRUE = 258, FALSE = 259 };

class Token {
protected:
    const int tag;
public:
    Token(int t) : tag(t) {}
    virtual ~Token() {}
    int GetTag() const { return tag; }
};

class Number : public Token {
    const int value;
public:
    Number(int v) : Token(NUMBER), value(v) {}
    virtual ~Number() {}
    int GetValue() const { return value; }
};

class Word : public Token {
    const char *const lexeme;
    bool owner;
public:
    Word(int t, const char *l, bool own = true)
        : Token(t), lexeme(l), owner(own) {}
    Word(const Word &w) : Token(w), lexeme(w.lexeme), owner(false) {}
    virtual ~Word() { if (owner) delete[] lexeme; }
    const char *GetLexeme() const { return lexeme; }
private:
    void operator=(const Word &);
};

template <class Key>
class Pair<Key, Word*> {
    typedef Word *Value;
    Key key;
    Value value; /* is the owner */
public:
    Pair(const Key &k, const Value &v) : key(k), value(v) {}
    ~Pair() { delete value; }
    const Key &GetKey() const { return key; }
    const Value &GetValue() const { return value; }
    void SetKey(const Key &k) { key = k; }
    void SetValue(const Value &v) { delete value; value = v; }
};

#endif
