#include "AVLtree.h"
#include "./course_files/library1.h"

template<class KeyElem, class Data>
TNode::TNode(const &KeyElem key, const &Data data):
    key(key), data(data),
    father(nullptr), left_son(nullptr), right_son(nullptr){
}

template<class KeyElem, class Data>
TNode::void refreshBF(){
    int h_l = 0, h_r= 0;
    if(this->left_son){
        h_l = this->left_son.h;
    }
    if(this->right_son){
        h_r = this->right_son.h;
    }

    this->BF = h_l - h_r;
}

