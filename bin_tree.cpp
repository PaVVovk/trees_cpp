#include <iostream>
#include <iomanip>
//Огромная благодарность Павлу Колесникову
//за столь же огромную проделанную им работу

using std::endl;
using std::cout;
using std::cin;

enum colors{black, red};

struct Node{
    int value = 0;
    Node* left = nullptr;
    Node* right = nullptr;

    //Я РУССКИЙ
    //Я ИДУ ДО КОНЦА
    //Я РУССКИЙ
    //МОЯ КРОВЬ ОТ ОТЦААА
    Node* parent = nullptr;
    bool color = black;
};

Node* create_node(int value=0){
    Node* node = new Node;
    node->value = value;
    return node;
}

Node* grandpa(Node* node){
    if ((node != nullptr) && (node->parent != nullptr)){
        return node->parent->parent;
    }
    return nullptr;
}

Node* uncle(Node* node){
    Node* grand = grandpa(node);
    if (grand != nullptr){
        if (grand->left != node->parent){
            return grand->left;
        }
        return grand->right;
    }
    return nullptr;
}

bool is_left(Node* child){
    if (child->parent == nullptr){
        return false;
    }
    return (child->parent->left == child);
}

Node* bro(Node* node){
    if ((node == nullptr) or (node->parent == nullptr)){
        return nullptr;
    }
    if (is_left(node)){
        return node->parent->right;
    }
    return node->parent->left;
}

void rotate_left(Node* n, Node* &head){
    Node* pivot = n->right;
    pivot->parent = n->parent; /* при этом, возможно, pivot становится корнем дерева */
    if (pivot->parent != nullptr) {
        if (n->parent->left == n)
            n->parent->left = pivot;
        else
            n->parent->right = pivot;
    }
	
    n->right = pivot->left;
    if (pivot->left != nullptr)
        pivot->left->parent = n;

    n->parent = pivot;
    pivot->left = n;
    if (pivot->parent == nullptr){
        head = pivot;
    }
}

void rotate_right(Node* n, Node* &head){
    Node* pivot = n->left;
    pivot->parent = n->parent; /* при этом, возможно, pivot становится корнем дерева */
    if (n->parent != nullptr) {
        if (n->parent->left==n)
            n->parent->left = pivot;
        else
            n->parent->right = pivot;
    }		
	
    n->left = pivot->right;
    if (pivot->right != nullptr)
        pivot->right->parent = n;

    n->parent = pivot;
    pivot->right = n;
    if (pivot->parent == nullptr){
        head = pivot;
    }
}

void change_color(Node* &leaf, Node* &head){
    leaf->color = red;
    if (leaf->parent == nullptr){
        leaf->color = black;
        return;
    }
    if ((leaf->parent->color == red) && ((uncle(leaf) != nullptr) && (uncle(leaf)->color == red))){
        leaf->parent->color = black;
        uncle(leaf)->color = black;
        change_color(leaf->parent->parent, head);
        return;
    }
    if ((leaf->parent->color == red) && ((uncle(leaf) == nullptr) or (uncle(leaf)->color == black))
        && (is_left(leaf) != is_left(leaf->parent))){
        if (is_left(leaf)){
            rotate_right(leaf->parent, head);
            leaf = leaf->right;
        } else {
            rotate_left(leaf->parent, head);
            leaf = leaf->left;
        }
    }
    if ((leaf->parent->color == red) && ((uncle(leaf) == nullptr) or (uncle(leaf)->color == black))
        && (is_left(leaf) == is_left(leaf->parent))){
            leaf->parent->color = black;
            grandpa(leaf)->color = red;
            if (is_left(leaf)){
                rotate_right(leaf->parent->parent, head);
            } else {
                rotate_left(leaf->parent->parent, head);
            }
    }
}

void insert(Node* &tree, int value){
    Node* leaf = create_node(value);
    if (tree == nullptr){
        tree = leaf;
        return;
    }
    Node* tmp = tree;
    Node* parent = nullptr;
    while (tmp != nullptr){
        parent = tmp;
        if (value < tmp->value){
            tmp = tmp->left;
        } else {
            tmp = tmp->right;
        }
    }
    leaf->parent = parent;
    if (value < parent->value){
        parent->left = leaf;
    } else {
        parent->right = leaf;
    }
    change_color(leaf, tree);
}

Node* min_tree(Node* tree){
    if (tree == nullptr){
        return nullptr;
    }
    while (tree->left != nullptr){
        tree = tree->left;
    }
    return tree;
}

Node* max_tree(Node* tree){
    if (tree == nullptr){
        return nullptr;
    }
    while (tree->right != nullptr){
        tree = tree->right;
    }
    return tree;
}

int tree_height(Node* tree){ //черная высота дерева без учёта NULL-листьев
    int h = 0;
    while (tree != nullptr){
        if (tree->color == black){
            h++;
        }
        tree = tree->left;
    }
    return h;
}

void black_conditions(Node* deleted, Node*& head){
    if ((deleted == nullptr) or (head == nullptr) or (deleted->parent == nullptr)){
        return;
    }

    if (bro(deleted)->color == red){
        deleted->color = red;
        deleted->parent->color = black;
        if (is_left(deleted)){
            rotate_left(deleted->parent, head);
        } else {
            rotate_right(deleted->parent, head);
        }
    } 
    
    if (bro(deleted)->color == black) {

        if (((bro(deleted)->left == nullptr) or (bro(deleted)->left->color == black)) && 
        ((bro(deleted)->right == nullptr) or (bro(deleted)->right->color == black))){
            
            if (deleted->parent->color == black){
                bro(deleted)->color = red;
                black_conditions(deleted->parent, head);
            } else {
                bro(deleted)->color = red;
                deleted->parent->color = black;
            }
        } else if ((is_left(deleted)) && ((bro(deleted)->right == nullptr) or (bro(deleted)->right->color == black))
            && ((bro(deleted)->left != nullptr) && (bro(deleted)->left->color == red))){
                
                bro(deleted)->color = red;
                bro(deleted)->left->color = black;
                rotate_right(bro(deleted), head);
        
        } else if ((!is_left(deleted)) && ((bro(deleted)->left == nullptr) or (bro(deleted)->left->color == black))
            && ((bro(deleted)->right != nullptr) && (bro(deleted)->right->color == red))){
                
                bro(deleted)->color = red;
                bro(deleted)->right->color = black;
                rotate_left(bro(deleted), head);
        
        }

        if ((is_left(deleted)) && ((bro(deleted)->right != nullptr) && (bro(deleted)->right->color == red))){

            bro(deleted)->color = deleted->parent->color;
            deleted->parent->color = black;
            bro(deleted)->right->color = black;
            rotate_left(deleted->parent, head);

        } else if ((!is_left(deleted)) && ((bro(deleted)->left != nullptr) && (bro(deleted)->left->color == red))){
            
            bro(deleted)->color = deleted->parent->color;
            deleted->parent->color = black;
            bro(deleted)->left->color = black;
            rotate_right(deleted->parent, head);

        }
    }
}

void del(Node* deleted, Node* &head)
{   
    if ((deleted == nullptr) or (head == nullptr)){
        return;
    }
    if ((deleted->right != nullptr) && (deleted->left != nullptr))
    {
        deleted->value = max_tree(deleted->left)->value;
        del(max_tree(deleted->left), head);
        return;
    }

    if (deleted->color == red)
    {   
        deleted->parent->right = nullptr;
        delete deleted;
        return;
    }

    if (deleted->color == black){

        if (deleted->left != nullptr) //если так, то этот узел слева - обязательно красный
        { 
            if (deleted->parent != nullptr)
            {
                if (is_left(deleted))
                {
                    deleted->parent->left = deleted->left;
                }
                else
                {
                    deleted->parent->right = deleted->left;
                }
            }
            
            deleted->left->parent = deleted->parent;
            deleted->left->color = black;
            
            if (deleted->left->parent == nullptr){
                head = deleted->left;
            }
            delete deleted;
            return;
        } if (deleted->right != nullptr) //если так, то узел справа - обязательно красный
        { 
            if (deleted->parent != nullptr)
            {
                if (is_left(deleted))
                {
                    deleted->parent->left = deleted->right;
                }
                else
                {
                    deleted->parent->right = deleted->right;
                }
            }
            
            deleted->right->parent = deleted->parent;
            deleted->right->color = black;
            
            if (deleted->right->parent == nullptr){
                head = deleted->right;
            }
            delete deleted;
            return;
        } else {
            black_conditions(deleted, head);
            if (deleted->parent == nullptr){
                head = nullptr;
            } else if (is_left(deleted)){
                deleted->parent->left = nullptr;
            } else {
                deleted->parent->right = nullptr;
            }
            delete deleted;
            return;
        }
    }
}

void delete_tree(Node* &tree){
    Node* tmp = tree;
    while (tree != nullptr){
        del(tmp, tree);
        tmp = tree;
    }
}

Node* search(int value, Node* tree){
    Node* tmp = tree;
    while ((tmp != nullptr) and (tmp->value != value)){
        if (tmp->value > value){
            tmp = tmp->left;
        } else {
            tmp = tmp->right;
        }
    }
    return tmp;
}

void print_tree(Node* tree, int padding = 0) {//Функция вывода данных узла
    if (tree != nullptr) {
        cout << std::setw(padding) << ' ' << std::setw(0) << " ["<< tree->value << ", ";
        if (tree->color){
            cout <<  "Red]" << endl;
        } else {
            cout << "Black]" << endl;
        }
        padding += 2;
        print_tree(tree->left, padding); //левое поддерево
        print_tree(tree->right, padding); //правое поддерево
    }
}

void print_node(Node* node){
    if (node == nullptr){
        cout << "This node is nullptr" << endl;
        return;
    }
    cout << "Value: " << node->value << endl;
    cout << "Color: ";
    if (node->color){
        cout << "Red" << endl;
    } else {
        cout << "Black" << endl;
    }
    if (node->parent == nullptr){
        cout << "This node is head" << endl;
    } else if (node->parent->color){
        cout << "This node has red parent" << endl;
    } else {
        cout << "This node has black parent" << endl;
    }
    if (node->left == nullptr){
        cout << "This node has NULL (black) left child" << endl;
    } else if (node->left->color){
        cout << "This node has not-NULL red left child" << endl;
    } else {
        cout << "This node has not-NULL black left child" << endl;
    }
    if (node->right == nullptr){
        cout << "This node has NULL (black) right child" << endl;
    } else if (node->right->color){
        cout << "This node has not-NULL red right child" << endl;
    } else {
        cout << "This node has not-NULL black right child" << endl;
    }
}

int main(){
    Node* tree = nullptr;
    //тестовый пример
    for (int i = 0; i < 12; i++){
        insert(tree, i);
    }
    print_tree(tree);
    print_node(search(6, tree));
    cout << tree_height(tree) << endl;
    delete_tree(tree);
    return 0;
}