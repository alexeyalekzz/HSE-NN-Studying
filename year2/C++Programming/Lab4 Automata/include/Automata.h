// Copyright 2020 Alexey Klimov
#ifndef INCLUDE_AUTOMATA_H
#define INCLUDE_AUTOMATA_H

#include <string>
#include <map>
#include <utility>

class Automata {
 public:
  enum STATES { OFF = 0, WAIT, ACCEPT, CHECK, COOK };
  explicit Automata(std::ostream &);
  explicit Automata(std::ostream &, const char *);
  void on();
  void off();
  void coin(unsigned int);
  void writeMenu() const;
  [[nodiscard]] std::string getMenu();
  [[nodiscard]] std::string getMenu(int);
  [[nodiscard]] std::string getState() const;
  void choice(int);
  void cancel();
  void cook();
  void finish();
  int refund();
  void loadFromFile(const char *);
  void addToMenu(const std::string&);
 private:
  int cash;
  int current_id;
  std::map<int, std::pair<std::string, int>> menu;
  STATES state;
  std::ostream &out;

  bool check(int);
};

#endif  // INCLUDE_AUTOMATA_H
