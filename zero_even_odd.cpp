#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>
#include <queue>
#include <future>
#include <atomic>
#include <iostream>

using namespace std;

auto printNumber = [](int num) -> void {
  cout << num << endl;
};

//void printNumber(int num) {
//  cout << num << endl;
//}

class ZeroEvenOdd {
public:
  int N;
  int i{0};
  mutex m;
  condition_variable cond;

  ZeroEvenOdd(int n):N(n){}

  void zero(function<void(int)> printFn) {
    while(true) {
      unique_lock<mutex> lock(m);
      cond.wait(lock, [&]() { return i >= N || i == 0;});
      if(i >= N) {
        cond.notify_all();
        return;
      }
      ++i;
      printFn(i);
      lock.unlock();
      cond.notify_all();
    }
  }

  void odd(function<void(int)> printFn) {
    while(true) {
      unique_lock<mutex> lock(m);
      cond.wait(lock, [&]() { return i >= N || (i % 2 == 1);});
      if(i >= N) {
        cond.notify_all();
        return;
      }
      ++i;
      printFn(i);
      lock.unlock();
      cond.notify_all();
    }
  }

  void even(function<void(int)> printFn) {
    while(true) {
      unique_lock<mutex> lock(m);
      cond.wait(lock, [&]() { return i >= N || ((i > 0) && (i % 2 == 0));});
      if(i >= N) {
        cond.notify_all();
        return;
      }
      ++i;
      printFn(i);
      lock.unlock();
      cond.notify_all();
    }
  }
};


int main() {
  ZeroEvenOdd z(15);
  thread zeroThread(&ZeroEvenOdd::zero, &z, printNumber);
  thread oddThread(&ZeroEvenOdd::odd, &z, printNumber);
  thread evenThread(&ZeroEvenOdd::even, &z, printNumber);

  zeroThread.join();
  oddThread.join();
  evenThread.join();
  return 0;
}
