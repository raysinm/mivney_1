
/* bool AVLExist(const &KeyElem key_to_find){
    return AVLExist_rec(this->root, key_to_find);
}
bool AVLExist_rec(const TNode* current_node, const &KeyElem key_to_find){

        if(!current_node){
            return false;
        }

        if(current_node->key == key_to_find){
            return true;
        }

        if(key_to_find < current_node->key){
            return (AVLExist_rec(current_node.left_son, key_to_find));
        }
        else{
            return(AVLExist_rec(current_node.right_son, key_to_find));
        }

} */



//IGNORE
//SUPER Generic inorder scan (can add other stuff to it)
/* void AVLScanInOrder_rec(TNode* node){
    if (node.height() == 0){
        //do something with leaf
        return;
    }      //height(v) = max[h(Lv),h(Rv)] +1
    
    AVLScanInOrder_rec(node.left);
    AVLScanInOrder_rec(node.right);

    // do something();
} */


/* 
    template<class KeyElem, class Data>
    AVL_RESULT& AVLInsert(const KeyElem& key, const Data& data){

        std::shared_ptr<TNode*> new_node = new(TNode*(key, data));
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
 */


/* if((node_to_remove->right_son == nullptr) && (node_to_remove->left_son == nullptr)){
            if(node_to_remove_father->key < node_to_remove->key){
                node_to_remove_father->right_son = nullptr; //do we need to? yes!
            }
            else{
                node_to_remove_father->left_son = nullptr; //do we need to?
            }
            delete[] node_to_remove;
            AVLBalance(node_to_remove_father);
        }
        */