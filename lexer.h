#ifndef LEXER_H
#define LEXER_H

#define LEXER_MAX_TOKENS 1024
#define LEXER_MAX_TOKEN_SIZE 64
#define LEXER_NUM_CHAR_TOKENS 19

typedef enum {
  Number,
  Ident,
  If,
  For,
  While,
  LeftParen,
  RightParen,
  LeftSquirly,
  RightSquirly,
  DoubleQuote,
  SingleQuote,
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
  Ampersand,
  ForwardSlash,
  BackwardSlash,
  Underscore,
  EoL,
  EoF,
  Pipe,
  Comment,
  Unknown
} TokenType;

typedef struct {
  TokenType type;
  char* value;
} Token;

typedef struct {
  char* target_file_name;

  Token** tokens;
  int num_tokens;
} Lexer;

extern const char LEXER_CHAR_TOKENS[LEXER_NUM_CHAR_TOKENS];

extern Lexer* create_lexer(char*);
extern void destroy_lexer(Lexer*);
extern void tokenize(Lexer*);
extern Token* get_token(char*);
extern int streq(char*, char*);

static inline const char* get_token_str(Token* token) {
  if (token->value != NULL)
    return token->value;

  switch (token->type) {
    case If: return "if";
    case Equal: return "=";
    case LeftParen: return "(";
    case RightParen: return ")";
    case SemiColon: return ":";
    default: return "";
  }
}

#endif /* LEXER_H */
