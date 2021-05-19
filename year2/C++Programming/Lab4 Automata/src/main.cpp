// Copyright 2020 Alexey Klimov
#include <iostream>

#include "Automata.h"

int main() {
  Automata a(std::cout);

  // start machine
  a.on();

  // load menu
//  a.loadFromFile("menu.txt");

  // show menu
  a.writeMenu();

  // Add money
  a.coin(100);

  // Choose position
  a.choice(2);
  std::cout << "Your choice is: \"" << a.getMenu(2) << '\"';

  // Cook chosen position
  a.cook();
  a.finish();

  // Do refund
  int money = a.refund();

  // Turn off machine
  a.off();
}
