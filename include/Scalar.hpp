#ifndef SCALAR_H
#define SCALAR_H

#include <iomanip>
#include <cmath>
#include "Tracer.hpp"

template <class T>
class Scalar{

  static std::atomic<unsigned> globalIdentifier;//thread-safe counter
  unsigned identifier;//to compare for equality when computing Var with X = Y
  T value;//value of the scalar
  T variance;//square of the 1 sigma error on the 'value'
  
public:
  Scalar() = default;
  template <class K>
  Scalar(K value);//sets the error to std::sqrt(value)
  template <class K1, class K2>
  Scalar(K1 value, K2 error);
  Scalar(const Scalar<T>& other) = default;
  Scalar(Scalar<T>&& other) = default;
  ~Scalar() = default;
  Scalar<T>& operator = (const Scalar<T>& other) = default;
  Scalar<T>& operator = (Scalar<T>&& other) = default;
  template <class K>
  Scalar<T>& operator = (K otherValue);
  Scalar<T> operator-();
  template <class K>
  Scalar<T>& operator+=(const Scalar<K>& other);
  template <class K>
  Scalar<T>& operator+=(K otherValue);//otherValue is assumed to be error-less
    template <class K>
  Scalar<T>& operator-=(const Scalar<K>& other);
  template <class K>
  Scalar<T>& operator-=(K otherValue);//otherValue is assumed to be error-less
  template <class K>
  Scalar<T>& operator*=(const Scalar<K>& other);
  template <class K>
  Scalar<T>& operator*=(K otherValue);//otherValue is assumed to be error-less
  template <class K>
  Scalar<T>& operator/=(const Scalar<K>& other);
  template <class K>
  Scalar<T>& operator/=(K otherValue);//otherValue is assumed to be error-less
  template <class K>
  bool isEqualTo(const Scalar<K>& other) const;
  template <class K>
  bool isEqualTo(K otherValue) const;
  template <class K>
  bool isLessThan(const Scalar<K>& other) const;//does not take errors into account
  template <class K>
  bool isLessThan(K otherValue) const;//does not take errors into account
  T getValue() const;
  T getError() const;
  T getRelativeError() const;
  template <class K>
  void setValue(K value);
  template <class K>
  void setError(K error);
  
};

template <class T>
std::atomic<unsigned> Scalar<T>::globalIdentifier{};//allocate space and assign to default (zero) value

template <class T>
std::ostream& operator<<(std::ostream& output, const Scalar<T>& scalar){
  
  output<<std::setw(7)<<std::left<<scalar.getValue()
    <<" +/-  "<<std::setw(7)<<std::left<<scalar.getError();
    
  if(scalar.getValue() != T{}) output<<"  ("<<std::setw(5)<<std::left<<(100*scalar.getRelativeError())<<"%)";//add the relative error for non-zero values
  return output;
  
}

template <class T, class K>
Scalar<T> operator+(Scalar<T> scalar1, const Scalar<K>& scalar2){
  
  return scalar1 += scalar2;
  
}

template <class T, class K>
Scalar<T> operator+(Scalar<T> scalar, K otherValue){
  
  return scalar += otherValue;
  
}

template <class T, class K>
Scalar<T> operator+(K otherValue, const Scalar<T>& scalar){
  
  return scalar + otherValue;
  
}

template <class T, class K>
Scalar<T> operator-(Scalar<T> scalar1, const Scalar<K>& scalar2){
  
  return scalar1 -= scalar2;
  
}

template <class T, class K>
Scalar<T> operator-(Scalar<T> scalar, K otherValue){
  
  return scalar -= otherValue;
  
}

template <class T, class K>
Scalar<T> operator-(K otherValue, const Scalar<T>& scalar){
  
  return  Scalar<T>(otherValue, K{}) -scalar;//assume zero error on otherValue, so force the error to zero since the usual constructor doesn't
  
}

template <class T, class K>
Scalar<T> operator*(Scalar<T> scalar1, const Scalar<K>& scalar2){
  
  return scalar1 *= scalar2;
  
}

template <class T, class K>
Scalar<T> operator*(Scalar<T> scalar, K otherValue){
  
  return scalar *= otherValue;
  
}

template <class T, class K>
Scalar<T> operator*(K otherValue, const Scalar<T>& scalar){
  
  return scalar * otherValue;
  
}

template <class T, class K>
Scalar<T> operator/(Scalar<T> scalar1, const Scalar<K>& scalar2){
  
  return scalar1 /= scalar2;
  
}

template <class T, class K>
Scalar<T> operator/(Scalar<T> scalar, K otherValue){
  
  return scalar /= otherValue;
  
}

template <class T, class K>
Scalar<T> operator/(K otherValue, const Scalar<T>& scalar){
  
  return Scalar<T>(otherValue, K{})/scalar;//assume zero error on otherValue, so force the error to zero since the usual constructor doesn't
  
}

template <class T, class K>
bool operator == (const Scalar<T>& scalar1, const Scalar<K>& scalar2){
  
  return scalar1.isEqualTo(scalar2);
  
}

template <class T, class K>
bool operator == (const Scalar<T>& scalar, K otherValue){
  
  return scalar.isEqualTo(otherValue);
  
}

template <class T, class K>
bool operator == (K otherValue, const Scalar<T>& scalar){
  
  return scalar == otherValue;
  
}

template <class T, class K>
bool operator != (const Scalar<T>& scalar1, const Scalar<K>& scalar2){
  
  return !(scalar1 == scalar2);
  
}

template <class T, class K>
bool operator != (const Scalar<T>& scalar, K otherValue){
  
  return !(scalar == otherValue);
  
}

template <class T, class K>
bool operator != (K otherValue, const Scalar<T>& scalar){
  
  return scalar != otherValue;
  
}

template <class T, class K>
bool operator<(const Scalar<T>& scalar1, const Scalar<K>& scalar2){
  
  return scalar1.isLessThan(scalar2);
  
}

template <class T, class K>
bool operator<(const Scalar<T>& scalar, K otherValue){
  
  return scalar.isLessThan(otherValue);
  
}

template <class T, class K>
bool operator<(K otherValue, const Scalar<T>& scalar){
  
  return !(scalar < otherValue);
  
}

template <class T, class K>
bool operator>(const Scalar<T>& scalar1, const Scalar<K>& scalar2){
  
  return !(scalar1 < scalar2);
  
}

template <class T, class K>
bool operator>(const Scalar<T>& scalar, K otherValue){
  
  return !(scalar < otherValue);
  
}

template <class T, class K>
bool operator>(K otherValue, const Scalar<T>& scalar){
  
  return scalar < otherValue;
  
}

template <class T>
template <class K>
Scalar<T>::Scalar(K value):identifier(++globalIdentifier),value(value),variance(std::abs(value)){
  
}

template <class T>
template <class K1, class K2>
Scalar<T>::Scalar(K1 value, K2 error):identifier(++globalIdentifier),value(value),variance(error * error){
  
}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator = (K otherValue){
  
  return *this = Scalar<T>(otherValue);

}

template <class T>
Scalar<T> Scalar<T>::operator - (){
  
  Scalar<T> opposite{*this};//keep the error
  opposite.value = - opposite.value;//change the value
  return opposite;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator+=(const Scalar<K>& other){

  if(identifier != other.identifier){
    
    variance += other.variance;//indepent case Cov(X,Y) = 0
    identifier = ++globalIdentifier;//when adding another variable, assume that X+Y is now independent enough from X and from Y(for lack of an efficient alternative)
    
  }
  else  variance += other.variance + 2 * std::sqrt(variance*other.variance);//if linearly (Person) correlated, also add 2 sqrt(varX) sqrt(varY)
  
  value += other.value;
  
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator+=(K otherValue){
  
  value += otherValue;
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator-=(const Scalar<K>& other){

  if(identifier != other.identifier){
    
    variance += other.variance;//indepent case Cov(X,Y) = 0
    identifier = ++globalIdentifier;//when substracting another variable, assume that X-Y is now independent enough from X and from Y(for lack of an efficient alternative)
    
  }
  else  variance += other.variance - 2 * std::sqrt(variance*other.variance);//if linearly (Person) correlated, also add - 2 sqrt(varX) sqrt(varY)
  
  value -= other.value;
  
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator-=(K otherValue){
  
  value -= otherValue;
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator*=(const Scalar<K>& other){
  
  if(identifier != other.identifier){
    
    variance = std::pow(value,2) * other.variance + std::pow(other.value,2) * variance +  variance * other.variance;//for independent variables VarXY = //EX2 varY  + EY2 varX  + varX varY
    value *= other.value;
    identifier = ++globalIdentifier;//when multiplying by another variable, assume that XY is now independent enough from X and from Y
    
  }
  else{//for linearly correlated variables

    auto covariance = std::sqrt(variance * other.variance);
    
    variance = std::pow(value,2) * other.variance + std::pow(other.value,2) * variance //EX2 varY  + EY2 varX 
      - 2 * value * other.value * covariance //2 EX EY Cov(X,Y)
      + std::sqrt(
	(4 * std::pow(value,2) * variance + 2 * std::pow(variance,2)) * (4 * std::pow(other.value,2) * other.variance + 2 * std::pow(other.variance,2))
      );//Cov(X2, Y2) = sqrt(VarX2 VarY2)
 
    value = value * other.value + covariance;//mu_1 * mu_2 + sigma_1 * sigma_2
    
  }
  
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator*=(K otherValue){

  variance *= otherValue * otherValue;//Var(aX) = a2 VarX
  value *= otherValue;
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator/=(const Scalar<K>& other){

  T zero{};
  
  if(other.value != zero){
    
    if(identifier != other.identifier){
      
      variance = std::pow(value/other.value,2) * (variance/std::pow(value,2) + other.variance/std::pow(other.value,2));//for independent variables (EX/EY) * sqrt(relativeErrorX2 + relativeErrorY2)
      value = value / other.value + value * other.variance / std::pow(other.value, 3);//second order Taylor expansion approximation of E(X/Y)
      identifier = ++globalIdentifier;//when dividing by another variable, assume that X/Y is now independent enough from X and from Y
      
    }
    else{// linearly correlated case
      
      auto covariance = std::sqrt(variance * other.variance);
      variance = std::pow(value * std::sqrt(other.variance) - other.value * std::sqrt(variance),2)/std::pow(other.value,4);
      value = value / other.value - covariance/std::pow(other.value,2) + value * other.variance / std::pow(other.value, 3);//second order Taylor expansion approximation of E(X/Y)
      
    }
    
  }
  else Tracer(Verbose::Warning)<<"Scalar division by "<<zero<<" not allowed!"<<std::endl;

  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator/=(K otherValue){
  
  T zero{};
  
  if(otherValue != zero){
    
    variance /= otherValue * otherValue;// Var(X/a) = (1/a2) VarX
    value /= otherValue;
    
  }
  else Tracer(Verbose::Warning)<<"Scalar division by "<<zero<<" not allowed!"<<std::endl;
  
  return *this;

}

template <class T>
template <class K>
bool Scalar<T>::isEqualTo(const Scalar<K>& other) const{
  
  return value == other.value && variance == other.variance;
  
}

template <class T>
template <class K>
bool Scalar<T>::isEqualTo(K otherValue) const{
  
  return value == otherValue;
  
}

template <class T>
template <class K>
bool Scalar<T>::isLessThan(const Scalar<K>& other) const{
  
  return value < other.value;

}

template <class T>
template <class K>
bool Scalar<T>::isLessThan(K otherValue) const{
  
  return value < otherValue;

}

template <class T>
T Scalar<T>::getValue() const{

  return value;
  
}

template <class T>
T Scalar<T>::getError() const{

  return std::sqrt(variance);
  
}

template <class T>
T Scalar<T>::getRelativeError() const{

  return getError()/value;
  
}

template <class T>
template <class K>
void Scalar<T>::setValue(K value){

  this->value = value;
  
}

template <class T>
template <class K>
void Scalar<T>::setError(K error){

  this->variance = error*error;
  
}

#endif