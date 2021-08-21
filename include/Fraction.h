#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <vector>
#include <string>

class Fraction {
public:
  Fraction();
  void afficher();
  std::vector<std::string> extraction(std::string entree);
  void calcul(std::vector<std::string> bloc);
private:
  int m_num, m_den;
};

#endif