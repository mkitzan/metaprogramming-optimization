#pragma once

#include <cstddef>

namespace cexpr
{

    template <typename CharT, std::size_t N>
    class string
    {
    public:
        constexpr string() : size_{ 0 }, string_{ 0 } {}

        constexpr string(const CharT(&s)[N]) : string{}
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

        template <typename OtherCharT, std::size_t OtherN>
        constexpr bool operator==(string<OtherCharT, OtherN> const& other) const
        {
            if constexpr (N != OtherN)
            {
                return false;
            }

            std::size_t i{};
            for (; i < N && string_[i] == other[i]; ++i);

            return i == N;
        }

    private:
        CharT string_[N];
        std::size_t size_;
    };

    template <typename CharT, std::size_t N>
    string(const CharT[N]) -> string<CharT, N>;

} // namespace cexpr
