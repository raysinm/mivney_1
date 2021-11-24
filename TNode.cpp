#include "AVLtree.h"
#include "./course_files/library1.h"

template<class KeyElem, class Data>
TNode::TNode(const &KeyElem key, const &Data data):
    key(key), data(data),
    father(nullptr), left_son(nullptr), right_son(nullptr){
}

template<class KeyElem, class Data>
TNode::void refreshBF(){
    int bf_l = 0, bf_r= 0;
    if(this->left_son){
        bf_l = this->left_son.BF;
    }
    if(this->right_son){
        bf_r = this->right_son.BF;
    }

    this->BF = bf_l - bf_r;
}