#pragma once

namespace deadrop
{
    template<class T1, class T2>
    class pair
    {
    public:
        using first_type = T1;
        using second_type = T2;

        // default constructor
        pair() = default;

        // construct a pair from the given arguments
        constexpr pair(T1 _first, T2 _second) : first(_first), second(_second) {}

        // holds the data of the first and second type
        T1 first;
        T2 second;
    };

    // true if both lhs.first == rhs.first and lhs.second == rhs.second, otherwise false
    template<class T1, class T2>
    constexpr bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    // !(lhs == rhs)
    template<class T1, class T2>
    constexpr bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    {
        return !(lhs == rhs);
    }

    // If lhs.first < rhs.first, returns true. Otherwise, if rhs.first < lhs.first, returns false. 
    // Otherwise, if lhs.second < rhs.second, returns true. Otherwise, returns false.
    template<class T1, class T2>
    constexpr bool operator<(const pair<T1, T2> & lhs, const pair<T1, T2> & rhs)
    {
        return lhs.first < rhs.first || !(rhs.first < lhs.first) && lhs.second < rhs.second;
    }

    // rhs < lhs
    template<class T1, class T2>
    constexpr bool operator>(const pair<T1, T2> & lhs, const pair<T1, T2> & rhs)
    {
        return rhs < lhs;
    }

    // Creates a pair object, deducing the target type from the types of arguments.
    // The deduced types V1 and V2 are std::decay<T1>::type and std::decay<T2>::type 
    // (the usual type transformations applied to arguments of functions passed by value)
    // unless application of std::decay results in std::reference_wrapper<X> for some type X,
    // in which case the deduced type is X&.
    // TODO: re-implement make_pair() as the specification requires
    template<class T1, class T2>
    pair<T1, T2> make_pair(T1 t, T2 u)
    {
        return pair{ t, u };
    }
}