#ifndef B64_H
#define B64_H

namespace Base64 {
  std::string Encode(const std::string& data);
  std::string Decode(const std::string& encoded);
}

#endif
