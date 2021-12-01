#ifndef _AVLTREE_H
#define _AVLTREE_H


#include <memory>
#include <iostream>
#include <cassert>
#include "./course_files/library1.h"
//#include "TNode.h"

namespace AVL{

    /* typedef enum {
        ,
        _W_CHANGE,
        ,
            AVL_EMPTY,
            AVL_OUT_OF_MEMORY,
            AVL_OTHER_ERROR
    }void; */

    template <class KeyElem, class Data>
    class AVLTree{
        private:

            class TNode{

                friend class AVLTree;
                KeyElem key;
                Data data;
                int BF; //balance factor
                int height; //?

                TNode* father;
                TNode* left_son;
                TNode* right_son;

                public:
                    TNode(const KeyElem& key, const Data& data):
                        key(key), data(data),
                        father(nullptr), left_son(nullptr), right_son(nullptr),
                        height(0), BF(0){};

                    bool operator<(const TNode& other_node){  
                        if (other_node == nullptr || *this == other_node) return false;
                        return (this->key < other_node.key);
                    };

                    bool operator==(const TNode& other_node){
                        return this->key == other_node.key;
                    }
                    
                    bool isRightSon(){
                        if(!this->father) return false;
                        return(this->father->key < this->key);
                    }
                    bool isLeftSon(){
                        if(!this->father) return false;
                        return(this->key < this->father->key);
                    }
                    bool rightSonExists(){
                        return this->right_son;
                    }
                    bool leftSonExists(){
                        return this->left_son;
                    }
            };
            
            TNode* root;

            void AVLDestroy_rec(TNode*);
            void AVLBalance(TNode*);
            bool AVLExist(const KeyElem&);
            TNode* AVLFind_rec(TNode* current_node, const KeyElem& key_to_find, TNode** father_to_ret);
            void AVLRemove_rec(TNode* node, const KeyElem& key);
            void changeInFather(const KeyElem& key, TNode* father, TNode* new_son);
            void AVLNodeRefreshHeight(TNode* node);
            void AVLNodeRefreshBF(TNode* node);
            void changeNodes(TNode* to_remove, TNode* replacer);
            TNode* findReplacingNode(TNode* node);
            void AVLPrintInOrder_rec(TNode* node);

        public:

            AVLTree(): root(nullptr){};
            AVLTree(const AVLTree&); //Should we use user's copy functions?
            ~AVLTree(){
                AVLDestroy_rec(root);
            }; //Should we use user's free functions?

            TNode* AVLFind(const KeyElem& key);
            void AVLInsert(const KeyElem&, const Data&);
            void AVLRemove(const KeyElem&);
            void AVLPrintInOrder();
            //Data& AVLGet(const KeyElem&);

            void AVLRotate_LL(TNode*);
            void AVLRotate_RR(TNode*);
            void AVLRotate_LR(TNode*);
            void AVLRotate_RL(TNode*);
            
    };



    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLInsert(const KeyElem& key, const Data& data){
        
        if(key < 0 || this->AVLExist(key)){  //checks if key is already in this tree
            return;
        }
        //in case exists need to return FAILURE

        TNode* insert_node = new TNode(key, data);

        if(!this->root){
            this->root = insert_node;
            return;
        }

        TNode* insert_after_node = nullptr;
        auto not_used_output = AVLFind_rec(root, key, &insert_after_node);
        //assert(insert_after_node != nullptr); //sanity check

        if(key < insert_after_node->key){
            insert_after_node->left_son = insert_node;
        } 
        else{
            insert_after_node->right_son = insert_node;
        }
        
        insert_node->father = insert_after_node;
        AVLBalance(insert_node->father);    //SUPER important

        return;
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLBalance(TNode* start){
        
        auto current_node = start;
        while(current_node){
            int prev_height = current_node->height;
            AVLNodeRefreshHeight(current_node);
            AVLNodeRefreshBF(current_node);
            
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
            AVLNodeRefreshHeight(current_node);
            AVLNodeRefreshBF(current_node);
            current_node = current_node -> father;
        }
    }

    /* void AVLBalance_rec(std::shared_ptr<TNode*<KeyElem, Data> start)
    */
    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLNodeRefreshHeight(TNode* node){
        
        if(!node->right_son && !node->left_son){
            node->height = 0;
        }
        else if(!node->right_son){
            node->height = node->left_son->height + 1;
        }
        else if(!node->left_son){
            node->height = node->right_son->height +1;
        }
        else{
            node->height = std::max(node->left_son->height, node->right_son->height) + 1;
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLNodeRefreshBF(TNode* node){
        if(!node->right_son && !node->left_son){
            node->BF = 0;
        }
        else if(!node->right_son){
            node->BF = node->left_son->height - (-1);
        }
        else if(!node->left_son){
            node->BF = -1 - node->right_son->height;
        }
        else{
            node->BF = node->left_son->height - node->right_son->height;
        }
    }



    //******************_FIND, EXIST_********************//
    template <class KeyElem, class Data>
    bool AVLTree<KeyElem,Data>:: AVLExist(const KeyElem& key_to_find){
        return (AVLFind(key_to_find) != nullptr);
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::AVLFind(const KeyElem& key_to_find){
        TNode* dummy_ptr;
        return AVLFind_rec(this->root, key_to_find, &dummy_ptr);
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::AVLFind_rec(AVLTree<KeyElem,Data>::TNode* current_node,
                                     const KeyElem& key_to_find, AVLTree<KeyElem,Data>::TNode** father_to_ret){

        if(!current_node){
            return nullptr;
        }

        if(current_node->key == key_to_find){
            return current_node;
        }

        if(key_to_find < current_node->key){
            if(current_node->left_son  == nullptr){  //Knowledge for AVLInsert
                *father_to_ret = current_node;       
            }
            return (AVLFind_rec(current_node->left_son, key_to_find, father_to_ret));
        }
        else{
            if(current_node->right_son  == nullptr){  //Knowledge for AVLInsert
                *father_to_ret = current_node;       
            }
            return(AVLFind_rec(current_node->right_son, key_to_find, father_to_ret));
        }
    }


    //******************_ROTATIONS_********************//

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_LL(TNode* node_uneven){      //give names to ptrs for clarity

        auto temp_left_right_son = node_uneven->left_son->right_son;
        auto left_son = node_uneven->left_son;
        
        left_son->father = node_uneven->father;
        node_uneven->father = left_son;
        left_son->right_son = node_uneven;
        node_uneven->left_son = temp_left_right_son;

        if(this->root == node_uneven){
            this->root = left_son;
            return;
        }

       if(left_son->key < left_son->father->key){
           left_son->father->left_son = left_son;
       }else{
           left_son->father->right_son = left_son;
       }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_RR(TNode* node_uneven){

        auto temp_right_left_son = node_uneven->right_son->left_son;
        auto right_son = node_uneven->right_son;
        
        right_son->father = node_uneven->father;
        
        right_son->left_son = node_uneven;
        node_uneven->right_son = temp_right_left_son;
        node_uneven->father = right_son;

        if(this->root == node_uneven){
            this->root = right_son;
            return;
        }

        if(right_son->key > right_son->father->key){
            right_son->father->right_son = right_son;
        }
        else{
            right_son->father->left_son = right_son;
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_LR(TNode* node_uneven){
        AVLRotate_RR(node_uneven->left_son);
        AVLRotate_LL(node_uneven);
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_RL(TNode* node_uneven){
        AVLRotate_LL(node_uneven->right_son);
        AVLRotate_RR(node_uneven);
    }

    //******************_AVLRemove_********************//

    
    //Recursive Return

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRemove(const KeyElem& key){
        AVLRemove_rec(this->root, key);
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRemove_rec(TNode* node, const KeyElem& key){
        if(!node){
            return;
        }
                                         //ADD METHODS TO TNODE!!!!!!!!!!
        if(node->key == key){
            auto temp_father = node->father;
            if(!node->leftSonExists() && !node->rightSonExists()){  //This is a leaf
                if(node == this->root){
                    this->root = nullptr;
                }
                else if(node->isLeftSon()){
                    node->father->left_son = nullptr; 
                }
                else if(node->isRightSon()){
                    node->father->right_son = nullptr;
                }  
            }
            else if(!node->leftSonExists() || !node->rightSonExists()){  //Has only a right son
                /* TNode* node_sun;;
                if(node->leftSonExists()){
                    node_son = node->left_son;
                }else if(node->rightSonExists()){
                    node_son = node->right_son;
                } */
                TNode* node_son = node->leftSonExists() ? node->left_son : node->right_son;
                
                if(node == this->root){
                    this->root = node_son;
                }
                else if(node->isLeftSon()){
                    node->father->left_son = node_son;
                }
                else if(node->isRightSon()){
                    node->father->right_son = node_son;
                }
                node_son->father = node->father;
            }
            else{   //Has TWO sons
                auto replacer = findReplacingNode(node);    //replacer is the biggest node that is smaller than our node
                
                if(node == this->root){
                        this->root = replacer;
                    }
                    else if(node->isLeftSon()){
                        node->father->left_son = replacer;
                    }
                    else if(node->isRightSon()){
                        node->father->right_son = replacer;
                    }

                auto temp_node_left_son = node->left_son,
                    temp_node_right_son = node->right_son,
                    temp_replacer_left_son = replacer->left_son;
                
                if(node == replacer->father){   // special case. They are direct relatives, and therfore point to eachother
            
                    node->father = replacer;
                    replacer->left_son = node;
                
                }else{
                    node->father = replacer->father;
                    replacer->left_son = temp_node_left_son;
                }
                replacer->father = temp_father;
                node->left_son = temp_replacer_left_son;
                node->right_son = replacer->right_son;
                replacer->right_son = temp_node_right_son;
                replacer->right_son->father = replacer;
                replacer->left_son->father = replacer;

                if(node->leftSonExists() && node->rightSonExists()){ 
                    AVLRemove_rec(this->root, key);
                }
                else{
                    if(node->father == replacer){
                        node->father->left_son = node->left_son;
                    }else node->father->right_son = node->left_son;   //guranteed- has no right son!
                }
                temp_father = node->father;
            }
            
            delete[] node;
            AVLBalance(temp_father);
            return;

        }else if(node->leftSonExists() && key < node->key){
            AVLRemove_rec(node->left_son, key);
        }else if(node->rightSonExists() && node->key < key){
            AVLRemove_rec(node->right_son, key);
        }
        
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::findReplacingNode(AVLTree<KeyElem,Data>::TNode* node){
        auto replacer = node->left_son;
        while(replacer->right_son){
            replacer = replacer -> right_son;
        }
        return replacer;
    }


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

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLPrintInOrder(){
        AVLPrintInOrder_rec(this->root);
    }

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLPrintInOrder_rec(AVLTree<KeyElem,Data>::TNode* node){
        if(!node){
            return;
        }

        AVLPrintInOrder_rec(node->right_son);
        std::cout << node->key << ", " ;
        AVLPrintInOrder_rec(node->left_son);
        
    }
     

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLDestroy_rec(TNode* node){
        if(!node){
            return;
        }
        
        if(node->left_son){
            AVLDestroy_rec(node->left_son);
            node->left_son = nullptr;
        }
        if(node->right_son){
            AVLDestroy_rec(node->right_son);
            node->right_son = nullptr;
        }
        
        delete[] node;
        return;
    }

  /*   template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLPrint_rec(TNode* node){
        if(!node){
            return;
        }
        AVLPrint_rec(node->left_son);
        AVLPrint_rec(node->right_son);
        
    }
 */

}

#endif
