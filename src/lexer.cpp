#include <stdio.h>

#include "lexer.hpp"

unsigned int Lexer::Lexeme::Hash() const
{
    const char *s = str;
    unsigned int h = 5381;
    for (; *s; s++) {
        h = h * 33 + *s;
    }
    return h;
}

Lexer::Lexer() : line(1), peek(' ')
{
    Reserve(new Word(TRUE, "true", false));
    Reserve(new Word(FALSE, "false", false));
}

void Lexer::SkipSpaces()
{
    while (IsSpace(peek)) {
        if (peek == '\n') {
            line++;
        }
        peek = getchar();
    }
}

void Lexer::SkipSingleLineComment()
{
    do {
        peek = getchar();
    } while (peek != '\n' && peek != EOF);
}

void Lexer::SkipComment()
{
    char prev;
    peek = ' ';
    do {
        prev = peek;
        peek = getchar();
        if (peek == '\n') {
            line++;
        }
    } while (peek != EOF && (prev != '*' || peek != '/'));
    peek = (peek == EOF) ? EOF : ' ';
}

const Token *Lexer::SkipSpacesAndComments()
{
    for (;;) {
        SkipSpaces();
        if (peek != '/') {
            return 0;
        }
        peek = getchar();
        if (peek == '/') {
            SkipSingleLineComment();
        } else if (peek == '*') {
            SkipComment();
        } else {
            return new Token('/');
        }
    }
}

const Token *Lexer::ScanNumber()
{
    if (IsDigit(peek)) {
        int v = 0;
        do {
            v = v * 10 + (peek - '0');
            peek = getchar();
        } while (IsDigit(peek));

        if (peek != '.') {
            return new Number(v);
        }
        double f = v, divisor = 10;
        while (IsDigit(peek = getchar())) {
            f += (peek - '0') / divisor;
            divisor *= 10;
        }
        return new Real(f);
    } else if (peek == '.') {
        peek = getchar();
        if (!IsDigit(peek)) {
            return new Token('.');
        }
        double f = 0, divisor = 10;
        do {
            f += (peek - '0') / divisor;
            divisor *= 10;
            peek = getchar();
        } while (IsDigit(peek));
        return new Real(f);
    }
    return 0;
}

const Token *Lexer::ScanLexeme()
{
    if (IsLetter(peek)) {
        lexeme_buffer.Clear();
        do {
            lexeme_buffer.Append(peek);
            peek = getchar();
        } while (IsLetter(peek) || IsDigit(peek));
        char *s = lexeme_buffer.String();
        Word *const *pw = words.Get(s);
        if (pw) {
            delete[] s;
            return new Word(**pw);
        }
        Word *w = new Word(ID, s);
        words.Put(s, w);
        return new Word(*w);
    }
    return 0;
}

const Token *Lexer::ScanTwoCharToken(char first, char second, int tag)
{
    if (peek == first) {
        peek = getchar();
        if (peek == second) {
            peek = ' ';
            return new Token(tag);
        }
        return new Token(first);
    }
    return 0;
}

const Token *Lexer::ScanComparisonOperator()
{
    const char chars[] = { '<', '>', '=', '!' };
    const int tags[] = { LTE, GTE, EQ, NEQ };

    unsigned int size = sizeof(chars) / sizeof(*chars);
    for (unsigned int i = 0; i < size; i++) {
        const Token *token = ScanTwoCharToken(chars[i], '=', tags[i]);
        if (token) {
            return token;
        }
    }
    return 0;
}

const Token *Lexer::Scan()
{
    const Token *token = SkipSpacesAndComments();
    if (token) {
        return token;
    }
    token = ScanNumber();
    if (token) {
        return token;
    }
    token = ScanLexeme();
    if (token) {
        return token;
    }
    token = ScanComparisonOperator();
    if (token) {
        return token;
    }
    token = new Token(peek);
    peek = ' ';
    return token;
}
