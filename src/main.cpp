#include <stdio.h>

#include "lexer.hpp"

static const char *get_token_string(int tag)
{
    switch (tag) {
    case LTE:
        return "<=";
    case GTE:
        return ">=";
    case EQ:
        return "==";
    case NEQ:
        return "!=";
    }
    return 0;
}

static bool print_token(const Token *token)
{
    const Number *n;
    const Real *r;
    const Word *w;

    int tag = token->GetTag();
    switch (tag) {
    case EOF:
        return false;
    case TRUE:
        printf("Boolean: true\n");
        return true;
    case FALSE:
        printf("Boolean: false\n");
        return true;
    case ID:
        w = static_cast<const Word*>(token);
        printf("Identifier: %s\n", w->GetLexeme());
        return true;
    case NUMBER:
        n = static_cast<const Number*>(token);
        printf("Number: %d\n", n->GetValue());
        return true;
    case REAL:
        r = static_cast<const Real*>(token);
        printf("Real: %f\n", r->GetValue());
        return true;
    }

    const char *s = get_token_string(tag);
    if (s) {
        printf("Token: %s\n", s);
    } else {
        printf("Token: %c\n", tag);
    }
    return true;
}

int main()
{
    Lexer lex;
    bool loop;
    do {
        Token *token = lex.Scan();
        loop = print_token(token);
        delete token;
    } while (loop);
    return 0;
}
