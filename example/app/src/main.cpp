#include "autoconf.h"

#ifdef CONFIG_MODULE_1
#include "module_1.hpp"
#elif CONFIG_MODULE_2
#include "module_2.hpp"
#endif

#include <iostream>

int main() {
  std::cout << "App 1" << '\n';
  Module2 obj;
}