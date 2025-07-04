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
        } else {
            return new Token('/');
        }
    }
}

const Number *Lexer::ScanNumber()
{
    if (IsDigit(peek)) {
        int v = 0;
        do {
            v = v * 10 + (peek - '0');
            peek = getchar();
        } while (IsDigit(peek));
        return new Number(v);
    }
    return 0;
}

const Word *Lexer::ScanLexeme()
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

const Token *Lexer::Scan()
{
    const Token *token = SkipSpacesAndComments();
    if (token) {
        return token;
    }
    const Number *number = ScanNumber();
    if (number) {
        return number;
    }
    const Word *word = ScanLexeme();
    if (word) {
        return word;
    }
    token = new Token(peek);
    peek = ' ';
    return token;
}
