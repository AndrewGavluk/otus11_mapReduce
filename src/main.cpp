#include <iostream>

int main(){

  auto port = 9000;
  if((port <= 0) || (port > 65535)) {
    std::cerr << "Port must be an integer between 0 and 65535.\n";
    return EXIT_FAILURE;
  }

    try {

    } 
    catch(std::exception& e) {

    }

    return 0;
}
