#include "AVLtree.h"
#include "./course_files/library1.h"




template<class KeyElem, class Data>
TNode::TNode(const &KeyElem key, const &Data data):
    key(key), data(data),
    right(nullptr), left(nullptr){
}