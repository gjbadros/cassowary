// $Id$
// See http://cseng.aw.com/bookdetail.qry?ISBN=0-201-63371-X&ptype=634
// First version is from latest appendix of More Effective C++
// Second version from an earlier appendix (works w/ egcs)


#ifndef AUTO_PTR_H
#define AUTO_PTR_H

#if 0
template<class X>
class auto_ptr {
  mutable bool owner;
  X* px;
  template<class Y> friend class auto_ptr;
 public:
  explicit auto_ptr(X* p=0) 
    : owner(p), px(p) {}
  template<class Y>
    auto_ptr(const auto_ptr<Y>& r) 
    : owner(r.owner), px(r.release()) {}
  template<class Y>
    auto_ptr& operator=(const auto_ptr<Y>& r) {
  if ((void*)&r != (void*)this) {
  if (owner) 
    delete px;
  owner = r.owner; 
  px = r.release();
  }
  return *this;
  }
  ~auto_ptr()           { if (owner) delete px; }
  X& operator*()  const { return *px; }
  X* operator->() const { return px; }
  X* get()        const { return px; }
  X* release()    const { owner = 0; return px; }
};
#else
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


  void reset(T *p = 0) { delete pointee; pointee = p; }


 private:
  T *pointee;
};

#endif


#endif
