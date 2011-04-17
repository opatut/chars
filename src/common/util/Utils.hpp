#ifndef UTIL_UTILS_HPP
#define UTIL_UTILS_HPP

#include <string>
#include <boost/lexical_cast.hpp>

template <typename Source> std::string tostr(const Source& source) {
    return boost::lexical_cast<std::string>(source);
}

#endif
