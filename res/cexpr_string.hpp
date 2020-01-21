#pragma once

#include <stdexcept>

template<typename CharT, std::size_t N>
class cexpr_string {
public:
    using value_type = CharT;

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
        return N;
    }

    constexpr std::size_t size() const noexcept
    {
        return size_;
    }

    constexpr CharT* begin() noexcept
    {
        return string_;
    }
    constexpr const CharT* begin() const noexcept
    {
        return string_;
    }

    constexpr CharT* end() noexcept
    {
        return &string_[size_];
    }
    constexpr const CharT* end() const noexcept
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

    constexpr void push_back(const CharT& x)
    {
        if(size_ == N) 
        {
            throw std::runtime_error("Insufficient capacity");
        }

        string_[size_] = x; 
        string_[++size_] = CharT{ 0 }; 
    }

    constexpr void pop_back()
    {
        if(size_ == 0)
        {
            throw std::runtime_error("No data");
        }
        
        string_[--size_] = CharT{ 0 }; 
    }

    constexpr void clear() noexcept
    {
        while(size_ > 0) 
        {
            string_[--size_] = CharT{ 0 };
        }
    }

    constexpr CharT* next_token() noexcept
    {
        auto curr{ token_ };
        
        if (token_ != end())
        {
            while(*token_ != CharT{ ' ' })
            {
                ++token_;
            }

            *token_ = CharT{ '\0' };
            
            if (token_ != end())
            {
                ++token_;
            }
        }

        return curr;
    }

private:
    CharT string_[N + 1];
    std::size_t size_;
    CharT* token_{ &string_ };
};

template<typename CharT, std::size_t N>
cexpr_string(const CharT[N]) -> cexpr_string<CharT, N>;
