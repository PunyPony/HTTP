#pragma once

#include <stdio.h>


template <typename T>
class Serverparameter{
  public:
    Serverparameter();
   // Serverparameter(T);
   // Serverparameter(T, bool);
    T getparam();
    void setparam(T);
    bool get(void) const;
    void set(bool value);

  private:
    T parameter;
    bool is_set;


};

#include "ConfigOptions.hxx"
