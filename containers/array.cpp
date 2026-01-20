#include <assert.h>
#include <iostream>
using namespace std;
#include "array.h"

CArray::CArray(size_t size) {
  m_capacity = size;
  m_data = new value_type[size];
}
CArray::~CArray() { delete[] m_data; }

value_type &CArray::operator[](size_t index) {
  cout << "XResizing from " << m_capacity << " to at least " << index + 5
       << endl;
  if (index > m_capacity) {
    cout << "Resizing from " << m_capacity << " to at least " << index + 5
         << endl;
    resize(index - m_last + 5);
  }

  assert(index < m_capacity);

  if (index > m_last)
    m_last = index;
  return m_data[index];
}

size_t CArray::getSize() const { return m_last + 1; }

void CArray::push_back(value_type value) {
  if (m_last >= m_capacity)
    resize();
  m_data[m_last++] = value;
}

void CArray::resize(size_t delta) {
  size_t new_capacity = m_capacity + delta;
  value_type *new_data = new value_type[new_capacity];
  for (auto i = 0; i < m_capacity; ++i)
    new_data[i] = m_data[i];
  delete[] m_data;
  m_data = new_data;
  m_capacity = new_capacity;
}

void DemoArray() {
  const int N = 5;
  CArray arr1(N);
  for (auto i = 0; i < N; ++i)
    arr1.push_back(i * 10);
  cout << "First part finished" << endl;
  arr1[3] = 43;
  arr1.push_back(80);
  arr1.push_back(75);

  // test for [] operator with out of bounds index
  arr1[9] = 10;

  cout << "Ready to print" << endl;
  for (auto i = 0; i < arr1.getSize(); ++i)
    cout << arr1[i] << endl;
  // cout << arr1[50] << endl; // This will trigger an assertion failure
  cout << "DemoArray finished." << endl;
}
