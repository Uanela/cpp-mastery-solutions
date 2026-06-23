#include <iostream>

template <typename T, std::size_t S> class Vector {
public:
  T *data = nullptr;
  size_t length = 0;
  size_t max_capacity = 0;
  // Vector() = default;
  // Vector(const T(&arr)[S]) {
  //   for(int i = 0; i < S; i++) {
  //     this->data[i] = arr[i];
  //   };
  // }
  Vector() : data(nullptr), length(0) {}

  // Vector& operator=(const T(&arr)[S]) {
  //   // this->data = arr;
  //   // std::cout << value << "badass\n";
  //   return *this;
  // }
  void resize() {
    if (length == max_capacity ) {
      max_capacity = max_capacity == 0 ? 1 : max_capacity * 2;
      std::cout << "resizing from " << length << " to " << max_capacity << std::endl; 
    }
    T *new_v = new T[max_capacity];
    for(int i = 0; i < max_capacity; i++){
      if (i < length)
        new_v[i] = data[i];
      else new_v[i] = 0;
    }
    delete[] data;
    data = new_v;
  }

  Vector push_back(T val) {
    if (length == max_capacity) 
      resize();

    data[length] = val;
    length++;
    return *this;
  }

  size_t size() {
    return length;
  }

  size_t capacity() {
    return max_capacity;
  }
};

int main() {
  Vector<int, 3> v;
  v.push_back(1);
  v.push_back(30);
  v.push_back(7);
  v.push_back(70);
  v.push_back(75);
  v.push_back(76);
  v.push_back(78);
  v.push_back(79);
  v.push_back(80);


  std::cout << v.data[0] << std::endl;
  std::cout << v.data[1] << std::endl;
  std::cout << v.data[2] << std::endl;
  std::cout << v.data[3] << std::endl;
  // v.resize(10);
  // std::cout << sizeof(v.data);

  return 0;
}
