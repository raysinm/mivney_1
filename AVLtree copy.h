
#ifndef _AVLTREE_H
#define _AVLTREE_H

#include <memory>
#include "./course_files/library1.h"
#include "TNode.h"

namespace AVL{

    template <class KeyElem, class Data>
    class AVLTree{
        
        private:
            friend TNode;
            
            TNode<KeyElem,Data>* root;

            enum AVL_RESULT{
                AVL_SUCCESS,
                AVL_SUCCESS_W_CHANGE,
                AVL_EMPTY,
                AVL_OUT_OF_MEMORY,
                AVL_OTHER_ERROR
            };

            void AVLBalance(TNode<KeyElem,Data>*);
            void AVLExist(const KeyElem&);
            TNode<KeyElem,Data>* AVLFind_rec(const TNode<KeyElem,Data>* current_node, const KeyElem& key_to_find, TNode<KeyElem,Data>* father_to_ret);


        public:
            AVLTree(): root(nullptr);
            AVLTree(const AVLTree&); //Should we use user's copy functions?
            ~AVLTree(); //Should we use user's free functions?

            AVL_RESULT& AVLFind(const KeyElem& key);
            AVL_RESULT& AVLInsert(const KeyElem&, const Data&);
            AVL_RESULT& AVLRemove(const KeyElem&);
            //Data& AVLGet(const KeyElem&);

            void AVLRotate_LL();
            void AVLRotate_RR();
            void AVLRotate_LR();
            void AVLRotate_RL();
            
    };



    template <class KeyElem, class Data>
    StatusType AVLInsert(const KeyElem& key, const Data& data){
        
        if(!key || this->AVLExist(key)){  //checks if key is already in this tree
            return INVALID_INPUT;
        }

        TNode<KeyElem,Data>* insert_after_node;
        auto not_used_node = AVLFind_rec(this->root, key, insert_after_node);
        assert(insert_after_node != nullptr); //sanity check

        TNode<KeyElem,Data>* insert_node(key, data);
        if(key < insert_after_node->key){
            insert_after_node->left_son = insert_node;
        } 
        else{
            insert_after_node->right_son = insert_node;
        }
        
        insert_node->father = insert_after_node;
        this->AVLBalance(insert_node->father);    //SUPER important

        return SUCCESS;
    }

    template <class KeyElem, class Data>
    void AVLBalance(TNode<KeyElem,Data>* start){
        
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

    /* void AVLBalance_rec(std::shared_ptr<TNode<KeyElem,Data>*<KeyElem, Data> start)
    */
   template <class KeyElem, class Data>
    void AVLNodeRefreshHeight(TNode<KeyElem,Data>* node){
        
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

    template <class KeyElem, class Data>
    void AVLNodeRefreshBF(TNode<KeyElem,Data>* node){
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


 /*     template<class KeyElem, class Data>
    void AVLScanInOrder(AVLTree& tree){

        AVLScanInOrder_rec(tree.root);
    }
 */


    //******************_FIND, EXIST_********************//
    template <class KeyElem, class Data>
    bool AVLExist(const KeyElem& key_to_find){
        return (AVLFind(key_to_find != nullptr));
    }

    template <class KeyElem, class Data>
    TNode<KeyElem,Data>* AVLFind(const KeyElem& key_to_find){
        return AVLFind_rec(this->root, key_to_find);
    }

    template <class KeyElem, class Data>
    TNode<KeyElem,Data>* AVLFind_rec(const TNode<KeyElem,Data>* current_node,
                        const KeyElem& key_to_find, TNode<KeyElem,Data>* father_to_ret){

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

    template <class KeyElem, class Data>
    void AVLRotate_LL(TNode<KeyElem,Data>* node_uneven){      //give names to ptrs for clarity

        auto temp_left_right_son = node_uneven->left_son->right_son;

        node_eneven.left_son->father = node_eneven.father;
        if(node_eneven.left_son->key > node_uneven.father->key){
            node_eneven.left_son->father->right_son = node_eneven.left_son;
        }
        else{
            node_eneven.left_son->father->left_son = node_eneven.left_son;
        }
        
        node_eneven.father = node_eneven.left_son;
        
        node_uneven.left_son->right_son = node_eneven;
        node_eneven.left_son = temp_left_right_son;

    }

    template <class KeyElem, class Data>
    void AVLRotate_RR(TNode<KeyElem,Data>* node_uneven){

        auto temp_right_left_son = node_eneven.right_son->left_son;
        auto right_son = node_eneven.right_son;
        auto right_left_son = node_uneven.right_son->left_son;
        
        right_son->father = node_eneven->father;
        
        right_son->left_son = node_eneven;
        node_eneven->right_son = temp_right_left_son;

        if(right_son->key > right_son->father->key){
            right_son->father->right_son = right_son;
        }
        else{
            right_son->father->left_son = right_son;
        }

        node_eneven.father = right_son;

    }
    template <class KeyElem, class Data>
    void AVLRotate_LR(TNode<KeyElem, Data>* node_uneven){
        AVLRotate_RR(node_eneven.left_son);
        AVLRotate_LL(node_uneven);
    }

    template <class KeyElem, class Data>
    void AVLRotate_RL(TNode<KeyElem, Data>* node_uneven){
        AVLRotate_LL(node_uneven.right_son);
        AVLRotate_RR(node_uneven);
    }

    //******************_AVLRemove_********************//



    template <class KeyElem, class Data>
    AVL_RESULT& AVLRemove(const KeyElem& key){
    
        if(!key || !this->AVLExist(key)){
            return AVL_INVALID_INPUT;
        } 

        TNode<KeyElem,Data>* node_to_remove_father;
        auto node_to_remove = AVLFind(KeyElem, node_to_remove_father);

        //WHY WE SKIPPED THE LOOP: if the node has no sons at all, it will just give the father a nullptr anyway.
        if(node_to_remove->right_son == nullptr || node_to_remove->left_son == nullptr){
            if(node_to_remove_father < node_to_remove){

                if(!node_to_remove->right_son){
                    node_to_remove_father->right_son = node_to_remove->left_son;
                }
                else{
                    node_to_remove_father->right_son = node_to_remove->right_son;
                }
            }
            else{ 
                if(!node_to_remove->right_son){
                    node_to_remove_father->left_son = node_to_remove->left_son;
                
                }else{
                    node_to_remove_father->left_son = node_to_remove->right_son;
                }    
            }
        }

        else{
            auto node_to_replace = node_to_remove->left_son;
            
            while (node_to_replace->right_son != nullptr){ //finding biggest node from left tree
                node_to_replace = node_to_replace->right_son;
            }

            if(node_to_remove_father->key < node_to_remove->key){   //if our node is the right son of the father
                node_to_remove_father->right_son = node_to_replace;
            }
            else{
                node_to_remove->father->left_son = node_to_replace;
            }
            this->AVLRemove(node_to_replace->key);  //to delete the replacing node recursively by the rules
            
        }
        
        delete[] node_to_remove;
        AVLBalance(node_to_remove_father);
        return AVL_SUCCESS;
    }

}

#endif
