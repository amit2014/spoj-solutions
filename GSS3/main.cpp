#include <iostream>
#include <limits>
#include <cstdio>
using namespace std;

typedef long long Int;
const Int IDENTITY = std::numeric_limits<Int>::max();
struct Node;

struct Node {
  Int segmentSum;
  Int bestPrefix;
  Int bestSuffix;
  Int bestSum;
  void split(Node& l, Node& r) {}
  void merge(Node& l, Node& r) {
    if ( l.segmentSum == IDENTITY && r.segmentSum == IDENTITY ) {
      return;
    }
    if ( l.segmentSum == IDENTITY ) {
      *this = r;
    } else if ( r.segmentSum == IDENTITY ) {
      *this = l;
    } else {
      segmentSum = l.segmentSum + r.segmentSum;
      bestPrefix = max(l.segmentSum + r.bestPrefix, l.bestPrefix);
      bestSuffix = max(r.segmentSum + l.bestSuffix, r.bestSuffix);
      bestSum = max(max(l.bestSum, r.bestSum), l.bestSuffix + r.bestPrefix);
    }
  }
};

std::ostream& operator << ( std::ostream& os, const Node& node ) {
  os << "{";
  os << "sum = " << node.segmentSum << ", ";
  os << "pre = " << node.bestPrefix << ", ";
  os << "suf = " << node.bestSuffix << ", ";
  os << "max = " << node.bestSum << ", ";
  os << "}";
  return os;
}

const Int SIZE = 50000 + 11;
Int N;
Int AC;
Int A[SIZE];
const Int TREE_SIZE = 3 * SIZE;
Node tree[TREE_SIZE];
const Node NODE_IDENTITY = {IDENTITY, IDENTITY, IDENTITY, IDENTITY};

Node range_query( const Int& root, const Int& left_most_leaf, const Int& right_most_leaf, const Int& u, const Int& v ) {
  // prIntf("@range_query: %d, %d, %d, %d, %d\n", root, left_most_leaf, right_most_leaf, u, v);
  if ( u <= left_most_leaf && right_most_leaf <= v )
    return tree[root];

  Int mid = ( left_most_leaf + right_most_leaf ) / 2;
  Int left_child = root * 2;
  Int right_child = left_child + 1;

  tree[root].split(tree[left_child], tree[right_child]);
  Node l = NODE_IDENTITY, r = NODE_IDENTITY;
  if ( u < mid )
    l = range_query(left_child, left_most_leaf, mid, u, v);
  if ( mid < v )
    r = range_query(right_child, mid, right_most_leaf, u, v);

  tree[root].merge(tree[left_child], tree[right_child]);
  Node res;
  res.merge(l, r);
  return res;
}

void update_single_node( Node& node, const Node& new_node ) {
  node = new_node;
}

void range_update( const Int& root, const Int& left_most_leaf, const Int& right_most_leaf, const Int& u, const Int& v, const Node& new_node ) {
  if ( u <= left_most_leaf && right_most_leaf <= v )
    return update_single_node(tree[root], new_node);

  Int mid = ( left_most_leaf + right_most_leaf ) / 2;
  Int left_child = root * 2;
  Int right_child = left_child + 1;

  tree[root].split(tree[left_child], tree[right_child]);
  if ( u < mid )
    range_update(left_child, left_most_leaf, mid, u, v, new_node);
  if ( mid < v )
    range_update(right_child, mid, right_most_leaf, u, v, new_node);
  tree[root].merge(tree[left_child], tree[right_child]);
}

void update( const Int& pos, const Node& new_node ) {
  return range_update(1, N, 2 * N, N + pos, N + pos + 1, new_node);
}

int main() {
  std::fill(tree, tree + TREE_SIZE, NODE_IDENTITY);
  std::cin >> AC;
  for ( Int i = 0; i < AC; ++ i )
    std::cin >> A[i];

  // N: tree size
  N = 1;
  while ( N < AC )
    N *= 2;
  for ( Int i = 0; i < AC; ++ i ) {
    Node node = {A[i], A[i], A[i], A[i]};
    update(i, node);
  }

  Int Q;
  std::cin >> Q;
  for ( Int i = 0; i < Q; ++ i ) {
    Int query_type;
    Int x, y;
    std::cin >> query_type >> x >> y;
    if ( query_type ) {
      cout << range_query(1, N, 2 * N, N + x - 1, N + y).bestSum << endl;
    } else {
      Node node = {y, y, y, y};
      update(x - 1, node);
    }
  }

  return 0;
}
