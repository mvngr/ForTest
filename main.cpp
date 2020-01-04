#include <iostream>
#include <map>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <initializer_list>

/*!
 * \brief Неизменяемая строка
 */
class StringView
{
public:
    template<std::size_t N>
    constexpr StringView(const char(&inString)[N]) : str_{inString}, length_{N - 1} {}

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

/*!
 *  \brief неймспейс для всех классов, относящихся к дереву
 */
namespace Tree
{
    /*!
     * \brief Перечисление для удобной работы с красным и черным типом через boolean
     */
    enum Colors{
        RED = false,
        BLACK
    };

    /*!
     * \brief Нода для красно-черного дерева
     */
    template<typename T>
    class Node
    {
    public:
        constexpr Node(T inData) :
            data_(inData),
            color_(Colors::RED),
            left_(nullptr),
            right_(nullptr),
            parent_(nullptr)
        {}
    private:
        T data_;
        bool color_;
        Node<T> *left_, *right_, *parent_;
    };

    /*!
     * \brief Красно-черное дерево
     */
    template<typename T>
    class RBTree
    {

    };
}

int main()
{
    constexpr StringView s ("sss");
    constexpr StringView a ("ss2s");

    //тест корректного наполнения и сравнения классов
    std::cout << s.size() << " " << a.size() << " " << (s == a ? "true" : "false") << std::endl;

    constexpr Tree::Node<StringView> node("sssss"); //сейчас переменная только для теста инициализации


    return 0;
}
