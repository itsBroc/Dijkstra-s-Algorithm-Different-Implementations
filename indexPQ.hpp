#ifndef INDEX_PRIORITY_QUEUE_HPP_
#define INDEX_PRIORITY_QUEUE_HPP_

#include <vector>
#include <algorithm>

template <typename T>
class IndexPriorityQueue {
 private:
  // vector to hold priorities.  
  // priorities.at(i) is the priority associated to index i
  // the heap will only contain indices, we look up priorities
  // in this vector as needed
  std::vector<T> priorities {};
  // priorityQueue stores indices: priorityQueue.at(i) is an index
  // priorityQueue functions as the heap and is heap ordered: 
  // priorities.at(priorityQueue.at(i)) <= priorities.at(priorityQueue.at(2 * i)) 
  // priorities.at(priorityQueue.at(i)) <= priorities.at(priorityQueue.at(2 * i) + 1) 
  std::vector<int> priorityQueue {};
  // indexToPosition.at(i) is the position in priorityQueue of index i
  // priorityQueue.at(indexToPosition.at(i)) = i
  // indexToPosition.at(priorityQueue.at(j)) = j
  std::vector<int> indexToPosition {};
  int size_ = 0;

 public:
  explicit IndexPriorityQueue(int);
  void push(const T&, int);
  void pop();
  void erase(int);
  bool contains(int) const;
  void changeKey(const T&, int);
  std::pair<T, int> top() const;
  bool empty() const;
  int size() const;

 private:
 void swim(int);
 void sink(int);
  // TODO: you may want to add your own member functions. swim? sink?
};

// Useful helper functions
int leftChild(int i) {
  return 2*i;
}

int rightChild(int i) {
  return 2*i + 1;
}

int parent(int i) {
  return i/2;
}

// IndexPriorityQueue member functions
template <typename T>
IndexPriorityQueue<T>::IndexPriorityQueue(int N) {
  priorities.resize(N);
  priorityQueue.resize(N + 1);
  indexToPosition.resize(N, -1);
}

template <typename T>
bool IndexPriorityQueue<T>::empty() const {
  return size_ == 0;
}

template <typename T>
int IndexPriorityQueue<T>::size() const {
  return size_;
}

template <typename T>
void IndexPriorityQueue<T>::push(const T& priority, int index) {
  if(contains(index)) return;

  priorities[index] = priority;
  priorityQueue[++size_] = index;
  indexToPosition[index] = size_;
  swim(size_);
}

template <typename T>
void IndexPriorityQueue<T>::pop() {
  if(empty()) return;

  std::swap(priorityQueue[1], priorityQueue[size_]);
  indexToPosition[priorityQueue[1]] = -1;
  size_--;
  sink(1);
}

template <typename T>
void IndexPriorityQueue<T>::erase(int index) {
  if(!contains(index)) return;

  int position = indexToPosition[index];
  std::swap(priorityQueue[position], priorityQueue[size_]);
  indexToPosition[priorityQueue[position]] = position; // update moved elements position
  indexToPosition[index] = -1;
  
  size_--;
  swim(position);
  sink(position);
}

template <typename T>
std::pair<T, int> IndexPriorityQueue<T>::top() const {
  return {priorities[priorityQueue[1]], priorityQueue[1]};
}

// if vertex i is not present, insert it with key
// otherwise change the associated key value of i to key
template <typename T>
void IndexPriorityQueue<T>::changeKey(const T& key, int index) {
  if(!contains(index)){
    push(key, index);
    return;
  }

  priorities[index] = key;
  swim(indexToPosition[index]);
  sink(indexToPosition[index]);
}

template <typename T>
bool IndexPriorityQueue<T>::contains(int index) const {
  if(index < 0 || index >= static_cast<int>(indexToPosition.size())) return false;
  return indexToPosition[index] != -1;
}

template <typename T>
void IndexPriorityQueue<T>::swim(int k){
  while(k > 1 && priorities[priorityQueue[k]] < priorities[priorityQueue[parent(k)]]) {
    std::swap(indexToPosition[priorityQueue[k]], indexToPosition[priorityQueue[parent(k)]]);
    std::swap(priorityQueue[k], priorityQueue[parent(k)]);
    k = parent(k);
  }

}

template <typename T>
void IndexPriorityQueue<T>::sink(int k){
  while(leftChild(k) <= size_){
    int j = leftChild(k);
    if(j < size_ && priorities[priorityQueue[j]] > priorities[priorityQueue[j + 1]]) j++;
    if(priorities[priorityQueue[k]] <= priorities[priorityQueue[j]]) break;
    std::swap(indexToPosition[priorityQueue[k]], indexToPosition[priorityQueue[j]]);
    std::swap(priorityQueue[k], priorityQueue[j]);
    k = j;
  }
}

#endif      // INDEX_PRIORITY_QUEUE_HPP_