#ifndef vector_class
#define vector_class

size_t CAPACITY = 100; 
#define vector_struct(name, T)            \
    typedef struct name {         \
        T * items;                        \
        size_t size;                    \
    } name;                       \


#define vector_init(name, T)                      \
    void name##_init(name * vec) {                \
        vec -> size = 0;                                    \
        vec -> items = malloc(sizeof(T) * (CAPACITY)); \
    }                                                            \

#define vector_get(name, T)                       \
    T name##_get(name * vec, size_t index) {                \
        if(index >= 0 && index < vec -> size) return vec -> items[index];    \
    }                                                                    \

#define vector_resize(name, T)    \
    void name##_resize(name * vec, int new_capacity) {    \
        T* items = realloc(vec -> items, sizeof(T) * new_capacity);  \
        if(items) { \
            vec -> items = items; \
            CAPACITY = new_capacity;   \
        }   \
    }   \


#define vector_add(name, T)                                                   \
    void name##_add(name * vec, T el) {                           \
        if(vec -> size == CAPACITY) {                                  \
            name##_resize(vec, CAPACITY * 2);                        \
        }                                                                   \
        vec -> items[vec -> size] = el;                                       \
        vec -> size++;                                                        \
    }                                                                       \


#define vector_size(name, T)  \
    int name##_size(name* vec) {  \
        return vec -> size;     \
    }   \

#define vector_concatenation(name, T) \
    name name##_cat(name* vec1, name* vec2) { \
        for(int i = 0; i < vec2 -> size; i++) { \
            name##_add(vec1, name##_get(vec2, i)); \
        } \
        return (*vec1); \
    } \

#define vector_back(name, T) \
    T name##_back(name* vec) { \
        return vec -> items[vec -> size - 1]; \
    } \

#define vector_pop(name, T) \
    T name##_pop(name* vec) { \
        T el = name##_back(vec); \
        vec -> size--; \
        return el; \
    } \

// #define vector_extend(name, T) \
//     name name##_extend(name* vec, T* items) { \
//         while(items) {	\
//             printf("%c", *items); \
//             name##_add(vec, *items); \
// 		    items++; \
//         } \
//         return *vec; \
//     } \

#define define_vector(name, T)   \
    vector_struct(name, T) vector_init(name, T) vector_get(name, T) vector_resize(name, T) \
    vector_add(name, T) vector_size(name, T) vector_concatenation(name, T) vector_back(name, T) \
    vector_pop(name, T)

#define vector(vec, name, T) name vec; name##_init(&vec);  

define_vector(int_vector, int); 
define_vector(string, char); 
define_vector(string_vector, string); 

string string_extend(string* vec, char* items) {
    while(*items != '\0') {
        string_add(vec, (char)*items);
        items++; 
    }
    return *vec; 
}

#define stringInitialize(vec, items) string_init(&vec); string_extend(&vec, items);


#endif 