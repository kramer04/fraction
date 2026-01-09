#include "ExprParser.h"

#include <cctype>
#include <limits>
#include <stdexcept>
#include <string>

// =====================
//  Helpers d'erreur
// =====================

[[noreturn]] void ExprParser::fail(const std::string &msg) const
{
  // Si on est à END, m_cur.start peut être == size() (ok)
  std::size_t col = m_cur.start + 1; // colonne 1-based
  throw std::runtime_error(msg + " (col " + std::to_string(col) + ")");
}

// =====================
//  API
// =====================

Fraction ExprParser::eval(const std::string &input)
{
  m_s = input;
  m_i = 0;
  m_cur = nextToken();

  Fraction res = parseExpr();

  if (m_cur.type != TokType::END)
  {
    if (m_cur.type == TokType::RPAREN)
      fail("Parenthese ')' en trop");
    fail("Expression invalide (caracteres en trop)");
  }

  return res;
}

// =====================
//  Lexer
// =====================

void ExprParser::skipSpaces()
{
  while (m_i < m_s.size() && std::isspace(static_cast<unsigned char>(m_s[m_i])))
    ++m_i;
}

ExprParser::Token ExprParser::nextToken()
{
  skipSpaces();

  if (m_i >= m_s.size())
    return {TokType::END, 0, m_i, m_i};

  std::size_t start = m_i;
  char c = m_s[m_i];

  if (std::isdigit(static_cast<unsigned char>(c)))
  {
    long long v = 0;

    while (m_i < m_s.size() && std::isdigit(static_cast<unsigned char>(m_s[m_i])))
    {
      int d = m_s[m_i] - '0';

      // overflow check minimal
      if (v > (std::numeric_limits<long long>::max() - d) / 10)
        throw std::runtime_error("Entier trop grand (overflow)");

      v = v * 10 + d;
      ++m_i;
    }

    return {TokType::INT, v, start, m_i};
  }

  ++m_i;
  TokType t;

  switch (c)
  {
  case '+':
    t = TokType::PLUS;
    break;
  case '-':
    t = TokType::MINUS;
    break;
  case 'x':
  case 'X':
  case '*':
    t = TokType::MUL;
    break;
  case '/':
    t = TokType::DIV;
    break;
  case '(':
    t = TokType::LPAREN;
    break;
  case ')':
    t = TokType::RPAREN;
    break;
  default:
    fail(std::string("Caractere invalide: '") + c + "'");
  }

  return {t, 0, start, start + 1};
}

// Peek du token suivant (sans consommer) : IMPORTANT -> restaurer m_i ET m_cur
ExprParser::Token ExprParser::peekToken()
{
  std::size_t save_i = m_i;
  Token save_cur = m_cur;

  Token t = nextToken();

  m_i = save_i;
  m_cur = save_cur;
  return t;
}

void ExprParser::eat(TokType t)
{
  if (m_cur.type != t)
    fail("Syntaxe invalide (token inattendu)");
  m_cur = nextToken();
}

// =====================
//  Parser
// =====================

// expr := term (('+'|'-') term)*
Fraction ExprParser::parseExpr()
{
  Fraction left = parseTerm();

  while (m_cur.type == TokType::PLUS || m_cur.type == TokType::MINUS)
  {
    TokType op = m_cur.type;
    eat(op);

    Fraction right = parseTerm();
    left = (op == TokType::PLUS) ? (left + right) : (left - right);
  }

  return left;
}

// helper (doit être membre statique car TokType est private)
bool ExprParser::startsFactor(TokType t)
{
  return t == TokType::INT || t == TokType::LPAREN;
}

// term := factor (('*'|'/') factor | implicitMul factor)*
Fraction ExprParser::parseTerm()
{
  Fraction left = parseFactor();

  while (true)
  {
    if (m_cur.type == TokType::MUL)
    {
      eat(TokType::MUL);
      left = left * parseFactor();
    }
    else if (m_cur.type == TokType::DIV)
    {
      eat(TokType::DIV);
      left = left / parseFactor();
    }
    else if (startsFactor(m_cur.type))
    {
      // Multiplication implicite: 2(3+1), (1/2)(2/3), 2-(-3)(4)...
      left = left * parseFactor();
    }
    else
    {
      break;
    }
  }

  return left;
}

// factor := '-' factor | '(' expr ')' | number
Fraction ExprParser::parseFactor()
{
  if (m_cur.type == TokType::MINUS)
  {
    eat(TokType::MINUS);
    Fraction f = parseFactor();
    return Fraction(-f.num(), f.den());
  }

  if (m_cur.type == TokType::LPAREN)
  {
    eat(TokType::LPAREN);
    Fraction inside = parseExpr();

    if (m_cur.type != TokType::RPAREN)
      fail("Parenthese fermante ')' manquante");

    eat(TokType::RPAREN);
    return inside;
  }

  if (m_cur.type == TokType::PLUS)
  {
    eat(TokType::PLUS);
    return parseFactor();
  }

  return parseNumber();
}

// number supporte:
// - entier: 8
// - fraction litterale: 2/3 (UNIQUEMENT si '/' suivi d'un INT)
// - fraction mixte AVEC ESPACE: 8 2/3 (interpretee comme 8 + 2/3)
//
// IMPORTANT: la fraction mixte doit être reconnue AVANT la multiplication implicite,
// sinon "8 2/3" deviendrait "8 * 2/3".
Fraction ExprParser::parseNumber()
{
  if (m_cur.type != TokType::INT)
    fail("Nombre attendu");

  // Token du "whole" (partie entiere potentielle)
  Token wholeTok = m_cur;
  long long whole = m_cur.value;
  eat(TokType::INT);

  // ---------------------------
  // 1) Tentative fraction mixte: "8 2/3"
  // Condition: prochain token INT ET il y a eu au moins un espace
  // entre wholeTok et ce INT (aTok.start > wholeTok.end)
  // Et ensuite: '/' puis INT
  // ---------------------------
  if (m_cur.type == TokType::INT)
  {
    Token aTok = m_cur;
    bool hasSpace = (aTok.start > wholeTok.end);

    if (hasSpace)
    {
      // On vérifie le pattern: INT '/' INT
      // On est actuellement sur INT (a)
      Token afterA = peekToken(); // token après "a"
      if (afterA.type == TokType::DIV)
      {
        // Pour voir ce qu'il y a après '/', on fait un peek "à 2 pas"
        std::size_t save_i = m_i;
        Token save_cur = m_cur;

        // simule: consommer token après a (qui est '/') puis lire suivant
        Token t1 = nextToken(); // devrait être '/'
        Token t2 = nextToken(); // devrait être INT (den)

        m_i = save_i;
        m_cur = save_cur;

        if (t1.type == TokType::DIV && t2.type == TokType::INT)
        {
          long long a = m_cur.value;
          eat(TokType::INT); // consomme a
          eat(TokType::DIV); // consomme '/'
          long long b = m_cur.value;
          eat(TokType::INT); // consomme b

          // whole + a/b
          return Fraction(whole, 1) + Fraction(a, b);
        }
      }
    }
  }

  // ---------------------------
  // 2) Fraction littérale: "n/d"
  // UNIQUEMENT si '/' suivi d'un INT
  // (donc "7/(8-1)" reste une division)
  // ---------------------------
  if (m_cur.type == TokType::DIV)
  {
    Token next = peekToken(); // token après '/'
    if (next.type == TokType::INT)
    {
      eat(TokType::DIV);
      long long d = m_cur.value;
      eat(TokType::INT);
      return Fraction(whole, d);
    }
  }

  // ---------------------------
  // 3) Entier simple
  // ---------------------------
  return Fraction(whole, 1);
}
