
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



       //FROM OLD REMOVE:

       /* auto orig_remove_left = node_to_remove->left_son,
                 orig_remove_right = node_to_remove->right_son,
                 orig_remove_father = node_to_remove->father,
                 orig_replace_father = replacer->father;


                if(father->key < key){
                 father->right_son = new_son;
                }                  
                else{
                    father->left_son = new_son;
                }
                new_son->father = father;
                
            }
            if(node_to_replace->father == node_to_remove){ //if they replace each other
                
                node_to_replace->father = node_to_remove->father;
                node_to_remove->father = node_to_replace; 
            }
            else{
                changeInFather(key ,node_to_replace->father, node_to_remove);
                changeInFather(key, temp_remove_father, node_to_replace);
            }
            node_to_remove->left_son = node_to_replace->left_son;
            node_to_remove->right_son = node_to_replace->right_son;
            node_to_replace->left_son = temp_remove_left;
            node_to_replace->right_son = temp_remove_right;
            
 */



/* 

    
    //func returnes
    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRemove(const KeyElem& key){
        if(!key || !AVLExist(key)){
            return;
        }
        //in case exists need to return FAILURE
        auto node_to_remove = AVLFind(key);
        AVLRemove_rec(node_to_remove);
        //auto node_to_remove_father = node_to_remove->father;
        
        delete[] node_to_remove;
        //AVLBalance(node_to_remove_father);
        
        return;
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRemove_rec(TNode* node_to_remove){
        auto key = node_to_remove->key;
        if((node_to_remove->right_son == nullptr) && (node_to_remove->left_son == nullptr))
        {
            changeInFather(key, node_to_remove->father, nullptr);
            AVLBalance(node_to_remove->father);
        }
        else if((node_to_remove->right_son == nullptr) || (node_to_remove->left_son == nullptr)){
            if(node_to_remove->right_son == nullptr){
                changeInFather(key, node_to_remove->father, node_to_remove->left_son);
            }
            else{
                changeInFather(key, node_to_remove->father, node_to_remove->right_son);
            }
            AVLBalance(node_to_remove->father);
        }
        else{
            auto node_to_replace = node_to_remove->left_son;
            
            while (node_to_replace->right_son != nullptr){
                node_to_replace = node_to_replace->right_son;
            }
            
            changeNodes(node_to_remove, node_to_replace);
            AVLRemove_rec(node_to_remove);
        }
        //?down
        //need to balance here
        auto node_to_remove_father = node_to_remove->father;
        if(node_to_remove< node_to_remove_father){
            node_to_remove_father->left_son = nullptr;
        }
        else{
            node_to_remove->right_son = nullptr;
        }
        
    }
 */

/* 

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::changeNodes(TNode* to_remove, TNode* replacer){
    
        auto orig_remove_left = to_remove->left_son,
            orig_remove_right = to_remove->right_son,
            orig_remove_father = to_remove->father,
            orig_replace_father = replacer->father;
        
        if(to_remove == replacer->father){  //if they are neighbours and point to each other
            to_remove->father = replacer;
            replacer->left_son = to_remove;
        }
        else{
            to_remove->father = replacer->father;
            replacer->left_son = orig_remove_left;
        }
        replacer->father = orig_remove_father;

        orig_replace_father->right_son = to_remove;
        to_remove->left_son = replacer->left_son;

        to_remove->right_son = replacer->right_son;
        replacer->right_son = orig_remove_right;
        
        if(!replacer->father){
            this->root = replacer;
        }else if(replacer < replacer->father){
            replacer->father->left_son = replacer;
        }else{
            replacer->father->right_son = replacer;
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: changeInFather(const KeyElem& key, TNode* father, TNode* new_son){
        if(father == nullptr)
        {
            this->root = new_son;
        }
        else if(father->key < key){
            father->right_son = new_son;
        }                  
        else{
            father->left_son = new_son;
        }
        if(new_son != nullptr){
            new_son->father = father;
        }
        
    } 
 */

   //for this func I need AVL funcs that return to me data of left & right sons
   //data might not be good enought to know where am I in tree
  /*  void PlayersManager:: InOrderRec(GroupData *group, int **Players, int numOfGroups, int count){
        if(count == 0 || group == nullptr) return;
        
        GroupData *leftGroup = (get group on the left);
        InorderRec(leftGroup, Players, numOfGroups, count);

        PlayerId bestPlayer = group.best_in_group;
        Players[count] = bestPlayer;
        count++;

        GroupData *rightGroup = (get group on the right);
        InorderRec(rightGroup, Players, numOfGroups, count);
        
    }
    */