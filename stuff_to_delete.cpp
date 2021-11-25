
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
