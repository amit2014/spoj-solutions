
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

// @snippet<sh19910711/contest:solution/define_classes.cpp>
namespace solution {
  class Solution;
  class Solver;
  class Storages;
}

// @snippet<sh19910711/contest:solution/interface.cpp>
namespace solution {
  class SolutionInterface {
  public:
    virtual ~SolutionInterface() {};
    virtual int run() = 0;
    
  protected:
    SolutionInterface() {}
    virtual void pre_calc() {}
    virtual bool action() = 0;
    virtual void init() {};
    virtual bool input( Storages& s ) { return false; };
    virtual void output( const Storages& s ) const {};
    virtual void before_action( const int& test_no ) const {}
    virtual void after_action( const int& test_no ) const {}
  };
}

// @snippet<sh19910711/contest:solution/solution-base.cpp>
namespace solution {
  class SolutionBase: public SolutionInterface {
  public:
    virtual ~SolutionBase() {}
    virtual int run() {
      this->pre_calc();
      for (;;) {
        this->before_action(-1);
        if ( ! this->action() ) { this->after_action(-1); break; }
        this->after_action(-1);
      };
      return 0;
    }
  protected:
    SolutionBase() {}
  };
}

// @snippet<sh19910711/contest:misc/smart_pointer_reference_count.cpp>
namespace misc {
  class SmartPointerReferenceCount {
  public:
    void add() { count ++; }
    int release() { return -- count; }
  private:
    int count;
  };
}

// @snippet<sh19910711/contest:misc/smart_pointer.cpp>
namespace misc {
  template <typename Type> class SmartPointer {
  public:
    SmartPointer(): pointer(0), reference(0) {
      reference = new misc::SmartPointerReferenceCount();
      reference->add();
    }
    SmartPointer(Type* value): pointer(value) {
      reference = new misc::SmartPointerReferenceCount();
      reference->add();
    }
    SmartPointer(const SmartPointer<Type>& sp): pointer(sp.pointer), reference(sp.reference) {
      reference->add();
    }
    ~SmartPointer() {
      if ( reference->release() == 0 ) {
        delete pointer;
        delete reference;
      }
    }
    Type& operator *() { return *pointer; }
    const Type& operator *() const { return *pointer; }
    Type* operator +( const int& x ) { return pointer + x; }
    const Type* operator +( const int& x ) const { return pointer + x; }
    Type* operator ->() { return pointer; }
    SmartPointer<Type>& operator = ( const SmartPointer<Type>& sp ) {
      if ( this != &sp ) {
        if ( reference->release() == 0 ) {
          delete pointer;
          delete reference;
        }
        pointer = sp.pointer;
        reference = sp.reference;
        reference->add();
      }
      return *this;
    }
  private:
    Type* pointer;
    misc::SmartPointerReferenceCount* reference;
  };
}

// @snippet<sh19910711/contest:storage/vector.cpp>
namespace storage {
  template <typename ValueType, int SIZE> class VectorClass {
  public:
    typedef misc::SmartPointer<ValueType> Pointer;
    VectorClass() { data = Pointer(new ValueType[SIZE]); }
    ValueType& operator [] ( const int& index ) { return *(data + index); }
    const ValueType& operator [] ( const int& index ) const { return *(data + index); }
  private:
    Pointer data;
  };
}

// @snippet<sh19910711/contest:storage/vector_types.cpp>
namespace storage {
  const int VECTOR_NONE = -1;
  template <typename ValueType, int SIZE_X = VECTOR_NONE, int SIZE_Y = VECTOR_NONE, int SIZE_Z = VECTOR_NONE> struct Vector {
    typedef storage::VectorClass<ValueType, SIZE_Z> Type__;
    typedef storage::VectorClass<Type__, SIZE_Y> Type_;
    typedef storage::VectorClass<Type_, SIZE_X> Type;
  };
  template <typename ValueType, int SIZE_X, int SIZE_Y> struct Vector<ValueType, SIZE_X, SIZE_Y, VECTOR_NONE> {
    typedef storage::VectorClass<ValueType, SIZE_Y> Type_;
    typedef storage::VectorClass<Type_, SIZE_X> Type;
  };
  template <typename ValueType, int SIZE_X> struct Vector<ValueType, SIZE_X, VECTOR_NONE, VECTOR_NONE> {
    typedef storage::VectorClass<ValueType, SIZE_X> Type;
  };
}

namespace tree {
  template <class Node, int TREE_SIZE> class Tree {
  public:
    virtual Node get_identity() = 0;
    
    void init( const int& new_nodes ) {
      N = new_nodes;
      std::fill(list + 0, list + TREE_SIZE, this->get_identity());
    }
    
    void update_single_node( const int& node_id, const Node& new_node ) {
      list[node_id] = new_node;
    }
    
    void range_update( const int& root, const int& left_most_leaf, const int& right_most_leaf, const int& u, const int& v, const Node& new_node ) {
      if ( u <= left_most_leaf && right_most_leaf <= v )
        return update_single_node(root, new_node);
      int mid = ( left_most_leaf + right_most_leaf ) / 2;
      int left_child_root = root * 2;
      int right_child_root = left_child_root + 1;
      if ( u < mid )
        range_update(left_child_root, left_most_leaf, mid, u, v, new_node);
      if ( mid < v )
        range_update(right_child_root, mid, right_most_leaf, u, v, new_node);
      list[root].merge(list[left_child_root], list[right_child_root]);
    }
    
    void update( const int& pos, const Node& new_node ) {
      range_update(1, N, 2 * N, N + pos, N + pos + 1, new_node);
    }
    
    Node range_query( const int& root, const int& left_most_leaf, const int& right_most_leaf, const int& u, const int& v ) {
      if ( u <= left_most_leaf && right_most_leaf <= v )
        return list[root];
      int mid = ( left_most_leaf + right_most_leaf ) / 2;
      int left_child_root = root * 2;
      int right_child_root = left_child_root + 1;
      Node left_node = this->get_identity();
      Node right_node = this->get_identity();
      if ( u < mid )
        left_node = range_query(left_child_root, left_most_leaf, mid, u, v);
      if ( mid < v )
        right_node = range_query(right_child_root, mid, right_most_leaf, u, v);
      Node res = this->get_identity();
      res.merge(left_node, right_node);
      return res;
    }
    
    Node query( const int& l, const int& r ) {
      return range_query(1, N, 2 * N, N + l, N + r);
    }
    
    Node list[TREE_SIZE];
    int N;
  };
}

// @snippet<sh19910711/contest:solution/typedef.cpp>
namespace solution {
  typedef std::istringstream ISS;
  typedef std::ostringstream OSS;
  typedef long long Int;
  typedef long double Double;
  typedef std::string String;
  
}

// @snippet<sh19910711/contest:solution/namespace-area.cpp>
namespace solution {
  // namespaces, types
  using namespace std;
}

// @snippet<sh19910711/contest:solution/consts-area.cpp>
namespace solution {
  // constant vars
  const Int SIZE = 50000 + 11;
  const Int TREE_SIZE = SIZE * 3 + 11;
  const Int WORST_VALUE = std::numeric_limits<Int>::min() / 2;
}

namespace solution {
  struct Node {
    Int segmentSum;
    Int bestPrefix;
    Int bestSuffix;
    Int bestSum;
    
    void merge( const Node& left, const Node& right ) {
      segmentSum = left.segmentSum + right.segmentSum;
      bestPrefix = std::max(left.segmentSum + right.bestPrefix, left.bestPrefix);
      bestSuffix = std::max(right.segmentSum + left.bestSuffix, right.bestSuffix);
      bestSum    = std::max(std::max(left.bestSum, right.bestSum), left.bestSuffix + right.bestPrefix);
    }
    
    friend std::ostream& operator << ( std::ostream& os, const Node& node ) {
      os << "{";
      os << node.segmentSum << ", ";
      os << node.bestPrefix << ", ";
      os << node.bestSuffix << ", ";
      os << node.bestSum << ", ";
      os << "}";
      return os;
    }
  };
  
  class Tree: public tree::Tree<Node, TREE_SIZE> {
    Node get_identity() {
      Node res = {0, WORST_VALUE, WORST_VALUE, WORST_VALUE};
      return res;
    }
  };
}

// @snippet<sh19910711/contest:solution/storages-area.cpp>
namespace solution {
  typedef storage::Vector<Int, SIZE>::Type List;
  
  // storages
  class Storages {
  public:
    Int N;
    List A;
    
    Int Q;
    List T;
    List X;
    List Y;
    
    Tree tree;
    
    List results;
  };
  
  Storages global_storages;
}

// @snippet<sh19910711/contest:solution/solver-area.cpp>
namespace solution {
  class Solver {
  public:
    void solve( Storages& s ) {
      s.tree.init(s.N);
      for ( int i = 0; i < s.N; ++ i )
        s.tree.update(i, create_leaf(s.A[i]));
      s.results = proc_queries(s.Q, s.T, s.X, s.Y, s.tree);
    }
  protected:
    static Node create_leaf( const Int& v ) {
      Node res = {v, v, v, v};
      return res;
    }
    
    static List proc_queries( const Int& queries, const List& T, const List& X, const List& Y, Tree& tree ) {
      List res;
      for ( int i = 0; i < queries; ++ i )
        res[i] = proc_query(T[i], X[i], Y[i], tree);
      return res;
    }
    
    static Int proc_query( const Int& type, const Int& x, const Int& y, Tree& tree ) {
      if ( type ) {
        return tree.query(x - 1, y).bestSum;
      } else {
        tree.update(x - 1, create_leaf(y));
        return -1;
      }
    }
    
  };
}

namespace solution {
  class Solution: public SolutionBase {
  public:
    Solution() {}
    Solution(Storages* p): storages(p) {}
    
  protected:
    virtual bool action() {
      this->init();
      if ( ! this->input(*this->storages) )
        return false;
      solver.solve(*this->storages);
      this->output(*this->storages);
      return true;
    }
    
    bool input( Storages& s ) {
      if ( ! ( std::cin >> s.N ) )
        return false;
      for ( int i = 0; i < s.N; ++ i )
        std::cin >> s.A[i];
      std::cin >> s.Q;
      for ( int i = 0; i < s.Q; ++ i ) {
        std::cin >> s.T[i] >> s.X[i] >> s.Y[i];
      }
      return true;
    }
    
    void output( const Storages& s ) const {
      for ( int i = 0; i < s.Q; ++ i )
        if ( s.T[i] )
          std::cout << s.results[i] << endl;
    }
    
  private:
    Solver solver;
    Storages* storages;
  };
}

// @snippet<sh19910711/contest:main.cpp>
#ifndef __MY_UNIT_TEST__
int main() {
  std::cin.sync_with_stdio(false);
  return solution::Solution(&solution::global_storages).run();
}
#endif



