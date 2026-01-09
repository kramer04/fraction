#ifndef FRACTION_H
#define FRACTION_H

#include <iosfwd>

class Fraction
{
public:
  Fraction(long long n = 0, long long d = 1);

  long long num() const { return m_num; }
  long long den() const { return m_den; }

  void normalize(); // réduit + den>0 + checks

  // opérateurs
  friend Fraction operator+(const Fraction &a, const Fraction &b);
  friend Fraction operator-(const Fraction &a, const Fraction &b);
  friend Fraction operator*(const Fraction &a, const Fraction &b);
  friend Fraction operator/(const Fraction &a, const Fraction &b);

  friend std::ostream &operator<<(std::ostream &os, const Fraction &f);

private:
  long long m_num;
  long long m_den;
};

#endif
