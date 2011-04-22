#ifndef UTIL_UTILS_HPP
#define UTIL_UTILS_HPP

#include <string>
#include <boost/lexical_cast.hpp>
#include <openssl/sha.h>

template <typename Source> std::string tostr(const Source& source) {
    return boost::lexical_cast<std::string>(source);
}

std::string sha(const std::string& in);

#endif
