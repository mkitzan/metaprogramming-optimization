#pragma once

#include <stdexcept>

template<typename CharT, std::size_t N>
class cexpr_string {
public:
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

    constexpr const CharT* begin() const noexcept
    {
        return string_;
    }

    constexpr const CharT* end() const noexcept
    {
        return &string_[size_];
    }

    constexpr CharT const& operator[](std::size_t i) const noexcept
    {
        return string_[i];
    }

    constexpr CharT* next_token() noexcept
    {
        const auto curr{ &string_[token_] };
        
        if (token_ != capacity())
        {
            auto ch{ string_[token_] };
            while(ch != CharT{ ' ' } || token_ != capacity())
            {
                ch = string_[++token_];
            }

            string_[token_] = CharT{ '\0' };
            
            if (token_ != capacity())
            {
                ++token_;
            }
        }

        return curr;
    }

private:
    constexpr cexpr_string() : size_{ 0 }, string_{ 0 }, token_{ 0 } {}

    CharT string_[N];
    std::size_t size_;
    std::size_t token_;
};

template<typename CharT, std::size_t N>
cexpr_string(const CharT[N]) -> cexpr_string<CharT, N>;
