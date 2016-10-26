#pragma once

#include "SgPoint.h"

namespace util{

template<std::size_t N> bool is_star_point(SgPoint const & p) {
    std::size_t x = SgPointUtil::Col(p);
    std::size_t y = SgPointUtil::Row(p);
    switch(N){
    case 9: return (x==3 || x==7) && (y==3 || y==7);
    case 13: return (x==4 || x==7 || x==10) && (y==4 || y==7 || y==10);
    case 19: return (x==4 || x==10 || x==16) && (y==4 || y==10 || y==16);
    }
}

template<std::size_t N> constexpr int twoDto1D(int x, int y) {
    return y * N + x;
}

}

#ifdef TORAASSERT
#define tora_assert(expr) { if(!(expr)) { std::cerr << "assertion violated in file " << __FILE__ << ", line " << __LINE__ << std::endl; }}
#define tora_equals(a,b) { if((a)!=(b)) { std::cerr << "left doesn't equal right expression in file " << __FILE__ << ", line " << __LINE__ << ": " << a << " != " << b << std::endl; }}
#else
#define tora_assert(ignore) {}
#define tora_equals(ignore_a, ignore_b) {}
#endif
