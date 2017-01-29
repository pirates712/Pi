#if !defined( THREADSAFELIST_H )
#define THREADSAFELIST_H

#include <mutex>
#include <list>
#include <condition_variable>

template <class T>
class threadSafeList
{
public:

   threadSafeList();

   ~threadSafeList();

   T getAndPopFront();

   void push_back( T newElem );

   typename std::list<T>::size_type size();

private:

   threadSafeList( const threadSafeList & copy );
   threadSafeList &operator =( const threadSafeList &rhs );

   std::list<T>            mList;
   std::mutex              mLock;
   std::condition_variable mCondVar;
};

template <class T>
threadSafeList<T>::threadSafeList()
{
}

template <class T>
threadSafeList<T>::~threadSafeList()
{
}

template <class T>
T threadSafeList<T>::getAndPopFront()
{
   T ret{};
   std::unique_lock<std::mutex> lk(mLock);
   mCondVar.wait(lk, [&]{return (mList.size() > 0);});
   ret = mList.front();
   mList.pop_front();
   lk.unlock();
   return ret;
}

template <class T>
void threadSafeList<T>::push_back( T newElem )
{
   {
      std::lock_guard<std::mutex> lk(mLock);
      mList.push_back( newElem );
   }
   mCondVar.notify_one();
}

template <class T>
typename std::list<T>::size_type threadSafeList<T>::size()
{
   typename std::list<T>::size_type size;
   {
      std::lock_guard<std::mutex> lk(mLock);
      size = mList.size();
   }
   return size;
}

#endif
