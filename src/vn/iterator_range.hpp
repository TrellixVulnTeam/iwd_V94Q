#ifndef VN__ITERATOR_RANGE_HPP
#define VN__ITERATOR_RANGE_HPP

namespace vn {

template<typename TIterator>
struct iterator_range
{
  iterator_range(TIterator begin, TIterator end);

  iterator_range(const iterator_range&) = default;
  iterator_range& operator=(const iterator_range&) = default;

  iterator_range(iterator_range&&) = default;
  iterator_range& operator=(iterator_range&&) = default;

  TIterator begin() const;
  TIterator end() const;

private:
  TIterator _begin;
  TIterator _end;
};

template<typename TIterator>
iterator_range<TIterator>
make_iterator_range(TIterator begin, TIterator end);

} // namespace vn

#include <vn/iterator_range_impl.hpp>

#endif
