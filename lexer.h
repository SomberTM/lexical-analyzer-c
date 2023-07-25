#ifndef LEXER_H
#define LEXER_H

typedef enum {
  If,
  Else,
  LeftParen,
  RightParen,
  LeftSquirly,
  RightSquirly,
  DoubleQuote,
  SingleQuote,
  Ident,
  Equal,
  LessThan,
  GreaterThan,
  Colon,
  SemiColon,
  Pound,
  Period,
  Include,
  Plus,
  Hyphen,
  Asterisk,
  ForwardSlash,
  BackwardSlash,
  Underscore,
  Typedef,
  Struct,
  Enum,
  Extern,
  Void
} TokenType;

typedef struct {
  TokenType type;
} Token;

typedef struct {
  char* target_file_name;
  FILE* file_handle;

  Token* tokens;
  int num_tokens;
} Lexer;

extern Lexer* create_lexer(char*);
extern void free_lexer(Lexer*);

#endif /* LEXER_H */