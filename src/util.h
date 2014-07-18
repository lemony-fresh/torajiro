#pragma once

namespace util{

template<int N> inline bool is_star_point(int x, int y) {
    switch(N){
    case 9: return (x==2 || x==6) && (y==2 || y==6);
    case 13: return (x==3 || x==6 || x==9) && (y==3 || y==6 || y==9);
    case 19: return (x==3 || x==9 || x==15) && (y==3 || y==9 || y==15);
    }
}

}
