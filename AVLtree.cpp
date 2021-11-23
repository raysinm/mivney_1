
#include "AVLtree.h"
#include "./course_files/library1.h"

template<class KeyElem, class Data>
AVLTree::AVLTree(): root(nullptr){}


template<class KeyElem, class Data>
AVLTree::AVL_RESULT& AVLInsert(const &KeyElem key, const &Data data){

    std::smart_ptr<TNode> new_node = new(TNode(key, data));
    bool node_exists = false;
    
    if(this->root == nullptr){
        root = new_node;
        return AVL_SUCCESS;    
    }
    else{
        auto father = AVLSearchFather(new_node, &node_exists);
        if (node_exists == true){
            father.data = new_node.data;
            return AVL_SUCCESS_W_CHANGE;
        }
        else{
            if(father.right > new_node){
                //rotate LL or RL ?????????
            }
            else{
                //rotate RR or LR ?????????????????????
            }
        }
    }

}

template<class KeyElem, class Data>
void AVLScanInOrder(AVLTree& tree){

    AVLScanInOrder_rec(tree.root);
}

//SUPER Generic inorder scan (can add other stuff to it)
void AVLScanInOrder_rec(TNode* node){
    if (node.height() == 0){
        //do something with leaf
        return;
    }      //height(v) = max[h(Lv),h(Rv)] +1
    
    AVLScanInOrder_rec(node.left);
    AVLScanInOrder_rec(node.right);

    // do something();
}