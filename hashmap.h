#include "vector.h"
#include <stdio.h>
#include <stdbool.h>

#define TABLE_SIZE 100001

#define defineHashmap(name, K, V, HashFunc, compareFunc) \
    typedef struct name##_node { \
        K key; \
        V value; \
        struct name##_node * next; \
    } name##_node ; \
    typedef struct \
    { \
        name##_node ** htable; \
        int size; \
    } name ; \
    void name##_init(name* hashmap) { \
        hashmap -> htable = malloc(sizeof(name##_node*) * (TABLE_SIZE)); \
        for(int i = 0; i < TABLE_SIZE; i++) hashmap -> htable[i] = NULL; \
        hashmap -> size = 0; \
    } \
    bool name##_search(name* hashmap, K key) { \
        int hash_val = HashFunc(key); \
        name##_node* entry = hashmap -> htable[hash_val]; \
        while(entry != NULL) { \
            if(compareFunc(key, entry -> key)) { \
                return 1; \
            } \
            entry = entry -> next; \
        } \
        return 0; \
    } \
    void name##_update(name* hashmap, K key, V value) { \
        if (! (name##_search(hashmap, key))) { \
            printf("Key is not in the hashmap, can't update"); \
            return; \
        } \
        int hash_val = HashFunc(key); \
        name##_node* prev = NULL; \
        name##_node* entry = hashmap -> htable[hash_val]; \
        while (!compareFunc(entry -> key, key)) \
        { \
            prev = entry; \
            entry = entry->next; \
        } \
        entry -> value = value; \
    } \
    void name##_insert(name* hashmap, K key, V value) { \
        if(name##_search(hashmap, key)) { \
            name##_update(hashmap, key, value); \
        } \
        int hash_val = HashFunc(key); \
        name##_node* prev = NULL; \
        name##_node* entry = hashmap -> htable[hash_val]; \
        while (entry != NULL) \
        { \
            prev = entry; \
            entry = entry -> next; \
        } \
        if(entry == NULL) { \
            entry = malloc(sizeof(name##_node)); \
            entry -> key = key; \
            entry -> value = value; \
            if(prev == NULL) { \
                hashmap -> htable[hash_val] = entry; \
            } \
            else{ \
                prev -> next = entry; \
            } \
        } \
        else{ \
            entry -> value = value; \
        } \
        hashmap -> size += 1; \
    } \
    V name##_get(name* hashmap, K key) { \
        int hash_val = HashFunc(key); \
        name##_node* entry = hashmap -> htable[hash_val]; \
        while (entry != NULL) { \
            if (compareFunc(entry -> key, key)) { \
                return entry -> value; \
            } \
            entry = entry -> next; \
        } \
    } \


int stringHash(string key) {
    unsigned long hash = 5381;
    int c;
    while (c = *(key.items++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % TABLE_SIZE;
}

bool stringComp(string s1, string s2) {
    int size1 = string_size(&s1); 
    int size2 = string_size(&s2); 
    if(size1 != size2) return 0; 
    for(int i = 0; i < size1; i++) {
        if(string_get(&s1, i) != string_get(&s2, i)) {
            return 0; 
        }
    }
    return 1; 
}

int charHash(char c) {
    unsigned long hash = 5381;
    int d = (int) c;
    hash = ((hash << 5) + hash) + d; 
    return hash % TABLE_SIZE; 
}

bool charComp(char c1, char c2) {
    if(c1 != c2) return 0; 
    return 1; 
}

defineHashmap(SSmap, string, string, stringHash, stringComp); 
defineHashmap(SImap, string, int, stringHash, stringComp);
defineHashmap(CImap, char, int, charHash, charComp);

#define HashMap(map, name) name map; name##_init(&map); 