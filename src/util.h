#pragma once

#include "SgPoint.h"

namespace util{

template<int N> inline bool is_star_point(SgPoint const & p) {
    int x = SgPointUtil::Col(p);
    int y = SgPointUtil::Row(p);
    switch(N){
    case 9: return (x==3 || x==7) && (y==3 || y==7);
    case 13: return (x==4 || x==7 || x==10) && (y==4 || y==7 || y==10);
    case 19: return (x==4 || x==10 || x==16) && (y==4 || y==10 || y==16);
    }
}

}
