#include "Fraction.h"
#include <algorithm>
#include <numeric>

//https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c?page=1&tab=votes#tab-top
bool isNbr(std::string &str)
{
  return std::ranges::all_of(str.begin(), str.end(), [](unsigned char c) {return isdigit(c) != 0; });
}

Fraction::Fraction() :m_num(0), m_den(1) {}

std::vector<std::string> Fraction::extraction(std::string entree)
{
  std::vector<std::string> bloc;
  std::string str;
  u_int64_t taille {entree.size()};
  int j {-1};
  bool isNbr = true;

  for (u_int16_t i {0}; i < taille; ++i)
  {
    if (isdigit(entree[i]) && isNbr == false)
    {
      str = entree[i];
      bloc[j] = bloc[j] + str;
    }
    if (isdigit(entree[i]) && isNbr)
    {
      str = entree[i];
      bloc.push_back(str);
      isNbr = false;
      ++j;
    }
    if (isdigit(entree[i]) == false)
    {
      str = entree[i];
      if (str != "/")
      {
        bloc.push_back(str);
        ++j;
      }
      isNbr = true;
    }
  }
  return bloc;
}

void Fraction::calcul(std::vector<std::string> bloc)
{
  u_int64_t taille {bloc.size()};

  //std::cout << std::stoi(bloc[0]) << "/" << std::stoi(bloc[1]) << bloc[2] << std::stoi(bloc[3]) << "/" << std::stoi(bloc[4]) << std::endl;
  // si le signe - est au tout début, ex -1/2+2/3
  int moins {1};
  if (bloc[0] == "-")
  {
    moins = -1;
    bloc.erase(std::begin(bloc));
  }
  // le & capture implicitement la variable bloc et moins
  auto signe = [&, this](int a)->int {bloc[2] == "-" ? a = -1 : a = 1; return moins * std::stoi(bloc[0]) * std::stoi(bloc[4]) + a * std::stoi(bloc[1]) * std::stoi(bloc[3]); };
  m_num = signe(0);
  m_den = std::stoi(bloc[1]) * std::stoi(bloc[4]);
  int pgcd {std::gcd(m_num, m_den)};
  m_num /= pgcd;
  m_den /= pgcd;

  //u_int16_t i {0};

  if (taille >= 5)
  {
    for (u_int16_t i {5}; i < taille - 2; i += 3)
    {
      auto lambda = [&, this](int a)->int {bloc[i] == "-" ? a = -1 : a = 1; return m_num * std::stoi(bloc[i + 2]) + a * m_den * std::stoi(bloc[i + 1]); };
      m_num = lambda(0);
      m_den *= std::stoi(bloc[i + 2]);
      pgcd = std::gcd(m_num, m_den);
      m_num /= pgcd;
      m_den /= pgcd;
    }
  }
}
void Fraction::afficher()
{
  std::cout << "--------------------------" << std::endl;
  std::cout << "Résultat :" << std::endl;
  std::cout << m_num << "/" << m_den << std::endl;
  std::cout << "--------------------------" << std::endl;
}