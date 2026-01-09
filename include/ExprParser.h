#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <string>
#include "Fraction.h"

class ExprParser
{
public:
  // Parse + évalue. Lance std::runtime_error si erreur.
  Fraction eval(const std::string &input);

private:
  enum class TokType
  {
    INT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    END
  };

  static bool startsFactor(TokType t);

  struct Token
  {
    TokType type;
    long long value = 0;
    std::size_t start = 0; // index dans la string
    std::size_t end = 0;   // index juste après le token
  };

  // Lexer state
  std::string m_s;
  std::size_t m_i = 0;
  Token m_cur{TokType::END, 0};
  Token peekToken();

  // lexer helpers
  void skipSpaces();
  Token nextToken();
  void eat(TokType t);

  // parser (recursive descent)
  Fraction parseExpr();   // + -
  Fraction parseTerm();   // * /
  Fraction parseFactor(); // unaire -, parenthèses, nombre
  Fraction parseNumber(); // INT ou INT/INT

  //donne la colonne de l'erreur
  [[noreturn]] void fail(const std::string &msg) const;
};

#endif
