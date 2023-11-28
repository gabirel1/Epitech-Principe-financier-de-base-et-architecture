#pragma once

#include <concepts>

template<class T>
concept IsKey = requires (T _val) {
    _val == _val;
};

namespace fix
{
    template<IsKey T, class _T>
    class InsertMap
    {
        public:
            InsertMap() = default;
            ~InsertMap() = default;

            using Pair = std::pair<T, _T>;
            using Iterator = std::vector<Pair>::iterator;
            using ConstIterator = std::vector<Pair>::const_iterator;

            [[nodiscard]] bool contains(const T &_key) const;
            [[nodiscard]] Iterator find(const T &_key) const;
            void emplace(const Pair &&_pair);

            [[nodiscard]] _T &at(const T &_key);
            [[nodiscard]] const _T &at(const T &_key) const;

            [[nodiscard]] constexpr size_t size() const;

            Iterator begin();
            Iterator end();

            ConstIterator begin() const;
            ConstIterator end() const;

        private:
            // check for reimplementation of a hash map
            std::vector<Pair> m_map;
    };
}

#include "Container/InsertMap.inl"