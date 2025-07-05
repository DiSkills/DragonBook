#ifndef LEXER_HPP
#define LEXER_HPP

#include <string.h>
#include <DiTL/hash/table.hpp>

#include "buffer.hpp"

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

class Lexer {
    class Lexeme {
        const char * const str; /* isn't the owner */
    public:
        Lexeme(const char *s) : str(s) {}
        unsigned int Hash() const;
        bool operator==(const Lexeme &l) const
            { return 0 == strcmp(str, l.str); }
    };
    unsigned int line;
    int peek;
    HashTable<Lexeme, Word*> words;
    Buffer lexeme_buffer;
public:
    Lexer();
    const Token *Scan();
    unsigned int GetLine() const { return line; }
private:
    void Reserve(Word *w) { words.Put(w->GetLexeme(), w); }

    void SkipSpaces();
    void SkipSingleLineComment();
    void SkipComment();
    const Token *SkipSpacesAndComments();
    const Number *ScanNumber();
    const Word *ScanLexeme();

    static bool IsSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
    static bool IsDigit(char c) { return '0' <= c && c <= '9'; }
    static bool IsLetter(char c)
        { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }
private:
    Lexer(const Lexer &);
    void operator=(const Lexer &);
};

#endif
