#ifndef LEXER_HPP
#define LEXER_HPP

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
    virtual ~Word() { if (owner) delete[] lexeme; }
    const char *GetLexeme() const { return lexeme; }
};

#endif
