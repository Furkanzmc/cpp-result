#include "result/result.h"

#include <doctest/doctest.h>

#include <fstream>
#include <iostream>

struct Point {
  int x;
  int y;
};

struct MyError {
  enum class Type { InvalidPoint };

  Type type;
};

zmc::Result<int> to_int(const std::string &str) {
  try {
    return {std::stoi(str.c_str())};
  } catch (std::invalid_argument &) {
    return {zmc::Error{std::string{"Cannot convert '"} + str + "' to int."},
            -1};
  } catch (std::out_of_range &) {
    return {zmc::Error{std::string{"Cannot convert '"} + str + "' to int."},
            -1};
  }
}

zmc::Result<Point> to_point(const std::string &str) {
  try {
    return {Point{std::stoi(str.c_str()), std::stoi(str.c_str())}};
  } catch (std::invalid_argument &) {
    return {zmc::Error{std::string{"Cannot convert '"} + str + "' to int."},
            Point{-1, -1}};
  } catch (std::out_of_range &) {
    return {zmc::Error{std::string{"Cannot convert '"} + str + "' to int."},
            Point{-1, -1}};
  }
}

zmc::Result<std::string, MyError> from_point(const Point &pt) {
  if (pt.x == -1 || pt.y == -1) {
    return {MyError{MyError::Type::InvalidPoint}, ""};
  }

  return {std::to_string(pt.x) + "," + std::to_string(pt.y)};
}

zmc::Result<std::string> read_file(const std::string &file_path) {
  std::ifstream istrm{file_path, std::ios::in};
  if (!istrm.is_open()) {
    return {zmc::Error{"Cannot read file."}, ""};
  }

  std::string contents;
  istrm >> contents;

  return contents;
}

TEST_CASE("Result with Primitives") {
  int number = to_int("2").expect([](const zmc::Error &err) {});
  CHECK(number == 2);

  number = to_int("a2a").expect([](const zmc::Error &err) {});
  CHECK(number == -1);

  try {
    number = to_int("a2a").expect(nullptr);
    CHECK(false);
  } catch (const std::invalid_argument &) {
    CHECK(true);
  }

  std::string file_contents =
      read_file("data/file.txt").expect([](const auto &err) {
        CHECK(false);
      });
  CHECK(!file_contents.empty());
}

TEST_CASE("Result with Compund Types") {
  Point p = to_point("2").expect([](const zmc::Error &err) {});
  CHECK(p.x == 2);
  CHECK(p.y == 2);

  std::string pstr = from_point(p).expect([](const MyError &err) {
    if (err.type == MyError::Type::InvalidPoint) {
      std::cerr << "InvalidPoint error.\n";
    } else {
      std::cerr << "Unknown error.\n";
    }
  });
  CHECK(pstr == "2,2");

  p = to_point("a2a").expect([](const zmc::Error &err) {});
  CHECK(p.x == -1);
  CHECK(p.y == -1);

  try {
    p = to_point("a2a").expect(nullptr);
    CHECK(false);
  } catch (const std::invalid_argument &) {
    CHECK(true);
  }

  pstr = from_point(p).expect([](const MyError &err) {
    if (err.type == MyError::Type::InvalidPoint) {
      std::cerr << "InvalidPoint error.\n";
    } else {
      std::cerr << "Unknown error.\n";
    }
  });
  CHECK(pstr == "");
}
