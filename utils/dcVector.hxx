////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                           DREAMCLOUD PROJECT                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef DREAMCLOUD__BASE_TYPES_CLIB__DCVECTOR_HXX
#define DREAMCLOUD__BASE_TYPES_CLIB__DCVECTOR_HXX

////////////////////
//    INCLUDES    //
////////////////////
#include <vector>
#include <algorithm>

////////////////////
//      USING     //
////////////////////
using std::vector;

template<class T, class Allocator = std::allocator<T> >
class dcVector
  : public vector<T,Allocator>
{
public:
  inline dcVector(const Allocator& alloc = Allocator()) : vector<T,Allocator>(alloc) { }
  inline dcVector(const typename std::vector<T,Allocator>& vec, const Allocator& alloc = Allocator())
    : vector<T,Allocator>(vec, alloc) { }

  inline dcVector(typename vector<T,Allocator>::size_type n) : vector<T,Allocator>(n) { }

  inline dcVector(typename vector<T,Allocator>::size_type n, const T& value, const Allocator& alloc = Allocator())
    : vector<T,Allocator>(n, value, alloc) { }
  
  inline dcVector(typename vector<T,Allocator>::iterator begin, typename vector<T,Allocator>::iterator end,
                   const Allocator& alloc = Allocator()) : vector<T,Allocator>(begin, end, alloc) { }


  static bool intersection(const typename std::vector<T,Allocator>& v1, 
                           const typename std::vector<T,Allocator>& v2, typename std::vector<T,Allocator>& intersec)
  {
    vector<T,Allocator> v1_cpy(v1), v2_cpy(v2);

    intersec.clear();

    sort(v1_cpy.begin(), v1_cpy.end());
    sort(v2_cpy.begin(), v2_cpy.end());
    std::set_intersection(v1_cpy.begin(), v1_cpy.end(), 
                          v2_cpy.begin(), v2_cpy.end(), back_inserter(intersec));

    return (!intersec.empty());
  }

  static inline bool intersection(const typename std::vector<T,Allocator>& v1, const typename std::vector<T,Allocator>& v2)
  {
    vector<T,Allocator> v;
    return intersection(v1, v2, v);
  }

protected:
  // empty

private:
  // empty
};

#endif

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  END OF FILE.                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
