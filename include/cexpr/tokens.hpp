#pragma once

#include <array>
#include <cstddef>

#include "cexpr/string.hpp"

namespace cexpr
{

	template<typename CharT, std::size_t Tl, std::size_t Tn>
	class tokens
	{
	public:
		using token_string = string<CharT, Tl>;

		template<std::size_t N>
		constexpr tokens(string<CharT, N> const& cs) : tokens_{}
		{
			auto begin{ cs.cbegin() }, end{ cs.cend() };
			std::size_t pos{}, i{};

			while (begin < end)
			{
				if (*begin == CharT{ ' ' })
				{
					tokens_[pos][i] = CharT{ '\0' };
					++pos;
					i = 0;
				}
				else
				{
					tokens_[pos][i] = *begin;
					++i;
				}

				++begin;
			}

			tokens_[pos][i] = CharT{ '\0' };
		}

		constexpr std::size_t size() const noexcept
		{
			return Tl;
		}

		constexpr std::size_t count() const noexcept
		{
			return Tn;
		}

		constexpr token_string* begin() noexcept
		{
			return tokens_.begin();
		}
		constexpr const token_string* cbegin() const noexcept
		{
			return tokens_.cbegin();
		}

		constexpr token_string* end() noexcept
		{
			return tokens_.end();
		}
		constexpr const token_string* cend() const noexcept
		{
			return tokens_.cend();
		}

		constexpr token_string& operator[](std::size_t i) noexcept
		{
			return tokens_[i];
		}
		constexpr token_string const& operator[](std::size_t i) const noexcept
		{
			return tokens_[i];
		}

	private:
		std::array<token_string, Tn> tokens_;
	};

	template<typename CharT, std::size_t N>
	constexpr std::size_t tcount(string<CharT, N> const& cs)
	{
		auto begin{ cs.cbegin() }, end{ cs.cend() };
		std::size_t count{ 1 };

		while (begin < end)
		{
			if (*begin == CharT{ ' ' })
			{
				++count;
			}
			++begin;
		}

		return count;
	}

	template<typename CharT, std::size_t N>
	constexpr std::size_t tlength(string<CharT, N> const& cs)
	{
		auto begin{ cs.cbegin() }, end{ cs.cend() };
		std::size_t max{ 1 }, curr{ 1 };

		while (begin < end)
		{
			if (*begin == CharT{ ' ' })
			{
				if (curr > max)
				{
					max = curr;
				}
				curr = 1;
			}
			else
			{
				++curr;
			}

			++begin;
		}

		if (curr > max)
		{
			max = curr;
		}

		return max;
	}

} // namespace cexpr
