#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vector.h"
#include "hashmap.h"


int main() {
    vector(s1, string, char); 
    string_add(&s1, 'c');
    string_add(&s1, 'a');
    vector(s2, string, char); 
    string_add(&s2, 'f');
    string_add(&s2, 'e');

    vector(vec, string_vector, string); 
    string_vector_add(&vec, s1); 
    string_vector_add(&vec, s2);
    for(int i = 0; i < string_vector_size(&vec); i++) {
        printf("%s\n", string_vector_get(&vec, i).items);
    } 
    HashMap(string_map, SSmap); 
    string special = {"matrix", 100, 6}; 
    SSmap_insert(&string_map, s1, special);
    SSmap_insert(&string_map, s2, special);
    printf("%s\n", SSmap_get(&string_map, s1).items); 

    return 0; 
}
