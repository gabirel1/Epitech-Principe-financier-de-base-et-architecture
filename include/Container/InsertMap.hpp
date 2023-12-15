#pragma once

#include <concepts>
#include <mutex>
#include <vector>

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
            using Pair = std::pair<T, _T>;
            using Iterator = std::vector<Pair>::iterator;
            using ConstIterator = std::vector<Pair>::const_iterator;

            InsertMap() = default;
            InsertMap(const InsertMap<T, _T> &_map);
            ~InsertMap() = default;

            InsertMap &operator=(const InsertMap<T, _T> &_map);

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
            std::mutex m_mutex;

            // check for reimplementation of a hash map
            std::vector<Pair> m_map;
    };
}

#include "Container/InsertMap.inl"