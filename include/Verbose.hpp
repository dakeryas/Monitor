#ifndef VERBOSE_H
#define VERBOSE_H

#include <map>
#include <iostream>

enum class Verbose{Quiet, Error, Warning, Debug};

std::ostream& operator<<(std::ostream& output, Verbose verbose);
std::istream& operator>>(std::istream& in, Verbose& verbose);

#endif