#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

const char LEXER_CHAR_TOKENS[LEXER_NUM_CHAR_TOKENS] = { '(', ')', '+', '-', '*', '/', '\\', '{', '}', '"', '\'', ';', ':', '.', '#', '&', '>', '<', '=' };

int is_char_token(char token) {
  for (int i = 0; i < LEXER_NUM_CHAR_TOKENS; i++) {
    if (token == LEXER_CHAR_TOKENS[i])
      return 1;
  }

  return 0;
}

Lexer* create_lexer(char* file_name) {
  Lexer* lexer = malloc(sizeof(Lexer));
  Token** tokens = malloc(LEXER_MAX_TOKENS * sizeof(Token*));

  char* target_file_name = malloc(sizeof(char) * strlen(file_name));
  strcpy(target_file_name, file_name);
  target_file_name[strlen(file_name)] = '\0';
  
  lexer->target_file_name = target_file_name;
  lexer->tokens = tokens;
  lexer->num_tokens = 0;
  return lexer;
}

void destroy_lexer(Lexer* lexer) {
  if (lexer == NULL) return;
  
  free(lexer->target_file_name);

  for (int i = 0; i < lexer->num_tokens; i++) {
    Token* token = lexer->tokens[i];

    if (token->value != NULL) {
      free(token->value);
      token->value = NULL;
    }

    free(token);
    lexer->tokens[i] = NULL;
  }

  free(lexer->tokens);
  lexer->tokens = NULL;

  free(lexer);
  lexer = NULL;
}

inline int streq(char* str, char* target) {
  return !strcmp(str, target);
}

Token* get_token(char* t) {
  Token* token = (Token*) malloc(sizeof(Token));

  if (streq(t, "if")) {
    token->type = If;
  } else if (streq(t, "for")) {
    token->type = For;
  } else if (streq(t, "while")) {
    token->type = While;
  } else if (streq(t, "(")) {
    token->type = LeftParen;
  } else if (streq(t, ")")) {
    token->type = RightParen;
  } else if (streq(t, "{")) {
    token->type = LeftSquirly;
  } else if (streq(t, "}")) {
    token->type = RightSquirly;
  } else if (streq(t, "\"")) {
    token->type = DoubleQuote;
  } else if (streq(t, "'")) {
    token->type = SingleQuote;
  } else if (streq(t, "=")) {
    token->type = Equal;
  } else if (streq(t, "<")) {
    token->type = LessThan;
  } else if (streq(t, ">")) {
    token->type = GreaterThan;
  } else if (streq(t, ";")) {
    token->type = SemiColon;
  } else if (streq(t, ":")) {
    token->type = Colon;
  } else if (streq(t, "#")) {
    token->type = Pound;
  } else if (streq(t, ".")) {
    token->type = Period;
  } else if (streq(t, "include")) {
    token->type = Include;
  } else if (streq(t, "+")) {
    token->type = Plus;
  } else if (streq(t, "-")) {
    token->type = Hyphen;
  } else if (streq(t, "*")) {
    token->type = Asterisk;
  } else if (streq(t, "&")) {
    token->type = Ampersand;
  } else if (streq(t, "/")) {
    token->type = ForwardSlash;
  } else if (streq(t, "\\")) {
    token->type = BackwardSlash;
  } else {
    token->type = Unknown;
    
    char* value = malloc(sizeof(char) * strlen(t));
    strcpy(value, t);
    value[strlen(t)] = '\0';

    token->value = value;
  }

  return token;
}

void add_raw_token(Lexer* lexer, char* raw_token) {
  if (lexer->num_tokens >= LEXER_MAX_TOKENS)
    return;

  Token* token = get_token(raw_token);
  lexer->tokens[lexer->num_tokens] = token;
  lexer->num_tokens++;
}

void tokenize(Lexer* lexer) {
  FILE* fd = fopen(lexer->target_file_name, "r");
  if (fd == NULL) return;

  char token[LEXER_MAX_TOKEN_SIZE] = {'\0'};
  size_t token_size = 0;

  unsigned long c;

  while (!feof(fd)) {
    fread(&c, sizeof(char), 1, fd);

    if (c == '\n') {
      token[token_size] = '\0';
      if (token_size > 0)
        add_raw_token(lexer, token);
      token_size = 0;
    } else if (is_char_token(c)) {
      if (token_size > 0) {
        token[token_size] = '\0';
        add_raw_token(lexer, token);
        token_size = 0;
      }

      token[0] = c;
      token[1] = '\0';

      add_raw_token(lexer, token);
      token_size = 0;
    } else if (isspace(c)) {
      token[token_size] = '\0';
      if (token_size > 0)
        add_raw_token(lexer, token);
      token_size = 0;
    } else {
      token[token_size] = c;
      token_size++;
    }

    if (lexer->num_tokens >= LEXER_MAX_TOKENS)
      break;
  }

  fclose(fd);
  fd = NULL;
}
