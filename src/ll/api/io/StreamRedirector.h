#pragma once

#include <functional>
#include <iostream>
#include <utility>

namespace ll {

template <class Char = char, class Traits = std::char_traits<Char>>
struct StreamRedirector {
    StreamRedirector(const StreamRedirector&)            = delete;
    StreamRedirector& operator=(const StreamRedirector&) = delete;

    StreamRedirector(std::basic_ostream<Char, Traits>& stream, std::basic_streambuf<Char, Traits>* rdbuf)
    : stream(stream),
      rdbuf_old(stream.rdbuf()) {
        stream.set_rdbuf(rdbuf);
    }

    ~StreamRedirector() { stream.set_rdbuf(rdbuf_old); }

private:
    std::basic_ostream<Char, Traits>&   stream;
    std::basic_streambuf<Char, Traits>* rdbuf_old;
};
} // namespace ll
