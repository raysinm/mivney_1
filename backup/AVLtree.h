#ifndef _AVLTREE_H
#define _AVLTREE_H

//stav shalom shalom
#include <memory>
#include <iostream>
#include <cassert>
#include <math.h>
#include "./course_files/library1.h"

namespace AVL{
    
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
                TNode* left_son; //smaller
                TNode* right_son; //bigger

                public:
                    TNode(const KeyElem& key, const Data& data):
                        key(key), data(data),
                        father(nullptr), left_son(nullptr), right_son(nullptr),
                        height(0), BF(0){};

                  //  TNode(const TNode&) = delete; //added to find use of copy constructor

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
                    TNode* nextInOrder(){
                        TNode* next_node = this;
                        if(next_node->right_son){
                            next_node = next_node->right_son;
                            while(next_node->left_son){
                                next_node = next_node->left_son;
                            }
                            return next_node;
                        }
                        while(1){
                            if(next_node->father == nullptr){
                                next_node = nullptr;
                                return next_node;
                            }
                            if(next_node->isLeftSon()){
                                next_node = next_node->father;
                                return next_node;
                            }
                            next_node = next_node->father;
                        }
                    }
            };
            
            TNode* root;
            int tree_size;

            void AVLBalance(TNode*);
            void AVLRotate_LL(TNode*);
            void AVLRotate_RR(TNode*);
            void AVLRotate_LR(TNode*);
            void AVLRotate_RL(TNode*);

            TNode* AVLFind(const KeyElem& key) const;
            TNode* AVLFind_rec(TNode* current_node, const KeyElem& key_to_find, TNode** father_to_ret) const;
            void AVLRemove_rec(TNode* node, const KeyElem& key);
            void AVLNodeRefreshHeight(TNode* node) const;
            void AVLNodeRefreshBF(TNode* node) const;
            TNode* findReplacingNode(TNode* node) const;
            void AVLPrintInOrder_rec(TNode* node) const;
            void MergeArray(AVLTree<KeyElem,Data>::TNode** arr1, const int arr1_size,
                                 AVLTree<KeyElem,Data>::TNode** arr2, const int arr2_size, AVLTree<KeyElem,Data>::TNode** merged_arr);
            TNode* ArrayToAVLTree(TNode** array, int start, int end, TNode* father);
            //void InOrderOutputDatas_rec(TNode* node, Data** arr, int& arr_index, const int arr_size);
            void InOrderOutputTNodes_rec(TNode* node, TNode** arr, int& arr_index, const int arr_size);
            TNode* AVLGetFirst() const;
            void AVLDestroy_rec(TNode*) const;

    

            public:

            class Iterator{
                TNode* current_node;
                public:
                    Iterator(TNode* t): current_node(t){};
                    // void begin(){
                    //     current_node = tree.AVLGetFirst();
                    // }
                    // void end(){
                    //     current_node = nullptr;
                    // }
                    Data& operator*(){
                        return current_node->data;
                    }
                    Iterator& operator++(){
                        current_node = current_node->nextInOrder();
                        return *this;
                    }
                    friend bool operator==(const Iterator& it1, const Iterator& it2){
                        return it1.current_node == it2.current_node;
                    }
                    friend bool operator!=(const Iterator& it1, const Iterator& it2){
                        return !(it1 == it2);
                    }

            };
            class ConstIterator{
                TNode* current_node;
                public:
                    ConstIterator(TNode* t): current_node(t){};
                    // void begin(){
                    //     current_node = tree.AVLGetFirst();
                    // }
                    // void end(){
                    //     current_node = nullptr;
                    // }
                    const Data& operator*(){
                        return current_node->data;
                    }
                    Iterator& operator++(){
                        current_node = current_node->nextInOrder();
                        return *this;
                    }
                    friend bool operator==(const ConstIterator& it1, const ConstIterator& it2){
                        return it1.current_node == it2.current_node;
                    }
                    friend bool operator!=(const ConstIterator& it1, const ConstIterator& it2){
                        return !(it1 == it2);
                    }

            };
            
            Iterator begin(){
                return Iterator(AVLGetFirst());
            }
            Iterator end(){
                return Iterator(nullptr);
            }

            ConstIterator begin() const{
                return ConstIterator(AVLGetFirst());
            }
            ConstIterator end() const{
                return ConstIterator(nullptr);
            }


            AVLTree(): root(nullptr), tree_size(0){};
            AVLTree(const AVLTree& other): AVLTree(){
                if(other.tree_size != 0) return;
                this->root = other.root;
                this->tree_size = other.tree_size;
            } //needed.
            ~AVLTree(){
                AVLDestroy_rec(root);
                root = nullptr;
            };

            bool AVLExist(const KeyElem&) const;
            void AVLInsert(const KeyElem&, const Data&);
            void AVLRemove(const KeyElem&);
            void AVLMerge(AVLTree<KeyElem,Data>& other_tree);
            Data& AVLGet(const KeyElem& key) const;
            int size() const;
            KeyElem& AVLMax() const;
            void AVLPrintInOrder() const;
            //void InOrderOutputDatas(Data** arr, const int arr_size);
            void printTree();
            void printTree_rec(const std::string& prefix, const TNode* node, bool isLeft);
            void printTreeData();
            void printTreeData_rec(const std::string& prefix, const TNode* node, bool isLeft);
            void printFather(const KeyElem& key);

    };


    //___________***___PUBLIC FUNCTIONS IMPLEMENTATION___***___________//


    /**
     * @brief checks if an element with a specified key exists in tree
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @param key_to_find input key
     * @return true 
     * @return false 
     */
    template <class KeyElem, class Data>
    bool AVLTree<KeyElem,Data>:: AVLExist(const KeyElem& key_to_find) const{
        return (AVLFind(key_to_find) != nullptr);
    }

    /**
     * @brief inserts a new node with input key and data to the tree. 
            if it causees tree imbalance- it calls the balancing function.
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @param key unique key of element to insert
     * @param data user input data for the element
     */
    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLInsert(const KeyElem& key, const Data& data){
        try{
            TNode* insert_node = new TNode(key, data);

            if(!this->root){
                this->root = insert_node;
                tree_size++;
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
            tree_size++;

        }catch(std::bad_alloc &e){
            throw(e);
        }
    }
    
    /**
     * @brief removes an element from tree
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @param key -key of element the user wants to remove
     */
    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRemove(const KeyElem& key){
        AVLRemove_rec(this->root, key);
        tree_size--; 
    }

    /**
     * @brief merges a tree to this tree, by re-arranging the TNode pointers. 
     *        the merged tree is not usable after 
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @param other_tree -tree to merge with this tree
     */
    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::AVLMerge(AVLTree<KeyElem,Data>& other_tree){
        //allocating two arrays for merging
        TNode** tree1_arr;
        TNode** other_tree_arr;
        TNode** merged_arr; 
        try{
        tree1_arr = new TNode*[this->tree_size];
        other_tree_arr = new TNode*[other_tree.tree_size];
        merged_arr = new TNode*[this->tree_size + other_tree.tree_size];

        int i1 = 0, i2 = 0;
        /* std::cout << "\nthis tree: " << std::endl;
        this->printTree();
        std::cout << "\nother tree: " << std::endl;
        other_tree.printTree(); */

        InOrderOutputTNodes_rec(this->root, tree1_arr, i1, this->tree_size);
        InOrderOutputTNodes_rec(other_tree.root, other_tree_arr, i2, other_tree.tree_size);

        /* for(int i=0; i< this->tree_size; i++){
            std::cout << tree1_arr[i]->key <<", " << std::endl;
        }
        std::cout << "\n" << std::endl;
        for(int i=0; i< other_tree.tree_size; i++){
            std::cout << other_tree_arr[i]->key <<", " << std::endl;
        } */

        this->root = nullptr;
        other_tree.root = nullptr;
        MergeArray(tree1_arr, this->tree_size, other_tree_arr, other_tree.tree_size, merged_arr);

        this->root = this->ArrayToAVLTree(merged_arr, 0, (this->tree_size + other_tree.tree_size - 1), nullptr);
        this->tree_size += other_tree.tree_size;
        
        //other_tree.root = nullptr;
        delete[] tree1_arr;
        delete[] other_tree_arr;
        delete[] merged_arr;
        tree1_arr = nullptr;
        other_tree_arr = nullptr;
        merged_arr = nullptr;
        }
        catch(std::bad_alloc &e){
            delete[] tree1_arr;
            delete[] other_tree_arr;
            delete[] merged_arr;
            throw(e);
        }
    }

    /**
     * @brief returns reference to data inside an element with a certain key
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @param key -key of the element whose data is returned
     * @return Data& -a reference to the found data is returned
     */
    template<class KeyElem, class Data>
    Data& AVLTree<KeyElem,Data>:: AVLGet(const KeyElem& key) const{   //return data by reference or as a ptr?
        return AVLFind(key)->data;
    }

    /**
     * @brief returns the number of elements in this tree
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @return int 
     */
    template<class KeyElem, class Data>
    int AVLTree<KeyElem,Data>::size() const{
            return this->tree_size;
    }

    /**
     * @brief returns reference to the key to the maximum element in tree
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @return const KeyElem& 
     */
    template<class KeyElem, class Data>
    KeyElem& AVLTree<KeyElem,Data>::AVLMax() const{
        auto* current = this->root; //added *
        while(current->left_son){
            current = current->left_son;
        }
        return current->key;
    }

    /**
     * @brief prints the keys of the tree elements in Inorder order. uses a private recurcisve function
     * 
     * @tparam KeyElem 
     * @tparam Data 
     */
    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLPrintInOrder() const{
        AVLPrintInOrder_rec(this->root);
    }

    /**
     * @brief recieves an empty array of pointers to generic Data type. fills the array with pointers to 
    *           tree's elments datas in Inorder order.
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @param arr - empty, allocated array from user
     * @param arr_size -input array size
     */
    /* template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: InOrderOutputDatas(Data** arr, const int arr_size){
        int& index = 0;
        InOrderOutputDatas_rec(this->root, arr, index, arr_size);
    } */

    
    //___________***___PRIVATE FUNCTIONS IMPLEMENTATION___***___________//
    
    
    /**
     * @brief balances the tree. goes through search root of the "start" node, upwards.
     *          if Balance factor of a certain node exceeds the legal values, it performs a rotation.
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @param start -node to start balancing from
     */
    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLBalance(TNode* start){
        
        auto current_node = start;
        while(current_node){
            int prev_height = current_node->height;
            AVLNodeRefreshHeight(current_node);
            AVLNodeRefreshBF(current_node);
            
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
            AVLNodeRefreshHeight(current_node->left_son);
            AVLNodeRefreshHeight(current_node->right_son);
            AVLNodeRefreshHeight(current_node);
            AVLNodeRefreshBF(current_node->left_son);
            AVLNodeRefreshBF(current_node->right_son);
            AVLNodeRefreshBF(current_node);
            if(current_node->right_son && abs(current_node->right_son->BF) > 1){
                AVLBalance(current_node->right_son);
            }
            
            if(current_node->left_son && abs(current_node->left_son->BF) > 1){
                AVLBalance(current_node->left_son);
            }
            if(current_node->height == prev_height){
                break;
            }
            current_node = current_node -> father;
            
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLNodeRefreshHeight(TNode* node) const{
        if(!node) return;

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
    void AVLTree<KeyElem,Data>:: AVLNodeRefreshBF(TNode* node) const{
        if(!node) return;
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
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::AVLFind(const KeyElem& key_to_find) const{
        TNode* dummy_ptr;
        return AVLFind_rec(this->root, key_to_find, &dummy_ptr);
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::AVLFind_rec(AVLTree<KeyElem,Data>::TNode* current_node,
                                     const KeyElem& key_to_find, AVLTree<KeyElem,Data>::TNode** father_to_ret) const{

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
    void AVLTree<KeyElem,Data>:: AVLRotate_LL(TNode* node_uneven) {      //give names to ptrs for clarity

        TNode* temp_left_right_son = node_uneven->left_son->right_son; //added &
        TNode* left_son = node_uneven->left_son; //added &
        
        
        left_son->father = node_uneven->father;
        left_son->right_son = node_uneven;
        if(temp_left_right_son) temp_left_right_son->father = node_uneven;
        node_uneven->left_son = temp_left_right_son;
        node_uneven->father = left_son;
        
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

        TNode* temp_right_left_son = (node_uneven->right_son->left_son);
        TNode* right_son = node_uneven->right_son;
        
        right_son->father = node_uneven->father;
        right_son->left_son = node_uneven;
        if(temp_right_left_son) temp_right_left_son->father = node_uneven;
        node_uneven->right_son = temp_right_left_son;
        node_uneven->father = right_son;

        if(this->root == node_uneven){
            this->root = right_son;
            return;
        }

        if(right_son->father->key < right_son->key){
            right_son->father->right_son = right_son;
        }
        else{
            right_son->father->left_son = right_son;
        }
        
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_LR(TNode* node_uneven){
        AVLRotate_RR(node_uneven->left_son);
        //this->printTree();
        AVLRotate_LL(node_uneven);
        //this->printTree();

    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_RL(TNode* node_uneven){
        AVLRotate_LL(node_uneven->right_son);
        //this->printTree();
        AVLRotate_RR(node_uneven);
        //this->printTree();
    }

    //******************_AVLRemove_********************//

    

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
                AVLBalance(temp_father);
                delete node;
                return;

            }
            else if(!node->leftSonExists() || !node->rightSonExists()){  //Has only a right son or a left son
            
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
                AVLBalance(temp_father);
                delete node;
                return;
            }
            else if(node->leftSonExists() && node->rightSonExists()){   //Has TWO sons
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
                

                TNode* temp_node_left_son = node->left_son;
                TNode* temp_node_right_son = node->right_son;
                TNode* temp_replacer_left_son = replacer->left_son;
                
                if(node == replacer->father){   // special case. They are direct relatives, and therfore point to eachother

                    replacer->father = temp_father;
                    node->father = replacer;
                    replacer->left_son = node;
                    temp_father = node->father;
                }else{
                    auto temp_replacer_father = replacer->father;
                    replacer->father = node->father;
                    node->father = temp_replacer_father;
                    replacer->left_son = temp_node_left_son;
                    
                }
                
                node->left_son = temp_replacer_left_son;
                node->right_son = replacer->right_son; 
                replacer->right_son = temp_node_right_son;
                replacer->right_son->father = replacer;
                replacer->left_son->father = replacer;
                replacer->BF = node->BF;
                replacer->height = node->height;
                AVLNodeRefreshHeight(node);
                AVLNodeRefreshBF(node);

                //this->printTree();
                if(!node->leftSonExists() && !node->rightSonExists()){  //This is a leaf
                    
                    if(node->father == replacer){
                        node->father->left_son = nullptr;
                    }
                    else{
                        node->father->right_son = nullptr;
                    }
                    AVLBalance(node->father);
                    //this->printTree();
                    delete node;
                    return;
                }

                if(!node->leftSonExists() || !node->rightSonExists()){  //Has only a right son or a left son
            
                    TNode* node_son = node->leftSonExists() ? node->left_son : node->right_son;
                    

                    if(node->father == replacer){
                        node->father->left_son = node_son;
                    }
                    else{
                        node->father->right_son = node_son;
                    }
                    node_son->father = node->father;
                }
                AVLBalance(node->father);
                //this->printTree();
                delete node;
                return;
            }
            

        }else if(node->leftSonExists() && key < node->key){
            AVLRemove_rec(node->left_son, key);
        }else if(node->rightSonExists() && node->key < key){
            AVLRemove_rec(node->right_son, key);
        }
        
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::findReplacingNode(AVLTree<KeyElem,Data>::TNode* node) const{
        auto replacer = node->left_son;
        while(replacer->right_son){
            replacer = replacer -> right_son;
        }
        return replacer;
    }
    

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLPrintInOrder_rec(AVLTree<KeyElem,Data>::TNode* node) const{
        if(!node){
            return;
        }

        AVLPrintInOrder_rec(node->left_son);
        std::cout << node->key << ", " ;
        AVLPrintInOrder_rec(node->right_son);
        
    }
     
    

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLDestroy_rec(TNode* node) const{
        if(!node){
            return;
        }
        
        if(node->left_son){
            AVLDestroy_rec(node->left_son);
            //node->left_son = nullptr;
        }
        if(node->right_son){
            AVLDestroy_rec(node->right_son);
            //node->right_son = nullptr;
        }
        delete node;
        return;
    }

    //******************_AVLMerge_********************//

    

    

    /* template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: InOrderOutputDatas_rec(TNode* node, Data** arr, int& arr_index, const int arr_size){
        if(!node || arr_index == arr_size){    //if group is empty
            return;
        }
        InOrderOutputDatas_rec(node->left_son, arr, arr_index, arr_size);
        if(node->data.isOk()){
            arr[arr_index] = &node->data;
            arr_index++;
        }
        InOrderOutputDatas_rec(node->right_son, arr, arr_index, arr_size);
        
        
    } */



    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: InOrderOutputTNodes_rec(TNode* node, TNode** arr, int& arr_index, const int arr_size){
        if(!node || arr_index == arr_size){
            return;
        }
        InOrderOutputTNodes_rec(node->left_son, arr, arr_index, arr_size);
        arr[arr_index] = node;
        arr_index++;
        /* if(node->isLeftSon()){
            node->father->left_son = nullptr;
        }else if(node->isRightSon()){
            node->father->right_son = nullptr;
        } */

        InOrderOutputTNodes_rec(node->right_son, arr, arr_index, arr_size);
        node = nullptr;
    }

    template<class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::ArrayToAVLTree(AVLTree<KeyElem,Data>::TNode** array,
                                                        int start, int end, AVLTree<KeyElem,Data>::TNode* father){
        if(end < start || start > end){
            return nullptr;
        }
        int middle = (start + end)/2;
        TNode* current_root = array[middle];
        current_root->father = father;
        current_root->left_son = ArrayToAVLTree(array, start, middle - 1, current_root);
        current_root->right_son = ArrayToAVLTree(array, middle + 1, end, current_root);
        AVLNodeRefreshHeight(current_root);
        AVLNodeRefreshBF(current_root);
        
        return current_root;
    }



    //recieves pointer to merged_arr so will update without having to return?  -should do just that
    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::MergeArray(typename AVLTree<KeyElem,Data>::TNode** arr1, const int arr1_size, 
                    typename AVLTree<KeyElem,Data>::TNode** arr2, const int arr2_size, typename AVLTree<KeyElem,Data>::TNode** merged_arr){
        int index = 0;
        int index1 = 0;
        int index2 = 0;
        while((index1 < arr1_size) && (index2 < arr2_size)){
            if(arr1[index1]->key < arr2[index2]->key){
                merged_arr[index] = arr1[index1];
                index1++;
            }
            else{
                merged_arr[index] = arr2[index2];
                index2++;
            }
            //std::cout << merged_arr[index]->key << std::endl;
            index++;
            
            //did not include arr1[index1] == arr2[index2] because not possible
        }
        if(index1 < arr1_size){
            while(index1 < arr1_size){
                merged_arr[index] = arr1[index1];
                index1++;
                index++;
            }
        }
        if(index2 < arr2_size){
            while(index2 < arr2_size){
                merged_arr[index] = arr2[index2];
                index2++;
                index++;
            }
        }
    }

    /**
     * @brief returns pointer to minimal element in tree
     * 
     * @tparam KeyElem 
     * @tparam Data 
     * @return AVLTree<KeyElem,Data>::TNode* 
     */
    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>:: AVLGetFirst() const{
        if(!this->root) return nullptr;
        auto* node = this->root; //added *
        while(node->left_son){
            node = node->left_son;
        }
        return node;
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::printTree(){
        std::cout << "\n" << std::endl;
        this->printTree_rec("", this->root, false);
        std::cout << "\n" << std::endl;
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::printTree_rec(const std::string& prefix, const AVLTree<KeyElem,Data>::TNode* node, bool isLeft){
    if( node != nullptr ){
        std::cout << prefix;

        std::cout << (isLeft ? "├──────" : "└──────" );
        //      << " player group : " <<(node->player_data->playing_group)
        //         print the value of the node  
        std::cout << "id: " << node->key  << " (" << node->BF << ") " << std::endl;//" height is "<< node->height << std::endl;

        //         enter the next tree level - left and right branch
        printTree_rec( prefix + (isLeft ? "│       " : "        "), node->right_son, true);
        printTree_rec( prefix + (isLeft ? "│       " : "        "), node->left_son, false);
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::printTreeData(){
        std::cout << "\n" << std::endl;
        this->printTreeData_rec("", this->root, false);
        std::cout << "\n" << std::endl;
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::printTreeData_rec(const std::string& prefix, const AVLTree<KeyElem,Data>::TNode* node, bool isLeft){
    if( node != nullptr ){
        std::cout << prefix;

        std::cout << (isLeft ? "├──────" : "└──────" );
        //      << " player group : " <<(node->player_data->playing_group)
        //         print the value of the node  
        std::cout << "group id: " << node->key << " (" << node->data  << ") " << std::endl;//" height is "<< node->height << std::endl;

        //         enter the next tree level - left and right branch
        printTreeData_rec( prefix + (isLeft ? "│       " : "        "), node->right_son, true);
        printTreeData_rec( prefix + (isLeft ? "│       " : "        "), node->left_son, false);
        }
    }
    
    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::printFather(const KeyElem& key){
        TNode* node = AVLFind(key);
        if (node->father){
            std::cout << "\nfather of " << key << " is " << node->father->key << "\n" << std::endl;
        }
        else{
            std::cout << "\nfather is nullptr\n" << std::endl;
        }
    }


    

}

#endif