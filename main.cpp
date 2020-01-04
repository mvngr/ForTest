#include <iostream>
#include <map>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <initializer_list>

class StringView
{
public:
    //constexpr StringView(const char *inString) : str_(inString), length_(sizeof(inString) / sizeof(char)) { }
    template<std::size_t N>
    constexpr StringView(const char(&inString)[N]) noexcept : str_{inString}, length_{N - 1} {}

    constexpr std::size_t size() const
    {
        return length_;
    }

    constexpr char operator[](std::size_t i) const
    {
        return i < length_ ? str_[i] : throw std::out_of_range{""};
    }

    constexpr operator const char*() const
    {
        return str_;
    }

    constexpr bool operator < (const StringView &other) const
    {
        return less_impl(str_, length_, other.str_, other.length_);
    }

    constexpr bool operator == (const StringView &other) const
    {
        return equal_impl(str_, length_, other.str_, other.length_);
    }

private:
    const char* str_;
    const std::size_t length_;

    constexpr bool less_impl(const char* s1, std::size_t n1, const char* s2, std::size_t n2) const
    {
        return n2 == 0 ? false :
          n1 == 0 ? true :
          *s1 == *s2 ? less_impl(s1 + 1, n1 - 1, s2 + 1, n2 - 1) :
             *s1 < *s2;
    }

    constexpr bool equal_impl(const char *s1, size_t n1, const char *s2, size_t n2) const
    {
        return n1 != n2 ?
                    false
               :
                    n1 == 0 ? true :
                        *s1 == *s2 ? equal_impl(s1 + 1, n1 - 1, s2 + 1, n2 - 1) :
                            *s1 == *s2;

    }
};



int main()
{
    constexpr StringView s ("sss");
    constexpr StringView a ("ss2s");

    std::cout << s.size() << " " << a.size() << " " << (s == a ? "true" : "false") << std::endl;



    return 0;
}
