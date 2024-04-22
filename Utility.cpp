//
// Created by Mac on 05.12.2023.
//

#include "Utility.h"
#include "algorithm"
#include "vector"
#include "string"
#include "set"

using std::string;
using std::vector;
using std::set;

template<typename T, typename U>
bool Contains(const T& container, const U& object) {
  return std::find(container.begin(), container.end(), object) != container.end();
}

template<typename T, typename U>
bool Intersects(const T& container1, const U& container2) {
  for (const auto& elem : container1) {
    if (Contains(container2, elem)) {
      return true;
    }
  }
  return false;
}

template
bool Intersects(const vector<char>&, const vector<char>&);

template
bool Intersects(const string&, const set<char>&);