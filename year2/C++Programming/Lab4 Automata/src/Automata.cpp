// Copyright 2020 Alexey Klimov
#include <fstream>
#include <iostream>

#include "Automata.h"


std::string parse(const std::string &buf, int *cost) {
  size_t dollar = buf.find('$');
  *cost = std::stoi(buf.substr(dollar + 1, buf.size() - dollar));
  return buf.substr(0, dollar - 1);
}

void Automata::loadFromFile(const char *name) {
  std::ifstream ifile(name);

  for (size_t i{1}; !ifile.eof(); i++) {
    char buf[30];
    ifile.getline(buf, 30);
    int cost;
    std::string pos_name = parse(buf, &cost);
    menu[i].first = pos_name;
    menu[i].second = cost;
  }
  ifile.close();
}

Automata::Automata(std::ostream &stream) : out(stream) {
  cash = 0;
  state = STATES::OFF;
  current_id = -1;
}

Automata::Automata(std::ostream &stream, const char *file) : Automata(stream) {
  this->loadFromFile(file);
}

void Automata::on() {
  if (state == STATES::OFF)
    state = STATES::WAIT;
  else
    out << "Machine already is on!";
}

void Automata::off() {
  if (state == STATES::OFF) {
    out << "Machine is already off!";
  } else if (state == STATES::WAIT) {
    state = STATES::OFF;
  } else {
    out << "Machine is busy, can't turn off! Current state: "
    << this->getState();
  }
}

void Automata::coin(unsigned int c) {
  if (state == STATES::WAIT || state == STATES::ACCEPT) {
    cash += static_cast<int>(c);
    state = STATES::ACCEPT;
  } else {
    out << "Wrong machine state! Current state: " << this->getState();
  }
}

void Automata::choice(const int id) {
  if (state == STATES::ACCEPT || state == STATES::WAIT) {
    state = STATES::CHECK;

    if (!check(id)) {
      out << "Not enough money!";
      state = STATES::WAIT;
    } else {
      current_id = id;
    }

  } else {
    out << "Wrong machine state! Current state: " << this->getState();
  }
}

bool Automata::check(const int id) {
  if (cash - menu[id].second >= 0)
    return true;
  else
    return false;
}

void Automata::cancel() {
  if (state == STATES::CHECK || state == STATES::ACCEPT) {
    state = STATES::WAIT;
  } else {
    out << "Wrong machine state! Current state: " << this->getState();
  }
}

void Automata::cook() {
  if (state == STATES::CHECK) {
    cash -= menu[current_id].second;
    state = STATES::COOK;
  } else {
    out << "Wrong machine state! Current state: " << this->getState();
  }
}

void Automata::finish() {
  if (state == STATES::COOK) {
    state = STATES::WAIT;
    current_id = -1;
  } else {
    out << "Wrong machine state! Current state: " << this->getState();
  }
}

void Automata::writeMenu() const {
  for (auto &item : menu) {
    out << item.first << ' '
        << item.second.first << "  $"
        << item.second.second << std::endl;
  }
}
std::string Automata::getMenu() {
  std::string res;
  for (size_t i{1}; i < menu.size(); i++) {
    std::to_string(i) + ' '
        + menu[i].first
        + " $"
        + std::to_string(menu[i].second)
        + '\n';
  }
  return res;
}
std::string Automata::getMenu(int id) {
  if (id > menu.size() || id < 0)
    return "Bad id";
  return std::string(std::to_string(id) + ' '
                         + menu[id].first
                         + " $"
                         + std::to_string(menu[id].second));
}

std::string Automata::getState() const {
  std::string s;
  switch (state) {
    case STATES::OFF: s = "OFF";
      break;
    case STATES::WAIT: s = "WAIT";
      break;
    case STATES::ACCEPT: s = "ACCEPT";
      break;
    case STATES::CHECK: s = "CHECK";
      break;
    case STATES::COOK: s = "COOK";
      break;
  }
  return s;
}

int Automata::refund() {
  if (cash > 0) {
    int refund = cash;
    cash = 0;
    return refund;
  } else {
    return 0;
  }
}

void Automata::addToMenu(const std::string& str) {
  int cost;
  std::string pos_name = parse(str, &cost);
  size_t i = menu.size() + 1;
  menu[i].first = pos_name;
  menu[i].second = cost;
}
