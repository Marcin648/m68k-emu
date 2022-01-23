#include "bubblesort.h"
#include "data.h"

int main(void){
    for(int i = 0; i < 10000; i++){
        if((i & 1) == 0){
            bubblesort(data, 30);
        }else{
            bubblesort_reverse(data, 30);
        }
    }
    
    for(;;);
}
