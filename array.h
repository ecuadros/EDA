#ifndef __ARRAY_H__
#define __ARRAY_H__

class Array {
public:
  Array(int size) {
    this->size = size;
    this->data = new int[size];
  }

  void insert(int val) {
    for (int i = 0; i < this->size; i++) {
      if (this->data[i] == 0) {
        this->data[i] = val;
        break;
      }
    }
  }

  void print() {
    for (int i = 0; i < this->size; i++) {
      cout << this->data[i] << " ";
    }
    cout << endl;
  }

private:
  int size;
  int* data;
};

#endif // __ARRAY_H__