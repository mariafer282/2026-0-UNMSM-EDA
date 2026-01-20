#ifndef __ARRAY_H__
#define __ARRAY_H__
#include <stddef.h>

using value_type = int;

class CArray {
private:
  size_t m_capacity = 0, m_last = 0;
  value_type *m_data = nullptr;

public:
  CArray(size_t size);
  ~CArray();

  void push_back(value_type value);
  value_type &operator[](size_t index);
  size_t getSize() const;
  void resize();
  void resize(size_t extra_capacity);
};

void DemoArray();

#endif // __ARRAY_H__
