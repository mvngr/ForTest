#include <iostream>
#include <map>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <initializer_list>
#include <string>

#define TUP_GET(q, a) std::get<q.getIndexOfColumn(a)>(q.getTuple());

/*!
 * \brief Неизменяемая строка
 */
class StringView
{
public:
    template<std::size_t N>
    constexpr StringView(const char(&inString)[N]) : str_{inString}, length_{N - 1} {}

    constexpr StringView(): str_(nullptr), length_(0){}

    constexpr StringView(const StringView &value) : str_(value.str_), length_(value.length_){}

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

template<typename... T>
class TupleClass
{
public:

    constexpr TupleClass(const StringView (&columns)[sizeof...(T)]) :
    query_(""),
    columns_(columns)
    {}

    constexpr std::size_t getIndexOfColumn(const StringView &element, const std::size_t currentValue = 0)
    {
        return (currentValue < (sizeof...(T)) ?
                    (columns_[currentValue] == element ?
                         currentValue
                     :
                         getIndexOfColumn(element, 1 + currentValue)
                    )
                :
                    throw std::logic_error("Не удалось найти элемент!")
               );
    }

    constexpr const std::tuple<T...> &getTuple(){return tuple_;}

private:
    std::tuple<T...> tuple_;
    StringView query_;
    const StringView columns_[sizeof...(T)];

};


template<typename T, std::size_t N>
constexpr const T *findElement(const T (&array)[N], const T &element, const std::size_t currentValue = 0)
{
    return (currentValue < N ?
                (array[currentValue] == element ?
                     &array[currentValue]
                 :
                     findElement(array, element, 1 + currentValue)
                )
            :
                throw std::logic_error("Не удалось найти элемент!")
           );
}

int main()
{
    constexpr StringView s ("sss");
    constexpr StringView a ("ss2s");

    constexpr const StringView array[] {"one", "two", "three", "four", "five"};
    constexpr const StringView found(*findElement(array, StringView("four")));
    //тест корректного наполнения и сравнения классов
    std::cout << found << std::endl;

    constexpr const StringView columns[] {"qq", "ww", "dd"};
    constexpr const TupleClass<int, double, int> q(columns);
    const auto a11 = std::get<q.getIndexOfColumn("dd")>(q.getTuple());
    std::cout << a11 << std::endl;
    auto ggg = TUP_GET(q, "qq");

    return 0;
}
