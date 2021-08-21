#include "Fraction.h"

int main()
{
  Fraction fraction;
  std::string entree;
  std::cout << "Addition soustraction de fraction" << std::endl;
  std::cin >> entree;
  

  fraction.calcul(fraction.extraction(entree));
  std::cout << entree << " = ";
  fraction.afficher();
  
  return 0;
}