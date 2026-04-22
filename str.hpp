// Implementation of a simple string class `str` per assignment requirements
#pragma once

#include <algorithm>
#include <cstring>
#include <vector>

class str {
private:
    char* data_;
    size_t len_;

    void allocate_and_copy(const char* s, size_t n) {
        len_ = n;
        data_ = new char[len_ + 1];
        if (n) std::memcpy(data_, s, n);
        data_[len_] = '\0';
    }

public:
    // Default constructor: empty string
    str() : data_(nullptr), len_(0) { allocate_and_copy("", 0); }

    // Construct from single character
    explicit str(const char& c) : data_(nullptr), len_(0) { allocate_and_copy(&c, 1); }

    // Construct from const char* (accept both lvalue and rvalue)
    explicit str(const char* s) : data_(nullptr), len_(0) {
        const char* p = s ? s : "";
        allocate_and_copy(p, std::strlen(p));
    }

    // Also provide rvalue-overload to satisfy given signature
    explicit str(const char*&& s_) : str(static_cast<const char*>(s_)) {}

    // Assign from const char* (copy contents)
    str& operator=(const char* s) {
        const char* p = s ? s : "";
        if (p == data_) return *this;
        delete[] data_;
        allocate_and_copy(p, std::strlen(p));
        return *this;
    }

    // Assign from rvalue const char* to satisfy given signature
    str& operator=(const char*&& s_) { return (*this = static_cast<const char*>(s_)); }

    // Copy constructor
    str(const str& other) : data_(nullptr), len_(0) { allocate_and_copy(other.data_, other.len_); }

    // Copy assignment
    str& operator=(const str& other) {
        if (this == &other) return *this;
        delete[] data_;
        allocate_and_copy(other.data_, other.len_);
        return *this;
    }

    // Element access (0-based)
    char& operator[](size_t pos) { return data_[pos]; }

    // Length accessor
    size_t len() const { return len_; }

    // Join: concatenate strings in `strs` separated by current instance
    str join(const std::vector<str>& strs) const {
        if (strs.empty()) return str();
        size_t total = 0;
        for (const auto& s : strs) total += s.len_;
        size_t sep_len = len_;
        if (strs.size() > 1) total += sep_len * (strs.size() - 1);

        str result;
        delete[] result.data_;
        result.allocate_and_copy("", total);

        char* p = result.data_;
        for (size_t i = 0; i < strs.size(); ++i) {
            const str& s = strs[i];
            if (s.len_) {
                std::memcpy(p, s.data_, s.len_);
                p += s.len_;
            }
            if (i + 1 < strs.size() && sep_len) {
                std::memcpy(p, data_, sep_len);
                p += sep_len;
            }
        }
        *p = '\0';
        return result;
    }

    // Slice [l, r)
    str slice(size_t l, size_t r) const {
        if (l >= r || l >= len_) return str();
        r = std::min(r, len_);
        size_t n = r - l;
        str result;
        delete[] result.data_;
        result.allocate_and_copy(data_ + l, n);
        return result;
    }

    // Destructor
    ~str() { delete[] data_; }
};
