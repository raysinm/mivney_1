
#include <iostream>
using namespace std;
#include "../AVLtree.h"

int main() {
	AVL::AVLTree<int,int> tree1;
	tree1.AVLInsert(7,0);
	tree1.AVLInsert(3,0);
	tree1.AVLInsert(5,0);
	tree1.AVLPrintInOrder();
	cout << endl;
	tree1.AVLInsert(10,0);
	tree1.AVLPrintInOrder();
	cout << endl;
	tree1.AVLInsert(4,0);
	tree1.AVLPrintInOrder();
	cout << endl;
	tree1.AVLInsert(1,0);
	tree1.AVLPrintInOrder();
	cout << endl;
	tree1.AVLInsert(23,0);
	tree1.AVLPrintInOrder();
	cout << endl;

	// RL and RR checks mostly
	AVL::AVLTree<double,double> tree3;
	tree3.AVLInsert(2,0);
	tree3.AVLInsert(3,0);
	tree3.AVLInsert(1,0);
	tree3.AVLInsert(8,0);
	tree3.AVLInsert(7,0);
	tree3.AVLInsert(6,0);
	tree3.AVLInsert(5,0);
	tree3.AVLInsert(12,0);
	tree3.AVLInsert(10,0);
	tree3.AVLInsert(9,0);
	tree3.AVLInsert(9.5,0);
	tree3.AVLInsert(11,0);
	tree3.AVLInsert(13,0);
	tree3.AVLPrintInOrder();
	cout << endl;

	// LR and LL checks mostly
	AVL::AVLTree<double,double> tree4;
	tree4.AVLInsert(8,0);
	tree4.AVLInsert(5,0);
	tree4.AVLInsert(3,0);
	tree4.AVLInsert(4,0);
	tree4.AVLInsert(4.5,0);
	tree4.AVLInsert(7,0);
	tree4.AVLInsert(6,0);
	tree4.AVLInsert(2,0);
	tree4.AVLInsert(2.5,0);
	tree4.AVLInsert(3.5,0);
	tree4.AVLPrintInOrder();
	cout << endl;

	AVL::AVLTree<double,double> tree2;
	tree2.AVLInsert(4,1);
	tree2.AVLInsert(2,0);
	tree2.AVLInsert(3,0);
	tree2.AVLInsert(1,0);
	tree2.AVLInsert(8,0);
	tree2.AVLInsert(7,0);
	tree2.AVLInsert(6,0);
	tree2.AVLInsert(5,0);
	tree2.AVLInsert(12,0);
	tree2.AVLInsert(10,0);
	tree2.AVLInsert(9,0);
	tree2.AVLInsert(9.5,0);
	tree2.AVLInsert(11,0);
	tree2.AVLInsert(13,0);
	tree2.AVLPrintInOrder();
	cout << endl;

	// AVLRemove node with no children - no rotation
	tree2.AVLRemove(8);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with one child - no rotation
	tree2.AVLRemove(9);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with one child - no rotation
	tree2.AVLRemove(2);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with two children - no rotation
	tree2.AVLRemove(3);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with no children - with 1 rotation
	tree2.AVLRemove(9.5);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with no children - with 1 rotation
	tree2.AVLRemove(13);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with no children - no rotation
	tree2.AVLRemove(10);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with two children - no rotation
	tree2.AVLRemove(4);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with two children - no rotation
	tree2.AVLRemove(5);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with no children - no rotation
	tree2.AVLRemove(12);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove node with one child - no rotation
	tree2.AVLRemove(6);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove root with two children - no rotation
	tree2.AVLRemove(7);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove root with one child - no rotation
	tree2.AVLRemove(11);
	tree2.AVLPrintInOrder();
	cout << endl;
	// AVLRemove root with no children - no rotation
	tree2.AVLRemove(1);
	tree2.AVLInsert(12,0);
	tree2.AVLPrintInOrder();
	cout << endl;
	tree2.AVLRemove(12);

	// after removal - 2 rotations
	AVL::AVLTree<int,int> tree5;
	tree5.AVLInsert(9,0);
	tree5.AVLInsert(3,0);
	tree5.AVLInsert(15,0);
	tree5.AVLInsert(1,0);
	tree5.AVLInsert(7,0);
	tree5.AVLInsert(13,0);
	tree5.AVLInsert(19,0);
	tree5.AVLInsert(5,0);
	tree5.AVLInsert(11,0);
	tree5.AVLInsert(17,0);
	tree5.AVLInsert(21,0);
	tree5.AVLInsert(23,0);
	tree5.AVLPrintInOrder();
	cout << endl;

	tree5.AVLRemove(1);
	tree5.AVLPrintInOrder();
	cout << endl;

	return 0;
}

