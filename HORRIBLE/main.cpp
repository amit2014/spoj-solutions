#include <iostream>
#include <limits>
#include <cstdio>
using namespace std;

typedef long long Int;

struct Node;
const Int INF = std::numeric_limits<Int>::max();

struct Node {
  Int add;
  Int sum;
  Int num_leaves;
  void split( Node& l, Node& r );
  void merge( Node& l, Node& r );
};

std::ostream& operator << ( std::ostream& os, const Node& node ) {
  os << "{";
  os << "add = " << node.add << ", "; 
  os << "sum = " << node.sum << ", "; 
  os << "leaves = " << node.num_leaves << ", "; 
  os << "}";
  return os;
}

void Node::split( Node& l, Node& r ) {
  l.add += add;
  l.sum += add * l.num_leaves;
  r.add += add;
  r.sum += add * r.num_leaves;
  add = 0;
}

void Node::merge( Node& l, Node& r ) {
  num_leaves = l.num_leaves + r.num_leaves;
  sum = l.sum + r.sum;
  add = 0;
}

const Int SIZE = 100000 + 11;
Int N;
Int AC;
Int A[SIZE];
const Int TREE_SIZE = 3 * SIZE + 11;
Node tree[TREE_SIZE];
const Node NODE_IDENTITY = {0, 0, 0};

Node range_query( const Int& root, const Int& left_most_leaf, const Int& right_most_leaf, const Int& u, const Int& v ) {
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
  node.num_leaves += new_node.num_leaves;
  node.add += new_node.add;
  node.sum += new_node.add * node.num_leaves;
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

void update( const Int& l, const Int& r, const Node& new_node ) {
  return range_update(1, N, 2 * N, N + l, N + r, new_node);
}

int main() {
  Int tests;
  std::cin >> tests;
  for ( Int t = 0; t < tests; ++ t ) {
    Int Q;
    std::fill(tree, tree + TREE_SIZE, NODE_IDENTITY);
    std::cin >> AC >> Q;

    N = 1;
    while ( N < AC )
      N *= 2;
    for ( Int i = 0; i < AC; ++ i ) {
      Node node = {0, 0, 1};
      update(i, node);
    }

    for ( Int q = 0; q < Q; ++ q ) {
      Int type;
      std::cin >> type;
      if ( type ) {
        Int p, q;
        std::cin >> p >> q;
        std::cout << range_query(1, N, 2 * N, N + p - 1, N + q).sum << endl;
      } else {
        Int p, q, v;
        std::cin >> p >> q >> v;
        Node node = {v, 0, 0};
        update(p - 1, q, node);
      }
    }
  }

  return 0;
}
