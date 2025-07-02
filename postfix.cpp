#include <stdio.h>

class Parser {
    enum error { noerror = 0, syntax_error = 1 };
    int lookahead;
public:
    int Run();
private:
    error Expr();
    error Term();
    error Factor();
    error Match(int t);
};

int Parser::Run()
{
    lookahead = getchar();
    if (lookahead == EOF)
        return 0;
    error err = Expr();
    if (err) {
        printf("syntax error\n");
        return 1;
    }
    putchar('\n');
    return 0;
}

Parser::error Parser::Expr()
{
    error err = Term();
    if (err)
        return err;
    while (lookahead == '+' || lookahead == '-') {
        int t = lookahead;
        err = Match(t);
        if (err)
            return err;
        err = Term();
        if (err)
            return err;
        putchar(t);
    }
    return noerror;
}

Parser::error Parser::Term()
{
    error err = Factor();
    if (err)
        return err;
    while (lookahead == '*' || lookahead == '/') {
        int t = lookahead;
        err = Match(t);
        if (err)
            return err;
        err = Factor();
        if (err)
            return err;
        putchar(t);
    }
    return noerror;
}

Parser::error Parser::Factor()
{
    if ('0' <= lookahead && lookahead <= '9') {
        int t = lookahead;
        error err = Match(t);
        if (err)
            return err;
        putchar(t);
        return noerror;
    } else if (lookahead == '(') {
        error err = Match('(');
        if (err)
            return err;
        err = Expr();
        if (err)
            return err;
        err = Match(')');
        if (err)
            return err;
        return noerror;
    }
    return syntax_error;
}

Parser::error Parser::Match(int t)
{
    if (lookahead == t) {
        lookahead = getchar();
        return noerror;
    }
    return syntax_error;
}

int main()
{
    Parser parser;
    return parser.Run();
}
