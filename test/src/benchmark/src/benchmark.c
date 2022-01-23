#include "bubblesort.h"
#include "data.h"

int main(void){
    for(int i = 0; i < 1; i++){
        if((i & 1) == 0){
            bubblesort(data, 1000);
        }else{
            bubblesort_reverse(data, 1000);
        }
    }
    
    for(;;);
}
