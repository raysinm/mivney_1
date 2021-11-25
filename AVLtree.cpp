
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

StatusType AVLInsert(const &KeyElem key, const &Data data){
    
    if(!key || this->AVLExist(key)){  //checks if key is already in this tree
        return INVALID_INPUT;
    }

    TNode* insert_after_node;
    auto not_used_node = AVLFind_rec(this->root, key, insert_after_node);
    assert(insert_after_node != nullptr); //sanity check

    insert_node = std::make_shared<TNode<KeyElem,Data>>(key, data);
    if(key < insert_after_node->key){
        insert_after_node->left_son = insert_node;
    } 
    else{
        insert_after_node->right_son = insert_node;
    }
    
    insert_node->father = insert_after_node;
/*     insert_node->father->height = 1;    //will update even if it is already 1 (ok)
    
    this->AVLRefreshHeight(insert_node); */

    this->AVLBalance(insert_node);    //SUPER important

}

void AVLBalance(TNode* start){
    
    auto current_node = start
    while(current_node){
        int prev_height = current_node->height;
        this->AVLNodeRefreshHeight(current_node);
        this->AVLNodeRefreshBF(current_node);
        
        if(current_node->height == prev_height){
            break;
        }
        if(current_node->BF == 2 && current_node->left_son->BF >= 0){
            this->AVLRotate_LL(current_node);
        }
        else if(current_node->BF == 2 && current_node->left_son->BF == -1){
            this->AVLRotate_LR(current_node);
        }
        else if(current_node->BF == -2 && current_node->right_son->BF == -1){
            this->AVLRotate_RR(current_node);
        }
        else if(current_node->BF == -2 && current_node->right_son->BF >=0){
            this->AVLRotate_RL(current_node);
        }

        current_node = current_node -> father;
    }

}

/* void AVLBalance_rec(TNode* start)
 */
void AVLNodeRefreshHeight(TNode* node){
    
    if(!node->right_son && !node->left_son){
        node->height = 0;
    }
    else if(!node->right_son){
        node->height = node->left_son->height + 1;
    }
    else if(!node->left_son{
        node->height = node->right_son->height +1;
    }
    else{
        node->height = max(node->left_son->height, node->right_son->height) + 1;
    }
}

void AVLNodeRefreshBF(TNode* node){
    if(!node->right_son && !node->left_son){
        node->BF = 0;
    }
    else if(!node->right_son){
        node->BF = node->left_son->height;
    }
    else if(!node->left_son{
        node->height = 0 - node->right_son->height;
    }
    else{
        node->BF = node->left_son->height - node->right_son->height;
    }
}


template<class KeyElem, class Data>
void AVLScanInOrder(AVLTree& tree){

    AVLScanInOrder_rec(tree.root);
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

//******************_FIND, EXIST_********************//

bool AVLExist(const &KeyElem key_to_find){
    return (AVLFind(key_to_find != nullptr))
}

TNode* AVLFind(const &KeyElem key_to_find){
    return AVLFind_rec(this->root, key_to_find);
}
TNode* AVLFind_rec(const TNode* current_node, const &KeyElem key_to_find, TNode* father_to_ret){

        if(!current_node){
            return nullptr;
        }

        if(current_node->key == key_to_find){
            return &current_node;
        }

        if(key_to_find < current_node->key){
            if(current_node.left_son  == nullptr){  //Knowledge for AVLInsert
                father_to_ret = current_node;       
            }
            return (AVLFind_rec(current_node.left_son, key_to_find));
        }
        else{
            if(current_node.right_son  == nullptr){  //Knowledge for AVLInsert
                father_to_ret = current_node;       
            }
            return(AVLFind_rec(current_node.right_son, key_to_find));
        }
}


//******************_ROTATIONS_********************//

void AVLRotate_LL(TNode* node_uneven){      //give names to ptrs for clarity

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

void AVLRotate_RR(TNode* node_uneven){

    auto temp_right_left_son = node_eneven->right_son->left_son;
    auto right_son = node_eneven->right_son;
    auto right_left_son = node_uneven->right_son->left_son;
    
    right_son->father = node_eneven->father;
    
    right_son->left_son = node_eneven;
    node_eneven->right_son = temp_right_left_son;

    if(right_son->key > right_son->father->key){
        right_son->father->right_son = right_son;
    }
    else{
        right_son->father->left_son = right_son;
    }

    node_eneven->father = right_son;

}

void AVLRotate_LR(TNode* node_uneven){
    AVLRotate_RR(node_eneven->left_son);
    AVLRotate_LL(node_uneven);
}

void AVLRotate_RL(TNode* node_uneven){
    AVLRotate_LL(node_uneven->right_son);
    AVLRotate_RR(node_uneven);
}