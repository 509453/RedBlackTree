
#include <iostream>
#include <cstring>
#include <string>
#include <iterator>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

/*
Name: Aleena Shaik
Date: 4/9/25
Project: Red Black Tree
Sources used: Programiz Red Black Tree Logic
Diya and Mahi helped me :(
 */

struct Node{
  int data;
  Node* parent; 
  char color;
  Node* right;
  Node* left;
};

void rotateRight(Node*& root, Node*& x){
    Node* y = x->left; 
    x->left = y->right; 
    if (y->right != NULL){
        y->right->parent = x; 
    }
    y->parent = x->parent; 
    if(x->parent == NULL){
        root = y;
    }else if(x== x->parent->right){
        x->parent->right = y;
    }else{
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y; 
}


void rotateLeft(Node*& root, Node*& x){
    Node* y = x->right; 
    x->right = y->left; 
    if (y->left !=NULL){
        y->left->parent = x;
    }
    y->parent = x->parent; 
    if (x->parent == NULL){
        root = y;
    }else if (x== x->parent->left){
        x->parent->left = y;
    }else{
        x->parent->right = y; 
    }
    y->left = x; 
    x->parent = y; 
}

void fixViolations(Node*& root, Node*& newNode) {
    /*
    Do the following while the parent of newNode p is RED.
If p is the left child of grandParent gP of z, do the following.
    */
    while (newNode != root && newNode->parent->color == 'R') {
        Node* parent = newNode->parent;
        Node* grandParent = parent->parent;
        Node* uncle;
        if (parent == grandParent->left) {
            uncle = grandParent->right;
        } else {
        uncle = grandParent->left;
       }
       
       /*
       Case-I:
If the color of the right child of gP of z is RED, set the color of
both the children of gP as BLACK and the color of gP as RED.
Assign gP to newNode.
       **/

        if (uncle != NULL && uncle->color == 'R') {
            parent->color = 'B';
            uncle->color = 'B';
            grandParent->color = 'R';
            newNode = grandParent; 
        } else {
            if (parent == grandParent->left) {
                if (newNode == parent->right) {
                    rotateLeft(root, parent);
                    newNode = parent;
                    parent = newNode->parent;
                }
                  /*
                Case-III:
Set color of p as BLACK and color of gP as RED.
Right-Rotate gP.
Else, do the following.
If the color of the left child of gP of z is RED, set the color of both the 
children of gP as BLACK and the color of gP as RED.
Assign gP to newNode.
*/
                parent->color = 'B';
                grandParent->color = 'R';
                newNode = grandParent; 
                rotateRight(root, grandParent);
            } else {
                /*Else if newNode is the left child of p then, assign p to newNode and Right-Rotate newNode.
Set color of p as BLACK and color of gP as RED.
Left-Rotate gP.
Set the root of the tree as BLACK.
                */
              
                if (newNode == parent->left) {
                    rotateRight(root, parent);
                    newNode = parent;
                    parent = newNode->parent;
                }
                parent->color = 'B';
                grandParent->color = 'R';
                rotateLeft(root, grandParent);
            }
        }
    }
    root->color = 'B';
}



Node* addNode(Node*& root, int actualNumber) {
    Node* newNode = new Node();
    newNode->data = actualNumber;
    newNode->color = 'R'; //ALWAYS START W R
    //INITIALIZE
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    
    ///ROOT ALWAYS NEEDS TO BE BLACK
    //THAT IS WHY WE INSERT NODES AS RED SO IT COULD CHECK FOR EVERYTHING
    if (root == NULL) {
        root = newNode;
        root->color = 'B'; 
        return root;
    }
  
  
  //ADDING AND CHECKING FOR VIOLATIONS
    Node* current = root;
    Node* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (actualNumber < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    newNode->parent = parent;
    if (actualNumber < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
}

    fixViolations(root, newNode);

    return root;
}


void printTree(Node*& root, int depth) {
    if (root != NULL) {
        printTree(root->right, depth + 1); 

        for (int i = 0; i < depth; i++) {
            cout << '\t';  
        }
        cout << root->data << "(" << root->color << ")" << endl;
        printTree(root->left, depth + 1);  
    }
}


void readFile(Node*& root){
      cout << "Enter a file name to use numbers from (numbers.txt): "<<endl;
      char filename[256];
      cin.getline(filename, 256, '\n');

      if (strcmp(filename, "numbers.txt") == 0){
        ifstream f;
        f.open(filename);
        cout << "How many number would you like to add?" <<endl;
        int howManyNum;
        cin >> howManyNum;
        int passthruNum;
        int count = 0;


        while (f >> passthruNum && count < howManyNum){
          root = addNode(root, passthruNum);
          count++;
        }

      }

}

void searchNumm(Node*& root, int searchNumber, bool& present) {
    if (root == nullptr) {
        return;
    }

    if (root->data == searchNumber) {
        cout <<"This number is present in the tree" <<endl;
        present = true;
    }else{
        searchNumm(root->left, searchNumber, present);
        searchNumm(root->right, searchNumber, present);
    }
}


Node* findMinimum(Node* root){
    while (root->left !=NULL){
        root = root->left; 
    }
    return root; 
}

void fixDeletion(Node*& root, Node* x, Node* xParent) {
    while (x != root && (x == NULL || x->color == 'B')) {
        if (x == xParent->left) {
            Node* sibling = xParent->right;
            
            //IF THE COLOR IS RED WE NEED TO CHANGE COLOR AND LEFT ROTATE

            if (sibling && sibling->color == 'R') {
                 sibling->color = 'B';
                xParent->color = 'R';
                rotateLeft(root, xParent);
                sibling = xParent->right;
          }
          // when sibling is black and both of its children are black case 1 
          if ((!sibling || (sibling->left && sibling->left->color == 'B') && 
                 (sibling->right && sibling->right->color == 'B'))) {
                if (sibling!=NULL){
                    sibling->color = 'R';
                } 
                x = xParent;
                xParent = xParent->parent;//move up to the tree 
            } else {
                //if sibling's right child is black, rotate sibling right case 2
               if (!sibling->right || sibling->right->color == 'B') {
                   if (sibling->left) sibling->left->color = 'B';
                    sibling->color = 'R';
                    rotateRight(root, sibling);
                    sibling = xParent->right;
                }

                //adjust colors and rotate left
                sibling->color = xParent->color;
                xParent->color = 'B';
                if (sibling->right!= NULL){
                   sibling->right->color = 'B'; 
                } 
                rotateLeft(root, xParent);
                break;
            }
        } else {
            Node* sibling = xParent->left;

            if (sibling && sibling->color == 'R') {
                sibling->color = 'B';
                xParent->color = 'R';
                rotateRight(root, xParent);
                sibling = xParent->left;
            }

            //  when sibling is black  and both of its children are black case 3
            if ((!sibling || (sibling->left && sibling->left->color == 'B') && 
                 (sibling->right && sibling->right->color == 'B'))) {
                if (sibling!=NULL){
                    sibling->color = 'R';
                } 
                x = xParent;
                xParent = xParent->parent;
            } else {
                //if sibling's left child is black, rotate sibling left
                if (!sibling->left || sibling->left->color == 'B') {
                    if (sibling->right) sibling->right->color = 'B';
                    sibling->color = 'R';
                    rotateLeft(root, sibling);
                    sibling = xParent->left;
     }
     //adjust colors and rotate right 
           sibling->color = xParent->color;
         xParent->color = 'B';
          if (sibling->left!=NULL){
              sibling->left->color = 'B';
          }
          rotateRight(root, xParent);
         break;
             }
        }
    }

    if (x != NULL) {
        x->color = 'B';
    }
}

void removee(Node*& root, int removeNumber) {
    Node* nodeToBeDeleted = root;

    //finding the node to delete by traversing through the tree
    //from binarysearchtree
    while (nodeToBeDeleted != NULL && nodeToBeDeleted->data != removeNumber) {
        if (removeNumber < nodeToBeDeleted->data)
            nodeToBeDeleted = nodeToBeDeleted->left;
        else
            nodeToBeDeleted = nodeToBeDeleted->right;
    }

    // if null then nothing to do
    if (nodeToBeDeleted == NULL) {
        return;
}

//store the variables make new nodes and save original color
    Node* y = nodeToBeDeleted;
    Node* x;
    Node* xParent;
    char originalColor = y->color;

    //if node has two children;replace with successor
    if (nodeToBeDeleted->left != NULL && nodeToBeDeleted->right != NULL) {
        y = findMinimum(nodeToBeDeleted->right);
        originalColor = y->color;
        nodeToBeDeleted->data = y->data;
        nodeToBeDeleted = y;
    }
    
    //got it from diya which is from programiz
    x = (nodeToBeDeleted->left != NULL) ? nodeToBeDeleted->left : nodeToBeDeleted->right;

    if (x != NULL) {
        x->parent = nodeToBeDeleted->parent;
    }

    xParent = nodeToBeDeleted->parent;

    //adjust the parent pointers
    if (nodeToBeDeleted->parent == NULL) {
        root = x;
    } else if (nodeToBeDeleted == nodeToBeDeleted->parent->left) {
        nodeToBeDeleted->parent->left = x;
    } else {
        nodeToBeDeleted->parent->right = x;
    }

    //if black we need to fix violoations....
    if (originalColor == 'B') {
        fixDeletion(root, x, xParent);
    }

 
    delete nodeToBeDeleted;

    // the root is ALWAYS black
    if (root != NULL) {
        root->color = 'B';
    }
}

int main(){

  //variables
  char input [100];
  char file [] = "numbers.txt";
  char addRandom[] = "ADD FILE";
  char addInput[] = "ADD INPUT";
  char print[] = "PRINT";
  char search[] = "SEARCH";
  char remove[] = "REMOVE";
  char quit[] = "QUIT";
  bool  stillPlaying = true;


  cout<< "Type the command terms: ADD FILE, ADD INPUT, PRINT, REMOVE, SEARCH, QUIT" << endl;
  Node* root = NULL;

  while (stillPlaying == true){
    cin.getline(input, 100, '\n');
    if (strcmp(input, addRandom) == 0){
        readFile(root);

    }else if(strcmp(input, addInput) == 0){
       int numbersToAdd = 0;
      cout<< "How many numbers would you like to add?" << endl;
      cin >> numbersToAdd;
      for (int i = 0; i < numbersToAdd; i++){
        int actualNumber = 0;
        cout << "Please enter your number: " << endl;
        cin >> actualNumber;
        if (actualNumber < 1 || actualNumber > 999){
          cout << "Please type in numbers between 1 - 999." << endl;
          cin >> actualNumber;
          root = addNode(root, actualNumber);

        }else{
          root = addNode(root, actualNumber);
        }
      }


    }else if (strcmp(input, print) == 0){
      int depth = 0;
      printTree(root, depth);

    }else if (strcmp(input, search) == 0){
       int searchNumber;
        cout << "What number would you like to search in the tree: "<<endl;
        cin >> searchNumber;
        bool present = false;
        searchNumm(root, searchNumber, present);
        if (present == false){
            cout<< "This number is not present in the tree" <<endl;
        }



    }else if (strcmp(input, remove) == 0){
      cout<<"What number would you like to remove?"<<endl;
        int removeNumber = 0; 
        cin>> removeNumber; 
        removee(root, removeNumber);


    }else if (strcmp(input, quit) == 0){
             stillPlaying = false;
    }


  }


  return 0;
}
