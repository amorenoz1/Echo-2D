#ifndef UTILS_H
#define UTILS_H

#include <core/core.h>

namespace Utils {
   
template <typename T> class Singleton {
public:
   static T& GetInstance() {
      static T instance;
      return instance;
   }
      
   Singleton(const Singleton&) = delete;
   Singleton& operator=(const Singleton&) = delete;

protected:
   Singleton() = default;
   virtual ~Singleton() = default;
};

}

#endif
