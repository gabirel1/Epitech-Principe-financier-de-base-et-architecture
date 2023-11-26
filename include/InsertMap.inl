#include "InsertMap.hpp"

namespace fix
{
    template<IsKey T, class _T>
    bool InsertMap<T, _T>::contains(const T &_key) const
    {
        return find(_key) == m_map.end();
    }

    template<IsKey T, class _T>
    InsertMap<T, _T>::Iterator InsertMap<T, _T>::find(const T &_key) const
    {
        return std::find_if(m_map.begin(), m_map.end(), [_key] (const T &_mkey) {
            return _key == _mkey;
        });
    }

    template<IsKey T, class _T>
    void InsertMap<T, _T>::emplace(const Pair &&_pair)
    {
        m_map.emplace_back(std::move(_pair));
    }

    template<IsKey T, class _T>
    typename _T &InsertMap<T, _T>::at(const T &_key)
    {
        return find(_key)->second;
    }

    template<IsKey T, class _T>
    const _T &InsertMap<T, _T>::at(const T &_key) const
    {
        return find(_key)->second;
    }

    template<IsKey T, class _T>
    constexpr size_t InsertMap<T, _T>::size() const
    {
        return m_map.size();
    }

    template<IsKey T, class _T>
    InsertMap<T, _T>::Iterator InsertMap<T, _T>::begin()
    {
        return m_map.begin();
    }

    template<IsKey T, class _T>
    InsertMap<T, _T>::Iterator InsertMap<T, _T>::end()
    {
        return m_map.end();
    }

    template<IsKey T, class _T>
    InsertMap<T, _T>::ConstIterator InsertMap<T, _T>::begin() const
    {
        return m_map.begin();
    }

    template<IsKey T, class _T>
    InsertMap<T, _T>::ConstIterator InsertMap<T, _T>::end() const
    {
        return m_map.end();
    }
}