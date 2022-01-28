int result = 0;

int fibonacci(int n){
    if(n == 0){
        return 0;
    }

    if(n == 1){
        return 1;
    }

    return fibonacci(n - 2) + fibonacci(n - 1);
}

int main(void){
    result = fibonacci(20);
    for(;;);
}
