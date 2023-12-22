#include "Common/Container/InsertMap.hpp"

namespace fix
{
    template<IsKey T, class _T>
    InsertMap<T, _T>::InsertMap(const InsertMap<T, _T> &_map)
        : m_mutex(), m_map(_map.m_map)
    {
    }

    template<IsKey T, class _T>
    InsertMap<T, _T> &InsertMap<T, _T>::operator=(const InsertMap<T, _T> &_map)
    {
        m_map = _map.m_map;
        return *this;
    }

    template<IsKey T, class _T>
    bool InsertMap<T, _T>::contains(const T &_key)
    {
        return find(_key) == m_map.end();
    }

    template<IsKey T, class _T>
    InsertMap<T, _T>::Iterator InsertMap<T, _T>::find(const T &_key)
    {
        // std::lock_guard<std::mutex> guard(m_mutex);

        return std::find_if(m_map.begin(), m_map.end(), [_key] (const Pair &_mkey) {
            return _key == _mkey.first;
        });
    }

    template<IsKey T, class _T>
    void InsertMap<T, _T>::emplace(const Pair &&_pair)
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        m_map.emplace_back(std::move(_pair));
    }

    template<IsKey T, class _T>
    _T &InsertMap<T, _T>::at(const T &_key)
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

    template<IsKey T, class _T>
    void InsertMap<T, _T>::clear()
    {
        std::lock_guard<std::mutex> guard(m_mutex);

        m_map.clear();
    }
}