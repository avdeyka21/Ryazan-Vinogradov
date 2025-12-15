#pragma once

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Вспомогательная функция для сравнения unordered_map независимо от порядка элементов
template <class K, class V>
bool unordered_maps_equal(const unordered_map<K, V>& m1, const unordered_map<K, V>& m2) {
  if (m1.size() != m2.size()) return false;

  for (const auto& kv : m1) {
    auto it = m2.find(kv.first);
    if (it == m2.end()) return false;
    if (!(it->second == kv.second)) return false;
  }
  return true;
}

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const unordered_map<K, V>& m) {
  // Сначала собираем ключи в вектор и сортируем для детерминированного вывода
  vector<K> keys;
  for (const auto& kv : m) {
    keys.push_back(kv.first);
  }
  sort(keys.begin(), keys.end());

  os << "{";
  bool first = true;
  for (const auto& key : keys) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << key << ": " << m.at(key);
  }
  return os << "}";
}

// Специальная версия AssertEqual для unordered_map
template <class K, class V>
void AssertEqual(const unordered_map<K, V>& t, const unordered_map<K, V>& u, const string& hint = {}) {
  if (!unordered_maps_equal(t, u)) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

// Общая версия AssertEqual для других типов
template <class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
  if (!(t == u)) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

inline void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
      try {
        func();
        cerr << test_name << " OK" << endl;
        tests_count++;
      } catch (exception& e) {
        ++fail_count;
        cerr << test_name << " fail: " << e.what() << endl;
      } catch (...) {
        ++fail_count;
        cerr << test_name << ": Unknown exception caught" << endl;
      }
    }

    ~TestRunner() {
      cout << "=============================================" << endl;
      cout << "Total tests run: " << tests_count << endl;
      cout << "Tests passed: " << tests_count - fail_count << endl;
      if (fail_count > 0) {
        cout << "Tests failed: " << fail_count << endl;
        cerr << fail_count << " unit tests failed. Terminate" << endl;
        exit(1);
      } else {
        cout << "All tests passed!" << endl;
      }
    }

    unsigned GetFailCount() const { return fail_count; }
    unsigned GetTestsCount() const { return tests_count; }

private:
    unsigned fail_count = 0;
    unsigned tests_count = 0;
};

// Макрос для сравнения unordered_map
#define ASSERT_UNORDERED_MAP_EQUAL(x, y) {                           \
    ostringstream __assert_unordered_map_equal_private_os;           \
    __assert_unordered_map_equal_private_os                          \
        << #x << " != " << #y << ", "                                \
        << __FILE__ << ":" << __LINE__;                              \
    if (!unordered_maps_equal((x), (y))) {                           \
        ostringstream os;                                            \
        os << "Assertion failed: " << (x) << " != " << (y)           \
           << ", " << __assert_unordered_map_equal_private_os.str(); \
        throw runtime_error(os.str());                               \
    }                                                                \
}

#define ASSERT_EQUAL(x, y) {                          \
    ostringstream __assert_equal_private_os;          \
    __assert_equal_private_os                         \
        << #x << " != " << #y << ", "                 \
        << __FILE__ << ":" << __LINE__;               \
    AssertEqual(x, y, __assert_equal_private_os.str()); \
}

#define ASSERT(x) {                                   \
    ostringstream os;                                 \
    os << #x << " is false, "                         \
        << __FILE__ << ":" << __LINE__;               \
    Assert(x, os.str());                              \
}

#define RUN_TEST(tr, func)                           \
    tr.RunTest(func, #func)