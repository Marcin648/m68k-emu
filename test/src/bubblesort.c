int data[] = {
    6312,
    1862,
    3020,
    1965,
    2195,
    3112,
    2051,
    7286,
    1778,
    9875,
    5121,
    3791,
    8590,
    6205,
    4144,
    6858,
    3338,
    7556,
    4918,
    3897,
    6911,
    1041,
    1302,
    4492,
    7073,
    6459,
    6429,
    8399,
    8274,
    3013,
};

void bubblesort(int table[], int size){
	int i, j, temp;
	for (i = 0; i<size-1; i++){
		for (j=0; j<size-1-i; j++){
			if (table[j] > table[j+1]){
				temp = table[j+1];
				table[j+1] = table[j];
				table[j] = temp;
			}
		}
    }
}

void start(void){
    bubblesort(data, 30);
    for(;;);
}