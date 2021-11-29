#ifndef _AVLTREE_H
#define _AVLTREE_H

#include <memory>
#include <cassert>
#include "./course_files/library1.h"
//#include "TNode.h"

namespace AVL{

    typedef enum {
            AVL_SUCCESS,
            AVL_SUCCESS_W_CHANGE,
            AVL_INVALID_INPUT,
            AVL_EMPTY,
            AVL_OUT_OF_MEMORY,
            AVL_OTHER_ERROR
    }AVL_RESULT;

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
                        if (other_node == nullptr || *this == other_node){
                            return false;
                        }
                        return (this->key < other_node.key);
                    };

                    bool operator==(const TNode& other_node){
                        return this->key == other_node.key;
                    }
                    
                    void brotherExists(){
                        auto father_node = this->father;
                        if(this->key < father_node.key){
                            return (father_node.right_son != nullptr);
                        }
                        else{           //we have to make sure its not equal
                            return (father_node.left_son != nullptr);
                        }
                    }
            };
            
            TNode* root;

            void AVLDestroy_rec(TNode*);
            void AVLBalance(TNode*);
            bool AVLExist(const KeyElem&);
            TNode* AVLFind_rec(TNode* current_node, const KeyElem& key_to_find, TNode** father_to_ret);
            void AVLRemove_rec(TNode*);
            void changeInFather(const KeyElem& key, TNode* father, TNode* new_son);
            void AVLNodeRefreshHeight(TNode* node);
            void AVLNodeRefreshBF(TNode* node);

        public:

            AVLTree(): root(nullptr){};
            AVLTree(const AVLTree&); //Should we use user's copy functions?
            ~AVLTree(){
                AVLDestroy_rec(root);
            }; //Should we use user's free functions?

            TNode* AVLFind(const KeyElem& key);
            AVL_RESULT AVLInsert(const KeyElem&, const Data&);
            AVL_RESULT AVLRemove(const KeyElem&);
            
            //Data& AVLGet(const KeyElem&);

            void AVLRotate_LL(TNode*);
            void AVLRotate_RR(TNode*);
            void AVLRotate_LR(TNode*);
            void AVLRotate_RL(TNode*);
            
    };



    template <class KeyElem, class Data>
    AVL_RESULT AVLTree<KeyElem,Data>:: AVLInsert(const KeyElem& key, const Data& data){
        
        if(!key || this->AVLExist(key)){  //checks if key is already in this tree
            return AVL_INVALID_INPUT;
        }

        TNode* insert_node = new TNode(key, data);

        if(!this->root){
            this->root = insert_node;
            return AVL_SUCCESS;
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

        return AVL_SUCCESS;
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

      
    
    //func returnes
    template <class KeyElem, class Data>
    AVL_RESULT AVLTree<KeyElem,Data>:: AVLRemove(const KeyElem& key){
        if(!key || !AVLExist(key)){
            return AVL_INVALID_INPUT;
        }
        auto node_to_remove = AVLFind(key);
        AVLRemove_rec(node_to_remove);
        return AVL_SUCCESS;
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRemove_rec(TNode* node_to_remove){
        auto key = node_to_remove->key;
        if((node_to_remove->right_son == nullptr) && (node_to_remove->left_son == nullptr))
        {
            changeInFather(key, node_to_remove->father, nullptr);
        }
        else if((node_to_remove->right_son == nullptr) || (node_to_remove->left_son == nullptr)){
            if(node_to_remove->right_son == nullptr){
                changeInFather(key, node_to_remove->father, node_to_remove->left_son);
            }
            else{
                changeInFather(key, node_to_remove->father, node_to_remove->right_son);
            }
        }
        else{
            auto node_to_replace = node_to_remove->left_son,
                node_to_remove_father = node_to_remove->father,
                node_to_remove_right = node_to_remove->right_son,
                node_to_remove_left = node_to_remove->left_son;
            
            while (node_to_replace->right_son != nullptr){
                node_to_replace = node_to_replace->right_son;
            }

            changeInFather(key ,node_to_replace->father, node_to_remove);
            node_to_remove->left_son = node_to_replace->left_son;
            node_to_remove->right_son = nullptr;

            changeInFather(key,node_to_remove_father,node_to_replace);
            node_to_replace->right_son = node_to_remove_right;
            node_to_replace->left_son = node_to_remove_left;

            AVLRemove_rec(node_to_remove);
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: changeInFather(const KeyElem& key, TNode* father, TNode* new_son){
        if(father == nullptr)
        {
            this->root = new_son; //this.root?
        }
        else if(father->key < key){
            father->right_son = new_son;
        }                  
        else{
            father->left_son = new_son;
        }
        AVLBalance(new_son);
    } 
    

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLDestroy_rec(TNode* node){
        if(!node){
            return;
        }
        
        AVLDestroy_rec(node->left_son);
        AVLDestroy_rec(node->right_son);
        delete[] node;
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
