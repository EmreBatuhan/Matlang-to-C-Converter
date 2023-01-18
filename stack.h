#include "vector.h"

#define define_stack(T) \
    typedef struct T##_stack \
    { \
        T* arr; \
        int rearInd; \
        int capacity; \
        int size;  \
    } T##_stack; \
    void T##_stack_init(T##_stack* stack) { \
        stack -> capacity = 100; \
        stack -> arr = malloc(sizeof(T) * stack -> capacity); \
        stack -> size = 0; \
        stack -> rearInd = -1; \
    } \
    void T##_stack_resize(T##_stack* stack, int new_capacity) { \
        T* arr = realloc(stack -> arr, sizeof(T) * new_capacity); \
        if(arr) { \
            stack -> arr = arr; \
            stack -> capacity = new_capacity; \
        } \
    } \
    void T##_stack_push(T##_stack* stack, T el) { \
        if(stack -> size == stack -> capacity) { \
            T##_stack_resize(stack, stack -> capacity * 2); \
        } \
        stack -> rearInd++; \
        stack -> arr[stack -> rearInd] = el; \
        stack -> size++; \
    } \
    T T##_stack_pop(T##_stack* stack) { \
        stack -> rearInd--; \
        stack -> size--; \
        return stack -> arr[stack -> rearInd + 1]; \
    } \
    T T##_stack_back(T##_stack* stack) { \
        return stack -> arr[stack -> rearInd]; \
    } \

#define stack(s, T) T##_stack s; T##_stack_init(&s);  
define_stack(int); 
define_stack(string); 