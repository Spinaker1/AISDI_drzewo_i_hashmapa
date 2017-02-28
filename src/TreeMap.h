#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>


namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
private:

  struct Item
  {
    std::pair<const KeyType, ValueType> * para;

    Item * left;
    Item * right;
    Item * parent;

    Item(KeyType key, ValueType value)
    {
        para = new std::pair <const KeyType, ValueType>(key,value);
    }

    Item()
    {
        left=nullptr;
        right=nullptr;
        parent=nullptr;
        para=nullptr;
    }

    ~Item()
    {
        if (para)
            delete para;
        left=nullptr;
        right=nullptr;
        parent=nullptr;
    }
  };

  Item * root;
  size_t size;

  Item * findSmallest(Item * item) const
  {
   if (item==nullptr) return nullptr;
   for (;item->left!=nullptr;item=item->left);
   return item;
  }

  Item * findLargest(Item * item) const
  {
    if (item==nullptr) return nullptr;
    for (;item->right!=nullptr;item=item->right);
    return item;
  }

  void clean_tree(Item* t)
  {
    if (t->left)
    {
        clean_tree(t->left);
    }
    if (t->right)
    {
        clean_tree(t->right);
    }
    delete t;
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

  friend class Iterator;

  TreeMap()
  {
    root=nullptr;
    size=0;
  }

  ~TreeMap()
  {
    if (!isEmpty())
        clean_tree(root);
  }

  TreeMap(std::initializer_list<value_type> list)
  {
    root=nullptr;
    size=0;
    for(auto iter=list.begin();iter!=list.end();iter++)
        (*this)[(*iter).first]=(*iter).second;
  }

  TreeMap(const TreeMap& other)
  {
    root=nullptr;
    size=0;
    for(auto iter=other.begin();iter!=other.end();iter++)
        (*this)[(*iter).first]=(*iter).second;
  }

  TreeMap(TreeMap&& other)
  {
    this->size=other.size;
    this->root=other.root;
    other.size=0;
    other.root=nullptr;
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if (*this==other)
        return *this;
    while(size>0)
        remove(begin());
    if (other.size==0)
        return *this;
    for(auto iter=other.begin();iter!=other.end();iter++)
        (*this)[iter->first]=iter->second;
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    if (*this==other)
        return *this;
    while(size>0)
        remove(begin());
    this->size=other.size;
    this->root=other.root;
    other.size=0;
    other.root=nullptr;
    return *this;
  }

  bool isEmpty() const
  {
    if (!root) return true;
    return false;
  }


  mapped_type& operator[](const key_type& key)
  {
    if (isEmpty())
    {
        root = new Item(key, {});
        root->parent=nullptr;
        root->left=nullptr;
        root->right=nullptr;
        size++;
        return root->para->second;
    }
    Item * item = root;
    int direction;
    Item * father = item;
    while (item->para->first!=key)
    {
        if (key < item->para->first)
        {
            father=item;
            item=item->left;
            direction=0;
        }
        else
        {
            father=item;
            item=item->right;
            direction=1;
        }
        if (item == nullptr)
        {
            item = new Item(key,{});
            item->parent=father;
            if (direction==0)
            {
                father->left=item;
            }
            else
                father->right=item;
            item->left=nullptr;
            item->right=nullptr;
            size++;
            return item->para->second;
        }
    }
    return item->para->second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    if (isEmpty())
        throw std::out_of_range("");
    Item * item = root;
    while (item->para->first!=key)
    {
        if (key>item->para->first)
            item=item->right;
        else
            item=item->left;
        if (item==nullptr)
            throw std::out_of_range("");
    }
    return item->para->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    if (isEmpty())
        throw std::out_of_range("");
    Item * item = root;
    while (item->para->first!=key)
    {
        if (key>item->para->first)
            item=item->right;
        else
            item=item->left;
        if (item==nullptr)
            throw std::out_of_range("");
    }
    return item->para->second;
  }

  const_iterator find(const key_type& key) const
  {
    ConstIterator iter;
    Item * item = root;
    if (root)
    {
    while (item->para->first!=key)
        {
            if (key>item->para->first)
                item=item->right;
            else
                item=item->left;
            if (item == nullptr)
                return cend();
        }
    }
    iter.item=item;
    iter.tree=this;
    return iter;
  }

  iterator find(const key_type& key)
  {
    Iterator iter;
    Item * item = root;
    if (root)
    {
    while (item->para->first!=key)
        {
            if (key>item->para->first)
                item=item->right;
            else
                item=item->left;
            if (item == nullptr)
                return end();
        }
    }
    iter.item=item;
    iter.tree=this;
    return iter;
  }

  void remove(const key_type& key)
  {
    auto iter = find(key);
    remove (iter);
  }


  void remove(const const_iterator& it)
  {
    Item * item = it.item;
    if (item==nullptr)
        throw std::out_of_range("");
    Item * to_delete, * child;
    if (!item->left || !item->right)
    {
        to_delete=item;
        if (to_delete->left)
            child=to_delete->left;
        else
            child=to_delete->right;
        if(child)
            child->parent = to_delete->parent;
        if(!to_delete->parent)
            root = child;
        else if(to_delete == to_delete->parent->left)
            to_delete->parent->left  = child;
        else
            to_delete->parent->right = child;
        delete to_delete;
        size--;
        return;
    }
    else
    {
        Iterator iter = (Iterator)it;
        iter++;
        to_delete=iter.item;
        item->para = to_delete->para;
        to_delete->para = nullptr;
        remove(iter);
    }
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const TreeMap& other) const
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

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    Iterator iter;
    iter.item=findSmallest(root);
    iter.tree=this;
    return iter;
  }

  iterator end()
  {
    Iterator iter;
    iter.item=nullptr;
    iter.tree=this;
    return iter;
  }

  const_iterator cbegin() const
  {
    ConstIterator iter;
    iter.item=findSmallest(root);
    iter.tree=this;
    return iter;
  }

  const_iterator cend() const
  {
    ConstIterator iter;
    iter.item=nullptr;
    iter.tree=this;
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
private:
  Item * item;
  const TreeMap * tree;


public:

  friend class TreeMap;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  explicit ConstIterator()
  {}

  ConstIterator(const ConstIterator& other)
  {
    item=other.item;
    tree=other.tree;
  }

  ConstIterator& operator++()
  {
    Item * item=this->item;
    if (item==nullptr)
        throw std::out_of_range("");
    if (item->right!=nullptr)
        item=tree->findSmallest(item->right);
    else
    {
        Item * father = item->parent;
      while(father && (item == father->right))
      {
        item = father;
        father = father->parent;
      }
      item=father;
    }
    this->item=item;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if (this->item==nullptr)
        throw std::out_of_range("");
    ConstIterator orig=(*this);
    ++(*this);
    return orig;
  }

  ConstIterator& operator--()
  {
    Item * item=this->item;
    ConstIterator begin=tree->begin();
    if (*this==begin)
        throw std::out_of_range("");
    if (item==nullptr)
        item=tree->findLargest(tree->root);
    else
    {
        if (item->left!=nullptr)
            for (item=item->left;item->right!=nullptr;item=item->right);
        else
        {
            Item * father = item->parent;
          while(father && (item == father->left))
          {
            item = father;
            father = father->parent;
          }
          item=father;
        }
    }
    this->item=item;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator begin=tree->begin();
    if (*this==begin)
        throw std::out_of_range("");
    ConstIterator orig=(*this);
    --(*this);
    return orig;
  }

  reference operator*() const
  {
    if (this->item == nullptr)
        throw std::out_of_range("");
    return *(this->item->para);
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if (this->item == other.item)
        return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

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

#endif /* AISDI_MAPS_MAP_H */
