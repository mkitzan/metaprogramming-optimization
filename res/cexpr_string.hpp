#include <stdexcept>

template<typename T, std::size_t N>
class cexpr_string {
public:
    constexpr cexpr_string() : size_{ 0 }, string_{ 0 }
    {}

    constexpr cexpr_string(const T(&s)[N]) : cexpr_string{}
    {
        for(size_ = 0; s[size_]; ++size_)
        {
            string_[size_] = s[size_];
        }
    }

    constexpr cexpr_string(cexpr_string const&) = default;
    constexpr cexpr_string& operator=(cexpr_string const&) = default;
    ~cexpr_string() = default;

    constexpr std::size_t capacity() const noexcept
    { 
        return N;
    }

    constexpr std::size_t size() const noexcept
    {
        return size_;
    }

    constexpr T* begin() noexcept
    {
        return string_;
    }
    constexpr const T* begin() const noexcept
    {
        return string_;
    }

    constexpr T* end() noexcept
    {
        return &string_[size_];
    }
    constexpr const T* end() const noexcept
    {
        return &string_[size_];
    }

    constexpr T& operator[](std::size_t i) noexcept
    {
        return string_[i];
    }
    constexpr T const& operator[](std::size_t i) const noexcept
    {
        return string_[i];
    }

    constexpr void push_back(const T& x)
    {
        if(size_ == N) 
        {
            throw std::runtime_error("Insufficient capacity");
        }

        string_[size_] = x; 
        string_[++size_] = T{ 0 }; 
    }

    constexpr void pop_back()
    {
        if(size_ == 0)
        {
            throw std::runtime_error("No data");
        }
        
        string_[--size_] = T{ 0 }; 
    }

    constexpr void clear() noexcept
    {
        while(size_ > 0) 
        {
            string_[--size_] = T{ 0 };
        }
    }

private:
    T string_[N + 1];
    std::size_t size_;
};

template<typename T, std::size_t N>
cexpr_string(const T[N]) -> cexpr_string<T, N>;
