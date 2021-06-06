# cpp-result

A C++ port of Rust's [std::result](https://doc.rust-lang.org/std/result/).

## Example

```cpp
#include <result/result.h>

zmc::Result<std::string> read_file(const std::string &file_path) {
  std::ifstream istrm{file_path, std::ios::in};
  if (!istrm.is_open()) {
    return {zmc::Error{"Cannot read file."}, ""};
  }

  std::string contents;
  istrm >> contents;

  return contents;
}

int main() {
  std::string file_contents =
      read_file("data/file.txt").expect([](const auto &err) {
        std::cerr << err.message << '\n';
        exit(-1);
      });

  return 0;
}
```
