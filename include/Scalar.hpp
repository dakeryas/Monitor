#ifndef SCALAR_H
#define SCALAR_H

#include <iomanip>
#include <cmath>
#include "Tracer.hpp"

template <class T>
class Scalar{

  T value;
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
  template <class K>
  Scalar<T>& operator+=(const Scalar<K>& other);
  template <class K>
  Scalar<T>& operator+=(K otherValue);//otherValue is assumed to be error-less
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
Scalar<T>::Scalar(K value):value(value),variance(value){
  
}

template <class T>
template <class K1, class K2>
Scalar<T>::Scalar(K1 value, K2 error):value(value),variance(error * error){
  
}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator = (K otherValue){
  
  return *this = Scalar<T>(otherValue);

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator+=(const Scalar<K>& other){

  if(this != &other) variance += other.variance;
  else variance *= 4;//Var(2X) = 4 Var(X)
  
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
Scalar<T>& Scalar<T>::operator*=(const Scalar<K>& other){
  
  if(this != &other) variance = other.variance * std::pow(value,2) + variance * std::pow(other.value,2) +  variance * other.variance;//for independent variables VarXY = varY EX2 + varX EY2 + varX varY
  else variance = 4 * variance * std::pow(value,2) + 2 * std::pow(variance,2); //for gaussians Var(X2) = 4 EX2 VarX + 2 VarX2
  
  value *= other.value;
  
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
  
  if(this != &other){
  
    T zero{};
    
    if(other.value != zero){
      
      variance = std::pow(value/other.value,2) * (variance/std::pow(value,2) + other.variance/std::pow(other.value,2));//for independent variables (EX/EY) * sqrt(relativeErrorX2 + relativeErrorY2)
      value /= other.value;//first order Taylor expansion approximation of E(X/Y)
      
    }
    else Tracer(Verbose::Warning)<<"Scalar division by "<<zero<<" not allowed!"<<std::endl;
    
  }
  else{// X over X equals to one with no error
    
    value = 1;
    variance = T{};
    
  }
  
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