#pragma once

#include <cstddef>

template<typename CharT, std::size_t N>
class cexpr_string {
public:
    constexpr cexpr_string() : size_{ 0 }, string_{ 0 } {}

    constexpr cexpr_string(const CharT(&s)[N]) : cexpr_string{}
    {
        for(size_ = 0; s[size_]; ++size_)
        {
            string_[size_] = s[size_];
        }
    }

    constexpr std::size_t capacity() const noexcept
    { 
        return N - 1;
    }

    constexpr std::size_t size() const noexcept
    {
        return size_;
    }

    constexpr CharT* begin() noexcept
    {
        return string_;
    }
    constexpr const CharT* cbegin() const noexcept
    {
        return string_;
    }

    constexpr CharT* end() noexcept
    {
        return &string_[size_];
    }
    constexpr const CharT* cend() const noexcept
    {
        return &string_[size_];
    }

    constexpr CharT& operator[](std::size_t i) noexcept
    {
        return string_[i];
    }
    constexpr CharT const& operator[](std::size_t i) const noexcept
    {
        return string_[i];
    }

private:
    CharT string_[N];
    std::size_t size_;
};

template<typename CharT, std::size_t N>
cexpr_string(const CharT[N]) -> cexpr_string<CharT, N>;
