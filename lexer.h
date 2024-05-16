#ifndef LEXER_H

#include <stddef.h>

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
  size_t num_tokens;
} Lexer;

extern const char LEXER_CHAR_TOKENS[LEXER_NUM_CHAR_TOKENS];

extern Lexer* create_lexer(char*);
extern void destroy_lexer(Lexer*);
extern void tokenize(Lexer*);
extern Token* get_token(char*);
extern int streq(char*, char*);

#endif /* LEXER_H */
