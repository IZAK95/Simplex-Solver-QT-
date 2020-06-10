#ifndef FRACTION_H
#define FRACTION_H
#include <QString>

class Fraction {
    long long numerator,
              denominator;
public:
    Fraction() {
        Initialize(0, 1);
    }
    Fraction(long long numerator, long long denominator = 1) {
        Initialize(numerator, denominator);
    }

    long long getNumerator() const {
        return numerator;
    }

    long long getDenominator() const {
        return denominator;
    }

    static QString getNumerator (const QString &value) {
        int ind = value.indexOf('/');
        if (ind == -1) return  value;
        return value.left(ind);
    }

    static QString getDenominator (const QString &value) {
        int ind = value.indexOf('/');
        if (ind == -1)return  "1";
        return value.right(value.length() - ind - 1);
    }

    QString getFraction() const {
        return QString::number(numerator) + (denominator == 1? "" : "/" + QString::number(denominator));
    }
private:
    static long long GCD(long long a, long long b) {
            if (b == 0) return a;
            return GCD(b, a % b);
    }

    void Initialize(long long numerator, long long denominator) {
        this->numerator = numerator;
        this->denominator = denominator;
        Reduce(*this);
    }
    void Reduce(Fraction &fraction) const {
        if(fraction.numerator == 0) {
            fraction.denominator = 1;
            return;
        }
        long long gcd = GCD(fraction.numerator, fraction.denominator);
        fraction.numerator /= gcd;
        fraction.denominator /= gcd;
        if(fraction.denominator < 0) {
            fraction.numerator *= -1;
            fraction.denominator *= -1;
        }
    }

    bool Equals(const Fraction &fraction) const {
        return numerator == fraction.numerator && denominator == fraction.denominator;
    }
    static Fraction Multiply(const Fraction &fraction1, const Fraction &fraction2) {
        return Fraction(fraction1.numerator * fraction2.numerator, fraction1.denominator * fraction2.denominator);
    }

    static Fraction Inverse(const Fraction &fraction) {
        if(fraction.numerator == 0)
            throw "Operation not possible (Denominator cannot be assigned a ZERO Value)";
        return Fraction(fraction.denominator, fraction.numerator);
    }
    static Fraction Negate(const Fraction &fraction) {
        return Fraction(-fraction.numerator, fraction.denominator);
    }
    static Fraction Add(const Fraction &fraction1, const Fraction &fraction2) {
        return Fraction(fraction1.numerator * fraction2.denominator + fraction2.numerator * fraction1.denominator,
                        fraction1.denominator * fraction2.denominator);
    }
public:
    friend Fraction operator - (const Fraction &fraction) {
        return Negate(fraction);
    }
    friend Fraction operator + (const Fraction &fraction1, const Fraction &fraction2) {
        return Add(fraction1, fraction2);
    }
    friend Fraction operator + (const Fraction &fraction, const long long &value) {
        return Add(fraction, Fraction(value));
    }

    friend Fraction operator + (const long long &value, const Fraction &fraction ) {
        return Add(fraction, Fraction(value));
    }
    friend Fraction operator - (const Fraction &fraction1, const Fraction &fraction2) {
        return Add(fraction1, -fraction2);
    }
    friend Fraction operator - (const long long &value, const Fraction &fraction) {
        return Add(-fraction, Fraction(value));
    }
    friend Fraction operator - (const Fraction &fraction, const long long &value) {
        return Add(fraction, -Fraction(value));
    }
    friend Fraction operator * (const Fraction &fraction1, const Fraction &fraction2) {
        return Multiply(fraction1, fraction2);
    }
    friend Fraction operator * (const Fraction &fraction, const long long &value) {
        return Multiply(fraction, Fraction(value));
    }
    friend Fraction operator * (const long long &value, const Fraction &fraction) {
        return Multiply(fraction, Fraction(value));
    }
    friend Fraction operator / (const Fraction &fraction1, const Fraction &fraction2) {
        return Multiply(fraction1, Inverse(fraction2));
    }
    friend Fraction operator / (const Fraction &fraction, const long long &value) {
        return Multiply(fraction, Inverse(Fraction(value)));
    }
    friend Fraction operator / (const long long &value, const Fraction &fraction) {
        return Multiply(Inverse(fraction), Fraction(value));
    }
    friend bool operator == (const Fraction &fraction1, const Fraction &fraction2) {
        return fraction1.Equals(fraction2);
    }
    friend bool operator != (const Fraction &fraction1, const Fraction &fraction2) {
        return !fraction1.Equals(fraction2);
    }
    friend bool operator == (const Fraction &fraction, const long long &value) {
        return fraction.Equals(Fraction(value));
    }
    friend bool operator != (const Fraction &fraction, const long long &value) {
        return !fraction.Equals(Fraction(value));
    }
    friend bool operator < (const Fraction &fraction1, const Fraction &fraction2) {
        return fraction1.numerator * fraction2.denominator < fraction2.numerator * fraction1.denominator;
    }
    friend bool operator < (const Fraction &fraction, const long long &value) {
       return fraction.numerator < value * fraction.denominator;
    }
    friend bool operator > (const long long &value, const Fraction &fraction) {
        return value * fraction.denominator > fraction.numerator;
    }
    friend bool operator <= (const Fraction &fraction, const long long &value) {
       return fraction.numerator <= value * fraction.denominator;
    }
    friend bool operator >= (const long long &value, const Fraction &fraction) {
        return value * fraction.denominator >= fraction.numerator;
    }
    friend bool operator <= (const Fraction &fraction1, const Fraction &fraction2) {
        return fraction1.numerator * fraction2.denominator <= fraction2.numerator * fraction1.denominator;
    }
    friend bool operator > (const Fraction &fraction1, const Fraction &fraction2) {
        return fraction1.numerator * fraction2.denominator > fraction2.numerator * fraction1.denominator;
    }
    friend bool operator >= (const Fraction &fraction1, const Fraction &fraction2) {
        return fraction1.numerator * fraction2.denominator >= fraction2.numerator * fraction1.denominator;
    }
    Fraction& operator += (const Fraction &fraction) {
          return *this = Add(*this, fraction);
    }
    Fraction& operator -= (const Fraction &fraction) {
        return *this = Add(*this, -fraction);
    }
    Fraction& operator *= (const Fraction &fraction) {
        return *this = Multiply(*this, fraction);
    }
    Fraction& operator /= (const Fraction &fraction) {
        return *this = Multiply(*this, Inverse((fraction)));
    }
};
#endif // FRACTION_H
