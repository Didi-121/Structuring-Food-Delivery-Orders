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

struct Order{
	int month;
	int day;
	int hour;
	int minute;
	int second;
	string restaurant;
	string dish;
	int price;
    long long sortKey; 
};


//Do a counting sort on the digit we are sorting
void countingSort(vector<Order> &orders, int exp) {
    int n = orders.size();
    vector<Order> output(n);
    int count[10] = {0};

	//Extract digit from the exp position
    for (int i = 0; i < n; i++)
        count[(orders[i].sortKey / exp) % 10]++;

	//Prefix sum 
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

	//Put each element in the output array going from right to left 
    for (int i = n - 1; i >= 0; i--) {
        output[count[(orders[i].sortKey / exp) % 10] - 1] = orders[i];
        count[(orders[i].sortKey / exp) % 10]--;
    }

	//Copy result in orders[]
    for (int i = 0; i < n; i++)
        orders[i] = output[i];
}
//Complexity: o(n)
void radixSort(vector<Order> &orders) {
	//	To know how many digits does the largest have 
    long long maxVal = 0;
    for (auto &o : orders)
        if (o.sortKey > maxVal){
			maxVal = o.sortKey;	
		} 

	//Changes the digit we are analyzing 
    for (long long exp = 1; maxVal / exp > 0; exp *= 10)
        countingSort(orders, exp);
}

//This functions returns a key from a vector<Order>, if not founded it returns the element that is the first
//greater or lesser than the key element that is in the array, depending on the greater argument.  
//complexity : O(log n)
int  find_first_kth(const vector<Order>& orders, pair<int, int> date, bool greater = false){
	int k = 0, left = 0, right = orders.size() - 1, mid = 0;
	
	while( left <= right ){
		mid = (left + right ) / 2;

		if( orders[mid].month == date.first && orders[mid].day == date.second)
			return mid; 
		
		// first find the month 
		if( orders[mid].month == date.first){
			//If middle is in the month, find the day
			if( date.second > orders[mid].day ){
				left = mid + 1;
			}else{
				right = mid -1;
			}
		}else{
			if( date.first > orders[mid].month){
				left = mid + 1;
			}else{
				right = mid - 1;
			}
		}
	}
	// If the exact day isnt found retur the first greather or lesser
	return greater ? right + 1 : left - 1 ;
}

int main(int argc, char* argv[]){
	string sline;

	ifstream inFile("orders.txt"); 	
	ofstream outFile("output.txt");	
	
	vector<Order> orders;


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

			orders.push_back(o);
		}
		
	} 

	inFile.close();

	radixSort(orders);

	
    cout << "First 10 lines after being sorted: \n";
    for (size_t i = 0; i < orders.size() && i < 10; i++) {
        auto &o = orders[i];
        cout << o.month << " " << o.day << " "
             << setw(2) << setfill('0') << o.hour   << ":"
             << setw(2) << setfill('0') << o.minute << ":"
             << setw(2) << setfill('0') << o.second
             << "R:"     << o.restaurant << "O:" << o.dish
             << "("     << o.price   << ")"   << endl;
    }
	

	ofstream outFileStream("output.txt");
	for (int j = 0; j <= orders.size(); j++) {
		const Order& o = orders[j];
		outFileStream << o.month << " " << o.day << " ";
		outFileStream << (o.hour < 10 ? "0" : "") << o.hour << ":"
					  << (o.minute < 10 ? "0" : "") << o.minute << ":"
					  << (o.second < 10 ? "0" : "") << o.second << " ";
		outFileStream << "R:" << o.restaurant << " O:" << o.dish << "(" << o.price << ")" << endl;
	}

	outFileStream.close();

	outFile.close();

	//Search of orders between two dates
	pair<int, int> date1;
	pair<int, int> date2;

	cout << "Input the first date as two separated numbers (month day): " << endl;
	cin >> date1.first >> date1.second;
	cout << "Input the second date as two separated numbers (month day): " << endl;
	cin >> date2.first >> date2.second;

	int begin_index = find_first_kth(orders, date1, true);
	int end_index = find_first_kth(orders, date2, false);
	
	// beginnig day index if found, but there could be other orders with the same day and earlier hour 
	// complexity: O(n), n being the days with same day and month but diferent hour
	int i = begin_index;
	while( i > 0 &&  orders[i].day == orders[i - 1].day ){
		i --; 
	}
	begin_index = i;
	//same for greatest 
	i = end_index;
	while( i < orders.size() &&  orders[i].day == orders[begin_index + 1].day ){
		orders[begin_index].day ++; 
	}
	end_index = i;

	cout << "Displaying orders from " <<  orders[begin_index].month << " " << orders[begin_index].day 
	<< " to " <<  orders[end_index].month << " " << orders[end_index].day << endl;
	for(i = begin_index; i <= end_index; i++){
		cout << orders[i].month << " " << orders[i].day << " " << orders[i].hour << " " << orders[i].minute << " " << orders[i].second << " "
		<< orders[i].restaurant << " " << orders[i].dish << " " << orders[i].price << endl;
	}

	char saveOption;
	bool saveToFile = false;
	string outputFileName;

	cout << "Do you want to save the output to a file? (y/n): ";
	cin >> saveOption;

	if (saveOption == 'y' ) {
		saveToFile = true;
		cout << "Enter the name of the output file: ";
		cin >> outputFileName;
	} else{
		saveToFile = false;
	} 
	//Here each order is saved in the file with the same format as the input file
	if (saveToFile) {
		ofstream outFileStream(outputFileName + ".txt");
		for (int j = begin_index; j <= end_index; j++) {
			const Order& o = orders[j];
			outFileStream << o.month << " " << o.day << " ";
			outFileStream << (o.hour < 10 ? "0" : "") << o.hour << ":"
						  << (o.minute < 10 ? "0" : "") << o.minute << ":"
						  << (o.second < 10 ? "0" : "") << o.second << " ";
			outFileStream << "R:" << o.restaurant << " O:" << o.dish << "(" << o.price << ")" << endl;
		}
		outFileStream.close();
	}
	return 0;
}