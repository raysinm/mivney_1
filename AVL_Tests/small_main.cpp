#include "../course_files/library1.h"
#include "../AVLtree.h"

int main(){

/*     AVL::AVLTree<int,int> test_1;
    test_1.AVLInsert(5,5);
    test_1.AVLInsert(6,5);
    test_1.AVLInsert(7,5);
    test_1.AVLInsert(4,5);
    test_1.AVLInsert(3,5);
    test_1.AVLInsert(2,5);

    test_1.AVLRemove(4);
    test_1.AVLInsert(1,5);
    test_1.AVLRemove(7);
    test_1.AVLInsert(0,5);
    test_1.AVLRemove(6);

    AVL::AVLTree<int,int> test_2;
    test_2.AVLInsert(6,0);
    test_2.AVLInsert(3,0);
    test_2.AVLInsert(4,0);
    test_2.AVLInsert(1,0);
    test_2.AVLPrintInOrder();
    std::cout << std::endl;

    AVL::AVLTree<int,int> test_3;
    test_3.AVLInsert(8,0);
    test_3.AVLInsert(5,0);
    test_3.AVLInsert(2,0);
    test_3.AVLInsert(7,0);
    test_3.AVLPrintInOrder();
    std::cout << std::endl;

    test_2.AVLMerge(test_3);
    test_2.AVLRemove(5);

    test_2.AVLPrintInOrder();
    std::cout << std::endl; */

    
    AVL::AVLTree<int,int> test4;
    test4.AVLInsert(3,0);
    test4.AVLInsert(2,0);
    test4.AVLInsert(6,0);
    test4.AVLInsert(1,0);
    test4.AVLInsert(5,0);
    test4.AVLInsert(8,0);
    test4.AVLInsert(7,0);
    test4.AVLInsert(9,0);
    test4.AVLInsert(4,0);
    
    

    /* for(int i = 1; i<=7 ; i++){
        test4.AVLInsert(i,0);
        std::cout << "\ninsert " << i << "\n" <<std::endl;
        test4.printTree();
        if(i>=3) test4.printFather(3);
    } */  

    test4.printTree();
    test4.AVLRemove(2);
    test4.printTree();
    //test4.printFather(3);

    
    
    


    return 0;
}