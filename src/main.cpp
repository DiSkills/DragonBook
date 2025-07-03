#include <stdio.h>

#include "lexer.hpp"

static bool print_token(const Token *token)
{
    const Number *n;
    const Word *w;

    bool res = true;
    switch (token->GetTag()) {
    case EOF:
        res = false;
        break;
    case TRUE:
        printf("Boolean: true\n");
        break;
    case FALSE:
        printf("Boolean: false\n");
        break;
    case ID:
        w = static_cast<const Word*>(token);
        printf("Identifier: %s\n", w->GetLexeme());
        break;
    case NUMBER:
        n = static_cast<const Number*>(token);
        printf("Number: %d\n", n->GetValue());
        break;
    default:
        printf("Token: %c\n", token->GetTag());
    }
    return res;
}

int main()
{
    Lexer lex;
    bool loop;
    do {
        const Token *token = lex.Scan();
        loop = print_token(token);
        delete token;
    } while(loop);
    return 0;
}
