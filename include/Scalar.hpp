#ifndef SCALAR_H
#define SCALAR_H

#include <iomanip>
#include <cmath>
#include "Tracer.hpp"

template <class T>
class Scalar{

  T value;
  T error;
  
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
  bool operator == (const Scalar<K>& other) const;
  template <class K>
  bool operator == (K otherValue) const;
  template <class K>
  bool operator<(const Scalar<K>& other) const;//does not take errors into account
  template <class K>
  bool operator<(K otherValue) const;//does not take errors into account
  T getValue() const;
  T getError() const;
  template <class K>
  void setValue(K value);
  template <class K>
  void setError(K error);
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Scalar<T>& scalar){
  
  output<<std::setw(7)<<std::left<<scalar.getValue()
    <<"+/-"<<std::setw(2)<<std::left<<" "<<std::setw(5)<<std::left<<scalar.getError();
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

template <class T>
template <class K>
Scalar<T>::Scalar(K value):Scalar(value, std::sqrt(std::abs(value))){
  
}

template <class T>
template <class K1, class K2>
Scalar<T>::Scalar(K1 value, K2 error):value(value),error(error){
  
}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator = (K otherValue){
  
  error = std::sqrt(std::abs(otherValue));
  value = otherValue;
  
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator+=(const Scalar<K>& other){
  
  error = std::sqrt(std::pow(error,2) + std::pow(other.error,2));
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
  
  error = std::sqrt(std::pow(value * other.error,2) + std::pow(other.value * error,2) +  std::pow(error * other.error,2));//for independent variables sqrt(varY EX2 + varX EY2 + varX varY)
  value *= other.value;
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator*=(K otherValue){
  
  error *= otherValue;
  value *= otherValue;
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator/=(const Scalar<K>& other){
  
  T zero{};
  
  if(other.value != zero){
    
    error = (value  / other.value) * std::sqrt(std::pow(error/value,2) + std::pow(other.error/other.value,2));//for independent variables (EX/EY) * sqrt(relativeErrorX2 + relativeErrorY2)
    value /= other.value;//first order Taylor expansion approximation of E(X/Y)
    
  }
  else Tracer(Verbose::Warning)<<"Scalar division by "<<zero<<" not allowed!"<<std::endl;
  
  return *this;

}

template <class T>
template <class K>
Scalar<T>& Scalar<T>::operator/=(K otherValue){
  
  T zero{};
  
  if(otherValue != zero){
    
    error /= otherValue;
    value /= otherValue;
    
  }
  else Tracer(Verbose::Warning)<<"Scalar division by "<<zero<<" not allowed!"<<std::endl;
  
  return *this;

}

template <class T>
template <class K>
bool Scalar<T>::operator == (const Scalar<K>& other) const{
  
  return value == other.value && error == other.error;
  
}

template <class T>
template <class K>
bool Scalar<T>::operator == (K otherValue) const{
  
  return value == otherValue;
  
}

template <class T>
template <class K>
bool Scalar<T>::operator<(const Scalar<K>& other) const{
  
  return value < other.value;

}

template <class T>
template <class K>
bool Scalar<T>::operator<(K otherValue) const{
  
  return value < otherValue;

}

template <class T>
T Scalar<T>::getValue() const{

  return value;
  
}

template <class T>
T Scalar<T>::getError() const{

  return error;
  
}

template <class T>
template <class K>
void Scalar<T>::setValue(K value){

  this->value = value;
  
}

template <class T>
template <class K>
void Scalar<T>::setError(K error){

  this->error = std::abs(error);
  Tracer(Verbose::Warning)<<"Deviation "<<error<<" is negative => Absolute value set"<<std::endl;
  
}

#endif