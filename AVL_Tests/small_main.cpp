#include "../course_files/library1.h"
#include "../AVLtree.h"

int main(){

    AVL::AVLTree<int,int> test_t;
    test_t.AVLInsert(5,5);
    test_t.AVLInsert(6,5);
    test_t.AVLInsert(7,5);
    test_t.AVLInsert(4,5);
    test_t.AVLInsert(3,5);
    test_t.AVLInsert(2,5);

    test_t.AVLRemove(4);
    test_t.AVLInsert(1,5);
    test_t.AVLRemove(7);
    test_t.AVLInsert(0,5);
    test_t.AVLRemove(6);

    return 0;
}