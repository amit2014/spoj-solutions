
// @snippet<sh19910711/contest:headers.cpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>
#include <complex>
#include <functional>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

// @snippet<sh19910711/contest:solution/typedef.cpp>
namespace solution {
    typedef std::istringstream ISS;
    typedef std::ostringstream OSS;
    typedef std::vector<std::string> VS;
    typedef long long LL;
    typedef int INT;
    typedef std::vector<INT> VI;
    typedef std::vector<VI> VVI;
    typedef std::pair<INT,INT> II;
    typedef std::vector<II> VII;
}

// @snippet<sh19910711/contest:solution/interface.cpp>
namespace solution {
    class ISolution {
    public:
        virtual void init() {};
        virtual bool input() { return false; };
        virtual void output() {};
        virtual int run() = 0;
    };
}

// @snippet<sh19910711/contest:solution/solution.cpp>
namespace solution {
    using namespace std;

    class Node {
    public:
        int sum;
        int minsum;

        void merge( const Node& a, const Node& b ) {
            sum = a.sum + b.sum;
            minsum = min(a.minsum, a.sum + b.minsum);
        }
    };

    const int SIZE = 30000 + 11;
    const int TREE_SIZE = 3 * SIZE + 2 + 11;
    const char* YES = "YES";
    const char* NO = "NO";

    int n, m;
    char s[SIZE];
    Node T[TREE_SIZE];

    int get_type( int k ) {
        return s[k] == '(' ? 1 : -1;
    }

    void update( int cur, int l, int r, int k ) {
        if ( k != 0 && ( k < l || k > r ) )
            return;
        if ( l == r ) {
            T[cur].sum = T[cur].minsum = get_type(l - 1);
        } else {
            int mid = ( l + r ) / 2;
            int left_child = 2 * cur + 0;
            int right_child = 2 * cur + 1;
            update(left_child, l, mid, k);
            update(right_child, mid + 1, r, k);
            T[cur].merge(T[left_child], T[right_child]);
        }

    }

    bool check() {
        return T[1].sum == 0 && T[1].minsum == 0;
    }

    void query( int q ) {
        if ( q == 0 ) {
            printf("%s\n", (check() ? YES : NO));
        } else {
            s[q - 1] = ( s[q - 1] == '(' ? ')' : '(' );
            update(1, 1, n, q);
        }
    }

    void solve() {
        update(1, 1, n, 0);
        for ( int i = 0; i < m; ++ i ) {
            int q;
            scanf("%d", &q);
            query(q);
        }
    }
    
    class Solution: public ISolution {
    public:
        
        bool input() {
            if ( ! ( scanf("%d", &n) == 1 && n ) )
                return false;
            scanf("%s%d", s, &m);
            return true;
        }
        
        int run() {
            int test_no = 1;
            while ( init(), input() ) {
                printf("Test %d:\n", test_no);
                test_no ++;
                solve();
                output();
            }
            return 0;
        }
        
    };
}

// @snippet<sh19910711/contest:main.cpp>
int main() {
    return solution::Solution().run();
}

