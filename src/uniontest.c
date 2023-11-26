#include<stdio.h>
#include<string.h>

struct s1 {
    int type;
    float val;
} typedef s1;

struct s2 {
    int type;
    char data[4];
} typedef s2;

union test_union {
    int type;
    s1 t1;
    s2 t2;
} typedef test_union;

int main(int argc, char **argv) {
    test_union tu;
    memset(&tu, 0, sizeof(test_union));

    tu.t1.type = 1;
    tu.t1.val = 100;
    
    printf("tu type: %d\n", tu.type);

    tu.t2.type = 2;
    tu.t2.data[0] = 'f';

    printf("tu type: %d\n", tu.type);
    
    return 0;
}