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

class ZeroEvenOdd {
public:
  int N;
  int i{0};

  ZeroEvenOdd(int n):N(n){}

  int start(function<void(int)> printFn) {
     printFunc = printFn;
     pthread_cond_init(&cond, NULL);
     int rc0 = pthread_create(&threads[0], nullptr, ZeroEvenOdd::zero, this);
     int rc1 = pthread_create(&threads[1], nullptr, ZeroEvenOdd::odd, this);
     int rc2 = pthread_create(&threads[2], nullptr, ZeroEvenOdd::even, this);
  }

  void run() {
  }

  void join() {
    pthread_join(threads[0], nullptr);
    pthread_join(threads[1], nullptr);
    pthread_join(threads[2], nullptr);
  }
protected:
  
  static void *zero(void *pthis) {
    cout << "In static zero" << endl;
    ZeroEvenOdd *pObj = (ZeroEvenOdd *)pthis;
    pObj->runZero();
  }

  static void *odd(void *pthis) {
    cout << "In static odd" << endl;
    ZeroEvenOdd *pObj = (ZeroEvenOdd *)pthis;
    pObj->runOdd();
  }

  static void *even(void *pthis) {
    cout << "In static even" << endl;
    ZeroEvenOdd *pObj = (ZeroEvenOdd *)pthis;
    pObj->runEven();
  }

  void runZero() {
    if(i <= 0) {
      cout << "In runZero: i = " << i << endl;
      pthread_mutex_lock(&mutex);
      cout << "runZero got lock: i = " << i << endl;
      if(i > N) {
        return;
      }
      printFunc(i);
      i++;
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
    }
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
  }

  void runOdd() {
    while(i <= N) {
      pthread_mutex_lock(&mutex);
      while(i < 1 || i % 2 == 0) {
        pthread_cond_wait(&cond, &mutex);
      }

      if(i > N) {
        break;
      }
      printFunc(i);
      i++;
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
    }
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
  }

  void runEven() {
    while(i <= N) {

      pthread_mutex_lock(&mutex);
      while(i < 1 || i % 2 == 1) {
        int rc = pthread_cond_wait(&cond, &mutex);
      }

      if(i > N) {
        break;
      }
      printFunc(i);
      i++;
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
    }
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

  }
  pthread_t threads[3];
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  function<void(int)> printFunc;
};

int main() {
  ZeroEvenOdd z(15);
  z.start(printNumber);
  z.join();
  return 0;
}
