// Eduardo Didier Aguilar Alvarez - A00841850
// Damaris Paola Castrellón Carrillo - A01234497

#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <fstream>
#include <string.h>
#include <iomanip>
using namespace std;

//---------------- Order struct ----------------//
// Used to store each order's information
struct Order{
	int month;
	int day;
	int hour;
	int minute;
	int second;
    int restaurantID;
	string restaurant;
	string dish;
	int price;
    long long sortKey; 
};

//---------------- Node class ----------------//
//Use for the doubly linked list 
template <class T>
class Node{
	public:
		//int value;		// stored value
		T value; 			

		Node<T>* prev; 		// address of previous node
		Node<T>* next;		// address of next node
		
		Node(T value){
			this->value = value;
			this->prev = NULL;
			this->next = NULL;
		}
};

//---------------- Doubly Linked List ----------------//
// Doubly-linked list class: 
// pointer to the first node and total size
template <class T>
class List{
	int size;	

	public:
	Node<T>* first;	// pointer to the first node
	Node<T>* last; 	// pointer to the last node	
		List(){
			this->first = NULL; 
			this->last = NULL; 
			this->size = 0;
		}
		
		int getSize(){return size;}
		void showList();
		void showListReverse();
		
		void insertFirst(T);		
		void insertLast(T);
		bool insertAtIndex(int, T); 	

		void deleteFirst();
		void deleteLast();				
		void deleteAtIndex(int);		

		Node<T>* find(T, int*);			
		void update(int, T);			
};


//******** Done Together in Class: ************//
template <class T>
//There is no intput nor return value, but each element at list is printed
//The complexity is O(n) because it goes through all the elements of the list once 
void List<T>::showList(){
	cout << "Size: " << size << endl;

	Node<T>* aux = first;
	int i = 0;
	while (i < size){
		cout << aux->value << " ";
		aux = aux->next;

		i++;
	}
	cout << endl;
}

template <class T>
void List<T>::showListReverse(){
	cout << "Size: " << size << endl;

	Node<T>* aux = last;
	int i = size - 1;
	while (i >= 0){
		cout << aux->value << " ";
		aux = aux->prev;

		i--;
	}
	cout << endl;
}

//The input is a value wich will be used to create a new node and inser it at the beginning of the list
//There is no return value
//The complexity is O(1) because it has simple commands without loops or recursions
template <class T>
void List<T>::insertFirst(T newValue){
	Node<T>* node = new Node<T>(newValue);
	Node<T>* aux = first;
	
	node->next = aux;
	first = node;
	
	if (size == 0){
		last = node;
	}
	else {
		aux->prev = first;
	}

	first->prev = last;
	last->next = first;

	size++;
}	

//The input is a value wich will be used to create a new node and inser it at the end of the list
//There is no return value
//The complexity is O(1) because it has simple commands without loops or recursions
template <class T>
void List<T>::insertLast(T newValue){
	Node<T>* node = new Node<T>(newValue);
	Node<T>* secondLast = last;

	node->prev = secondLast;
	last = node;

	if (size == 0){
		first = node;
	}
	else {
		secondLast->next = last;
	}

	last->next = first;
	first->prev = last;
	size++;
}

// Insert at a specific index
//The Inputs are an integer representing the index where you want to insert the value and a T which is the value you want to put in there
//The Output is a boolean value, true if it is inserted or false if it couldn't be inserted
//The time complexity is O(n), because in the worst case escenario you have to go through all the array
template<class T>
bool List<T>::insertAtIndex(int index, T newValue){
	Node<T>* node = new Node<T>(newValue);
	Node<T>* aux = first;
	int i = 0;

	while (i < size) {
		if (i == index - 1) {
			node->prev = aux;
			node->next = aux->next;
			aux->next->prev = node;
			aux->next = node;

			size++;
			return true;
		}
		aux = aux->next;
		i++;
	}
	return false;
}

//The time complexityof the algorithm is O(1), because it is independent of the
//size of the array
template<class T>
void List<T>::deleteFirst(){
		if (size != 0) {
			Node<T>* aux = first;
			first = aux->next;
			first->prev = last;
			delete(aux);
			size--;
		}
	}

//The time complexityof the algorithm is O(1), because it is independent of the
//size of the array
template<class T>
void List<T>::deleteLast(){
	if (size != 0) {
		Node<T>* aux = last;
		last = aux->prev;
		last->next = first;
		delete(aux);
		size--;
	}
}

//The input is an index where the node will be deleted
//There is no return value but the list is modified
//The complexity is O(n) because in the worst case it goes through all the elements once 
template<class T>
void List<T>::deleteAtIndex(int index){
	if(index == 0){
		deleteFirst();
		return;
	}
	else if(index == size - 1){
		deleteLast();
		return;
	}
	else if (size == 0){
		return;
	}

	Node<T>* aux = first;
	int i = 0;

	while (i < size) {
		if (i == index) {
			aux->prev->next = aux->next;
			aux->next->prev = aux->prev;
			delete(aux);
			size--;
			return;
		}
		aux = aux->next;
		i++;
	}
}		

//The inputs are the index of the new value that node will have 
// There is no return value but the list is modified
//The complexity is O(n) because in the worst case it goes through all the elements once 
template<class T>
void List<T>::update(int index, T newValue){
	Node<T>* aux = first; 
	int i = 0;
	while (i < size) {
		if (i == index) {
			aux->value = newValue;
			return;	
		}
		aux = aux->next;
		i++;
	}
}

// Input is a value to be found and a pointer to an integer
// Return the address of the node containing the value and the index variable is modified by pointer
// The complexity is O(n) because in the worst case it goes through all the elements once
template<class T>
Node<T>* List<T>::find(T value, int* index){
	if(size == 0){
		*index = -1;
		return NULL;
	}
	Node<T>* aux = first;
	int i = 0;
	while (i < size) {
		if (aux->value == value) {
			*index = i;
			return aux;	
		}
		aux = aux->next;
		i++;
	}
	*index = -1;
	return NULL;
}
template<class T>
//---------------- Stack  ----------------//
//Used for quicksort 
class Linked_Stack{
	private:

    	struct Node{
       		T value;
        	Node* next;

        	Node(const T& theElement, Node* n = NULL)
          		: value(theElement), next(n) { }
    	};
    	
		int size  = 0;
		Node* top;

	public:

		// Constructor
		Linked_Stack(){	
			top = NULL;
		}

		//	Class Destructor
		~Linked_Stack(){
    		while(!isEmpty())
        		pop();
		}
		
		bool push (T value);
		T pop();
		void show();

		int getSize();
		T getTop();
		bool isEmpty();
};

//The input is an order  that will be added to the stack
//The output is true if the operation is successful
//The complexity is O(1) because it does few constant operations
template <class T>
bool Linked_Stack<T>::push (T value){
	if(top == NULL){
		top = new Node(value);
	} else {
		Node* aux = new Node(value);
		aux->next = top;
		top = aux;
	}
	size++;
	return true;
}

//The output is the order at the top of the stack or an empty order if the stack is empty
//The complexity is O(1) because it does few constant operations
template<class T>
T Linked_Stack<T>::pop(){
	if(isEmpty()){
		return NULL;
	} else {
		Node* aux = top;
		T val = aux->value;
		top = top->next;
		delete aux;
		size--;
        return val;
	}
}

//There is no input
//The output is true if the stack is empty and false otherwise
//The complexity is O(1) because it does few constant operations
template<class T>
bool Linked_Stack<T>::isEmpty(){
	return top == NULL;
}

//The output is the size of the stack
//The complexity is O(1) because it does few constant operations
template<class T>
int Linked_Stack<T>::getSize(){
	return size;
}

//There is no input
//The output is the order at the top of the stack or an empty order if the stack is empty
//The complexity is O(1) because it does few constant operations
template<class T>
T Linked_Stack<T>::getTop(){	
	if(isEmpty()){
		return T();
	} else {
		return top->value;
	}
}

//---------------- Quicksort  ----------------//
//The inputs are a linked list of orders and the pointers for the left and right limits of the list
//The output is a pointer to the pivot node after partitioning
//The complexity is O(n) because it goes through all the elements of the list once
Node<Order>* partition(List<Order>& orders, Node<Order>* left, Node<Order>* right){
	//Pointers i and j to traverse the list and a pivot pointer
	Node<Order>* i = NULL, *j = left, *pivot = right;
	//Use for the exchange of values
	Order aux;
	
	//Transverse all the list from left to right
	while(j != right){
		//if a value less than or equal to the pivot is found, the value is swapped with the value at i
		if( j->value.restaurantID <= pivot->value.restaurantID ){
			//start the pointer if not initialized
			if( i == NULL ){
				i = left;
			}
			//use the next position
			else{
				i = i->next;
			}
			aux = i->value ;
			i->value = j->value;
			j->value = aux;
		} 
		j = j->next;
	}
	//if the pointer is not used, initialize it
	if( i == NULL){
		i = left;
	}else{ // else, use the next position
		i = i->next;
	}
	//swap the pivot with the value at i, so all values to the left of i are less than or equal to the pivot
	aux = i->value ;
	i->value = pivot->value;
	pivot->value = aux;
	return i; 
}

//The inputs are a linked list of orders and the pointers for the left and right limits of the list
//There is not output but the list is sorted
//The complexity is O(n log n) on average because it uses partitioning to sort the list
void quicksort(List<Order>& orders, Node<Order>* left, Node<Order>* right ){
    //stack used to store the sublists to be sorted
	Linked_Stack<Node<Order>*> stack; 
	//the first pair are the limits of the list
    stack.push(left);
    stack.push(right);
	//pointer used as pivot
	Node<Order>* pivot;

    while( !stack.isEmpty() ){
		//Obtain the limits of the current sublist
        right = stack.pop();
        left = stack.pop();
		pivot = partition(orders, left, right);

		//verifies if the pivot is not the leftmost element, checking if the left pointer is different from pivot
		// and if the element before pivot is different from right, because the list is circular
		if( pivot != left && pivot->prev != right ){
			stack.push(left);
			stack.push(pivot->prev);
		}
		//verifies if the pivot is not the rightmost element, checking if the right pointer is different from pivot
		// and if the element after pivot is different from left, because the list is circular
		if( pivot != right && pivot->next != left){
			stack.push(pivot->next);
			stack.push(right);
		}
    }
};

//---------------- Main  ----------------//
int main(int argc, char* argv[]){
	string sline;

	ifstream inFile("orders.txt"); 	
	ofstream outFile("output.txt");	
	
	List<Order> orders;

    //Map with restaurants id and names
    map<string,int> restaurants_map;
    //counter for restaurant IDs
    int restaurant_counter = 0;

	//Convert the month string into numbers
    map<string, int> month_to_int = {
        {"ene", 1}, {"Feb", 2}, {"Mar", 3}, {"Abr", 4}, 
        {"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Ago", 8}, 
        {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dic", 12}
    };	

	//	Verify that the files have opened correctly
	if (inFile.is_open() && outFile.is_open() ){	

		while ( getline(inFile, sline) ){			
			//Declare variables to hold data from current line
			string month_str, time_str, restaurant, dish;
            int day, hour, minute, second, price;			

			istringstream iss(sline);
			iss >> month_str >> day >> time_str;

			// Parse time_str (format HH:MM:SS)
			size_t pos1 = time_str.find(":");
			size_t pos2 = time_str.find(":", pos1 + 1);
			hour = stoi(time_str.substr(0, pos1));
			minute = stoi(time_str.substr(pos1 + 1, pos2 - pos1 - 1));
			second = stoi(time_str.substr(pos2 + 1));

			// Find restaurant (after R: and before O:)
			size_t r_pos = sline.find("R:");
			size_t o_pos = sline.find("O:");
			restaurant = sline.substr(r_pos + 2, o_pos - r_pos - 3); // -3 to remove space before O:

			// Find dish and price (dish is after O:, price is in parentheses at end)
			size_t paren_pos = sline.find_last_of('(');
			size_t paren_end = sline.find_last_of(')');
			dish = sline.substr(o_pos + 2, paren_pos - o_pos - 2);
			price = stoi(sline.substr(paren_pos + 1, paren_end - paren_pos - 1));

			Order o;
            o.month = month_to_int[month_str];
			o.day = day;
            o.hour = hour;
            o.minute = minute;
            o.second = second;
            // if restaurant is not in map, add it
            if (restaurants_map.find(restaurant) == restaurants_map.end()) {
                restaurants_map[restaurant] = restaurant_counter;
                restaurant_counter++;
            }
            o.restaurantID = restaurants_map[restaurant];
            o.restaurant = restaurant;
            o.dish = dish;
            o.price = price;

			//Create a single string with the date and time ofthe form MMDDHHMMSS
	        stringstream key;
    	    key << setfill('0') << setw(2) << month_to_int[month_str]
        	    << setw(2) << day
            	<< setw(2) << hour
	            << setw(2) << minute
    	        << setw(2) << second;

			//Convert into a long long integer and store in sortKey
        	o.sortKey = stoll(key.str());			

			orders.insertLast(o);
		}
		
	} 

	inFile.close();

	//sort by name
	cout << "Sorting orders by restaurant" << endl;
	quicksort(orders, orders.first, orders.last );
	
	Node<Order>* current = orders.first;
	for (int i = 0; i < orders.getSize(); i++) {
    Order order = current->value;
		outFile << "Month: " << order.month 
			<< ", Day: " << order.day 
			<< ", Hour: " << order.hour 
			<< ", Minute: " << order.minute 
			<< ", Second: " << order.second  
			<< ", Restaurant: " << order.restaurant 
			<< ", Dish: " << order.dish 
			<< ", Price: " << order.price  << endl;
		current = current->next;
	}
	outFile.close(); 

	return 0;
}