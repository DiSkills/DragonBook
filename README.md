## Syntax-Directed Translation Scheme

```
  expr -> expr + term   { print('+') }
       |  expr - term   { print('-') }
       |  term
  term -> term * factor { print('*') }
       |  term / factor { print('/') }
       |  factor
factor -> ( expr )
       |  digit       { print(digit) }
```
