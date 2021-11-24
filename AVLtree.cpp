
#include "AVLtree.h"
#include "./course_files/library1.h"

template<class KeyElem, class Data>
AVLTree::AVLTree(): root(nullptr){}


template<class KeyElem, class Data>
AVLTree::AVL_RESULT& AVLInsert(const &KeyElem key, const &Data data){

    std::shared_ptr<TNode> new_node = new(TNode(key, data));
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

//IGNORE
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

bool brotherExists(TNode* current_node){    //add ptr for error messages
    if(!current_node){
        //return ___
        //asign error to ptr
    }

    auto father_node = current_node.father();
    if(this->key < father_node.key){
        return (father_node.right_son != nullptr);
    }
    if(this->key >= father_node.key){           //we have to make sure its not equal
        return (father_node.left_son != nullptr);
    }
    
}


// tree.AVlFind() = true/false

bool AVLFind(const &KeyElem key_to_find){
    return AVLFind_rec(this->root, key_to_find);
}
bool AVLFind_rec(const TNode* current_node, const &KeyElem key_to_find){

        if(!current_node){
            return false;
        }

        if(current_node->key == key_to_find){
            return true;
        }

        if(key_to_find < current_node->key){
            return (AVLFind_rec(current_node.left_son, key_to_find));
        }
        else{
            return(AVLFind_rec(current_node.right_son, key_to_find));
        }

}

bool 
void AVLRotate_LL_execute(TNode* node_uneven){      //give names to ptrs for clarity

    auto temp_left_right_son = node_uneven->left_son->right_son;

    node_eneven->left_son->father = node_eneven->father;
    if(node_eneven->left_son->key > node_uneven->father->key){
        node_eneven->left_son->father->right_son = node_eneven->left_son;
    }
    else{
        node_eneven->left_son->father->left_son = node_eneven->left_son;
    }
    
    node_eneven->father = node_eneven->left_son;
    
    node_uneven->left_son->right_son = node_eneven;
    node_eneven->left_son = temp_left_right_son;

}