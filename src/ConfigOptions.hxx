#include <stdio.h>
#include "ConfigOptions.hh"
   
    template <typename T>
    Serverparameter<T>::Serverparameter(){
      is_set = false;
    }
    
    /*
    Serverparameter(T i)
    {
      parameter = i;
    }
    
   // template <class T>
    Serverparameter(T i, bool val)
    {
      parameter = i;
      is_set = val;
    }
    */

    template <typename T>
    T Serverparameter<T>::getparam()
    {
      return parameter;
    }

    template <typename T>
    void Serverparameter<T>::setparam(T i)
    {
      parameter = i;
      is_set = true;
    }
    template <typename T> 
    bool Serverparameter<T>::get(void) const
    {
      return is_set;
    }
    template <typename T>
    void Serverparameter<T>::set(bool value)
    {
      is_set = value;
    }
