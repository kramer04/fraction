#include "Fraction.h"

#include <numeric> // std::gcd
#include <stdexcept>
#include <ostream>

Fraction::Fraction(long long n, long long d) : m_num(n), m_den(d)
{
  normalize();
}

void Fraction::normalize()
{
  if (m_den == 0)
    throw std::runtime_error("Division par 0 (denominateur = 0).");

  if (m_den < 0)
  {
    m_den = -m_den;
    m_num = -m_num;
  }

  long long a = (m_num < 0) ? -m_num : m_num;
  long long g = std::gcd(a, m_den);
  if (g != 0)
  {
    m_num /= g;
    m_den /= g;
  }
}

Fraction operator+(const Fraction &a, const Fraction &b)
{
  return Fraction(a.m_num * b.m_den + b.m_num * a.m_den, a.m_den * b.m_den);
}

Fraction operator-(const Fraction &a, const Fraction &b)
{
  return Fraction(a.m_num * b.m_den - b.m_num * a.m_den, a.m_den * b.m_den);
}

Fraction operator*(const Fraction &a, const Fraction &b)
{
  return Fraction(a.m_num * b.m_num, a.m_den * b.m_den);
}

Fraction operator/(const Fraction &a, const Fraction &b)
{
  if (b.m_num == 0)
    throw std::runtime_error("Division par 0.");
  return Fraction(a.m_num * b.m_den, a.m_den * b.m_num);
}

std::ostream &operator<<(std::ostream &os, const Fraction &f)
{
  if (f.m_den == 1)
    os << f.m_num;
  else
    os << f.m_num << "/" << f.m_den;
  return os;
}
