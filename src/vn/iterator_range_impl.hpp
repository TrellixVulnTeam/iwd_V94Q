#ifndef VN__ITERATOR_RANGE_IMPL_HPP
#define VN__ITERATOR_RANGE_IMPL_HPP

namespace vn {

template<typename T>
iterator_range<T>::iterator_range(T begin, T end)
  : _begin(std::move(begin))
  , _end(std::move(end))
{}

template<typename T>
T
iterator_range<T>::begin() const
{
  return _begin;
}

template<typename T>
T
iterator_range<T>::end() const
{
  return _end;
}

template<typename T>
iterator_range<T>
make_iterator_range(T begin, T end)
{
  return iterator_range<T>(begin, end);
}

} // namespace vn

#endif
