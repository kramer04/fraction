#include <iostream>
#include <string>

#include "ExprParser.h"

int main()
{
  std::cout << "Entrer une expression:\n";
  std::string input;
  std::getline(std::cin, input);

  try
  {
    ExprParser p;
    Fraction r = p.eval(input);
    std::cout << "Resultat: " << r << "\n";
  }
  catch (const std::exception &e)
  {
    std::cout << "Erreur: " << e.what() << "\n";
  }
  return 0;
}
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c?page=1&tab=votes#tab-top