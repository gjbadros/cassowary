// $Id$
// See http://cseng.aw.com/bookdetail.qry?ISBN=0-201-63371-X&ptype=634
// auto_ptr from More Effective C++ an earlier appendix (works w/ egcs)


#ifndef AUTO_PTR_H
#define AUTO_PTR_H

#ifdef _WIN32
#include <memory>
#else
// FIXGJB: This implementation for egcs is buggy -- be careful
// and replace ASAP
template<class T>
class auto_ptr {
 public:
  explicit auto_ptr(T *p = 0): pointee(p) {}

  template<class U>
    auto_ptr(auto_ptr<U>& rhs): pointee(rhs.release()) {}

  ~auto_ptr() { delete pointee; }

  template<class U>
    auto_ptr<T>& operator=(auto_ptr<U>& rhs)
    {
    if (this != &rhs) reset(rhs.release());
    return *this;
    }

  T& operator*() const { return *pointee; }

  T* operator->() const { return pointee; }

  T* get() const { return pointee; }

  T* release()
    {
    T *oldPointee = pointee;
    pointee = 0;
    return oldPointee;
    } 

 protected:
  // This is non-standard
  void reset(T *p = 0) { delete pointee; pointee = p; }

 private:
  T *pointee;
};

#endif


#endif
