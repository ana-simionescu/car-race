// Copyright 2019 Simionescu Ana-Maria
#ifndef SKIPLIST_H_
#define SKIPLIST_H_
#define MinusInf -2147483647
#define PlusInf 2147483647
#define Maxim 10

template <typename T>
struct Node {
    T key;
    T value;
    Node<T> *next;
    Node<T> *prev;
    Node<T> *above;
    Node<T> *below;

    Node(T key, T value) {
        this->key = key;
        this->value = value;
        next = nullptr;
        prev = nullptr;
        above = nullptr;
        below = nullptr;
    }
};

template <typename T>
class SkipList {
 private:
    Node<T> *head;
    Node<T> *tail;
    int numElements;
    int Levels;

    // Functii care determina nivelul pana la care
    // va urca un element in SkipList
	int coin() {
		int flip;
        unsigned int seed = clock();
		flip = rand_r(&seed) % 2;
		return flip;
	}
    int randlev() {
        int lev = 1;
        while (coin() == 0  && lev < Maxim) {
            lev++;
        }
        return lev;
	}

 public:
    SkipList() {
    	head = new Node<T>(MinusInf, 0);
    	tail = new Node<T>(PlusInf, 0);
    	head->next = tail;
    	tail->prev = head;
        numElements = 0;
        Levels = 1;
    }

    ~SkipList() {
        Node<T> *aux, *tmp, *del;
        aux = head;
        while (aux) {
        	tmp = aux;
        	aux = aux->below;
        	while(tmp) {
        		del = tmp->next;
        		delete tmp;
        		tmp = del;
        	}
    	}
    }

  	Node<T>* search(T searchkey) {
  		Node<T> *current = head;
  		while(true) {
  			while ( current->next->key < searchkey ) {
            	current = current->next;
        	}
        	if(current->below == nullptr) {
        		break;
        	} else {
        		current = current->below;
        	}
  		}
  		return current;
  	}

    void insert(T addkey, T addvalue) {
    	int newlev = randlev();
    	int a = newlev;

        // Creez nivelele care nu exista inca in SkipList
        while (a > Levels) {
            Node<T> *nodemin = new Node<T>(MinusInf, 0);
            Node<T> *nodemax = new Node<T>(PlusInf, 0);
            nodemin->next = nodemax;
    		nodemin->prev = nullptr;
    		nodemin->above = nullptr;
    		nodemin->below = head;

            nodemax->next = nullptr;
    		nodemax->prev = nodemin;
    		nodemax->above = nullptr;
    		nodemax->below = tail;

    		head->above = nodemin;
    		tail->above = nodemax;

            head = nodemin;
    		tail = nodemax;
            Levels++;
    	}
  		Node<T> *current = search(addkey);
    	Node<T> *tmp = nullptr;
        // Creez nodurile pe nivele
    	while (newlev != 0) {
    		Node<T> *aux = new Node<T>(addkey, addvalue);
    		aux->key = addkey;
    		aux->value = addvalue;
    		aux->next = current->next;
    		aux->prev = current;
    		aux->below = tmp;
    		aux->above = nullptr;
    		aux->next->prev = aux;
    		current->next = aux;
    		if (tmp != nullptr) {
    			tmp->above = aux;
    		}
    		tmp = aux;
    		newlev--;
    		if (newlev > 0) {
    			if (current->above != nullptr) {
    				current = current->above;
    			} else {
    				while (current->above == nullptr) {
    					current = current->prev;
    				}
    				current = current->above;
    			}
    		}
    	}
    	numElements++;
    }

    Node<T> *getHead() {
        return head;
    }

    Node<T> *getTail() {
        return tail;
    }
};
#endif  //  SKIPLIST_H_
