#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <TreeMap.h>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
private:
  std::vector<TreeMap<KeyType,ValueType>> wektor;
  size_t size;
  const unsigned int BUCKETS=50;

  int h(const KeyType key) const
  {
    return key%BUCKETS;
  }

public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  friend class ConstIterator;

  HashMap()
  {
    size=0;
    wektor.reserve(BUCKETS);
    wektor.resize(BUCKETS);
  }

  ~HashMap()
  {
    while(size>0)
        remove(begin());
  }

  HashMap(std::initializer_list<value_type> list)
  {
    size=0;
    wektor.reserve(BUCKETS);
    wektor.resize(BUCKETS);
    for(auto iter=list.begin();iter!=list.end();iter++)
        (*this)[(*iter).first]=(*iter).second;
  }

  HashMap(const HashMap& other)
  {
    size=other.size;
    wektor=other.wektor;
  }

  HashMap(HashMap&& other)
  {
    size=other.size;
    wektor=other.wektor;
    other.size=0;
    other.wektor.clear();
    other.wektor.reserve(BUCKETS);
    other.wektor.resize(BUCKETS);
  }

  HashMap& operator=(const HashMap& other)
  {
    if (*this==other)
        return *this;
    while(size>0)
        remove(begin());
    wektor=other.wektor;
    size=other.size;
    return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
    if (*this==other)
        return *this;
    size=other.size;
    wektor=other.wektor;
    other.size=0;
    other.wektor.clear();
    other.wektor.reserve(BUCKETS);
    other.wektor.resize(BUCKETS);
    return *this;
  }

  bool isEmpty() const
  {
    if (size==0)
        return true;
    return false;
  }

  mapped_type& operator[](const key_type& key)
  {
    if (find(h(key))==end())
        size++;
    return wektor[h(key)].operator[](key);
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    if (isEmpty())
        throw std::out_of_range("");
    return wektor[h(key)].valueOf(key);
  }

  mapped_type& valueOf(const key_type& key)
  {
    if (isEmpty())
        throw std::out_of_range("");
    return wektor[h(key)].valueOf(key);
  }

  const_iterator find(const key_type& key) const
  {
    if (isEmpty())
        return end();
    typename TreeMap<KeyType, ValueType>::ConstIterator treeiter=wektor[h(key)].find(key);
    if (treeiter==wektor[h(key)].end())
        return end();
    ConstIterator iter;
    iter.hashmap=this;
    iter.treeiter=treeiter;
    iter.index=h(key);
    return iter;
  }

  iterator find(const key_type& key)
  {
    if (isEmpty())
        return end();
    typename TreeMap<KeyType, ValueType>::ConstIterator treeiter=wektor[h(key)].find(key);
    if (treeiter==wektor[h(key)].end())
        return end();
    Iterator iter;
    iter.hashmap=this;
    iter.treeiter=treeiter;
    iter.index=h(key);
    return iter;
  }

  void remove(const key_type& key)
  {
    wektor[h(key)].remove(key);
    size--;
  }

  void remove(const const_iterator& it)
  {
    remove(it->first);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    if (this->size != other.size)
        return false;
    Iterator iter1 = this->begin();
    Iterator iter2 = other.begin();
    while (iter1!=this->end())
    {
        if (iter1->first != iter2->first || iter1->second != iter2->second)
            return false;
        iter1++;
        iter2++;
    }
    return true;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    Iterator iter;
    iter.hashmap=this;
    if (isEmpty())
    {
        iter.index=BUCKETS;
        iter.treeiter=wektor[BUCKETS-1].end();
        return iter;
    }
    for (int i=0;i<BUCKETS;i++)
    {
        if (!wektor[i].isEmpty())
        {
            iter.index=i;
            iter.treeiter=wektor[i].begin();
        }
    }
    return iter;
  }

  iterator end()
  {
    Iterator iter;
    iter.hashmap=this;
    iter.index=BUCKETS-1;
    iter.treeiter=wektor[BUCKETS-1].end();
    return iter;
  }

  const_iterator cbegin() const
  {
    ConstIterator iter;
    iter.hashmap=this;
    if (isEmpty())
    {
        iter.index=BUCKETS-1;
        iter.treeiter=wektor[BUCKETS-1].end();
        return iter;
    }
    for (int i=0;i<BUCKETS;i++)
    {
        if (!wektor[i].isEmpty())
        {
            iter.index=i;
            iter.treeiter=wektor[i].begin();
        }
    }
    return iter;
  }

   const_iterator cend() const
  {
    ConstIterator iter;
    iter.hashmap=this;
    iter.index=BUCKETS-1;
    iter.treeiter=wektor[BUCKETS-1].end();
    return iter;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
protected:
  const HashMap * hashmap;
  unsigned int index;
  typename TreeMap<KeyType, ValueType>::ConstIterator treeiter;

public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

  friend class HashMap;

  explicit ConstIterator()
  {}

  ConstIterator(const ConstIterator& other)
  {
    hashmap=other.hashmap;
    index=other.index;
    treeiter=other.treeiter;
  }

  ConstIterator& operator++()
  {
    if (!((++treeiter)==hashmap->wektor[index].end()))
    {
        for (index++;index<hashmap->BUCKETS;index++)
        {
            if (!(hashmap->wektor[index].isEmpty()))
            {
                treeiter=hashmap->wektor[index].begin();
                return *this;
            }
        }
        *this=hashmap->end();
    }
     return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator orig=(*this);
    ++(*this);
    return orig;
  }

  ConstIterator& operator--()
  {
    if (*this==hashmap->begin())
        throw std::out_of_range("");
    if (treeiter!=hashmap->wektor[index].begin())
    {
        treeiter--;
        return *this;
    }
    else
    {

        for (index--;index>=0;index--)
        {
            if (!(hashmap->wektor[index].isEmpty()))
            {
                treeiter=(hashmap->wektor[index].end());
                treeiter--;
                break;
            }
        }
    }
    return *this;
  }

  ConstIterator operator--(int)
  {
    if (*this==hashmap->begin())
        throw std::out_of_range("");
    ConstIterator orig=(*this);
    --(*this);
    return orig;
  }

  reference operator*() const
  {
    return *treeiter;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if (treeiter == other.treeiter)
        return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
