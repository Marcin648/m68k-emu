#include "bubblesort.h"

void bubblesort(int table[], int size){
	for(int i = 0; i<size-1; i++){
		for(int j = 0; j<size-1-i; j++){
			if (table[j] > table[j+1]){
				int temp = table[j+1];
				table[j+1] = table[j];
				table[j] = temp;
			}
		}
    }
}

void bubblesort_reverse(int table[], int size){
	for(int i = 0; i<size-1; i++){
		for(int j = 0; j<size-1-i; j++){
			if (table[j] < table[j+1]){
				int temp = table[j+1];
				table[j+1] = table[j];
				table[j] = temp;
			}
		}
    }
}