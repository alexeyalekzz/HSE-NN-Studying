// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 29.01.2021.
//

#ifndef INCLUDE_GROUP_H_
#define INCLUDE_GROUP_H_

#include <string>
#include <vector>

class Student;
class Deanary;

class Group {
 public:
  double get_average_mark() const;
  bool contains_student(uint64_t) const;
  bool is_empty() const;

  friend class Student;
  friend class Deanary;

 private:
  Student* get_student(uint64_t) const;
  explicit Group(std::string);
  Student* create_student(uint64_t, const std::string&);
  void add_student(Student*);
  void choose_head();
  void remove_student(Student*);

  std::string title;
  std::vector<Student*> students;
  Student* head;
};

#endif  // INCLUDE_GROUP_H_
