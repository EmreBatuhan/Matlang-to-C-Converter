#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vector.h"
#include "hashmap.h"
#include "stack.h"

CImap specials; 
SImap types; 
SSmap varmap;
SImap precedence;  
string scalar, vec, matrix, print;
string sub, add, mul, choose, tr, sqr; 

string read_file_until(FILE *fp, int chr) {
    vector(str, string, char);
    int c; 
    while((c = fgetc(fp)) != EOF && c != chr) {
        string_add(&str, (char) c); 
    }
    return str; 
} 

string read_until(string line, char chr) {
    vector(str, string, char);
    int index = 0;   
    while(index < string_size(&line) && string_get(&line, index) != chr) {
        string_add(&str, string_get(&line, index)); 
        index += 1; 
    }
    return str; 
} 

string read_word_line(string line, int index) {
    vector(str, string, char);
    int size = string_size(&line);  
    while(index < size && !CImap_search(&specials, string_get(&line, index))) {
        char c = string_get(&line, index);
        string_add(&str, c);
        index += 1;  
    }
    return str; 
}

string_vector parse_line(string line) {
    vector(parsed_line, string_vector, string); 
    int index = 0, size = string_size(&line);  
    while(index < size) {
        string word = read_word_line(line, index);
        index += string_size(&word); 
        if(string_size(&word) != 0) {
            string_vector_add(&parsed_line, word); 
        }
        if(CImap_search(&specials, string_get(&line, index)) && string_get(&line, index) != ' ') {
            char c = string_get(&line, index); 
            vector(special, string, char);  
            string_add(&special, c);
            string_vector_add(&parsed_line, special);
        }
        index += 1; 
    }
    return parsed_line; 
}

string read_line(FILE *fp) {
    return read_file_until(fp, '\n'); 
}

void handle_definition(string_vector line, int line_number, FILE* out) {
    string type = string_vector_get(&line, 0); 
    string name = string_vector_get(&line, 1);
    // if(check_valid(name)) {
    //     // give error -> variable name is not valid
    // }
    if(SSmap_search(&varmap, name)) {
        // give error -> can't define same thing twice
    }
    //put matrix to file
    if(stringComp(type, matrix)) {
        SSmap_insert(&varmap, name, matrix); 
        string n = string_vector_get(&line, 3); 
        string m = string_vector_get(&line, 5); 
        fputs("matrix ", out); 
        fputs(name.items, out);
        fputs(" = createMatrix(", out);
        fputs(n.items, out);
        fputs(", ", out);  
        fputs(m.items, out);
        fputs(");\n", out);  
    }
    //put vector to file
    else if(stringComp(type, vec)) {
        SSmap_insert(&varmap, name, matrix); 
        string n = string_vector_get(&line, 3); 
        fputs("matrix ", out); 
        fputs(name.items, out);
        fputs(" = createMatrix(", out);
        fputs(n.items, out);
        fputs(", ", out);  
        fputs("1", out);
        fputs(");\n", out);  
    }
    //put scalar to file
    else{
        SSmap_insert(&varmap, name, scalar);         
        fputs("float ", out); 
        fputs(name.items, out); 
        fputs(";\n", out); 
    }
}
// TODO : virgüller handle edilecek
void evaluate(string_vector line, FILE* out) {
    vector(stack, string_vector, string); 
    string openPar; stringInitialize(openPar, "("); 
    string closePar; stringInitialize(closePar, ")"); 
    vector(evaluation, string_vector, string);
    for(int i = 0; i < line.size; i++) printf("%s ", string_vector_get(&line, i).items);
    printf("\n"); 
    for(int i = 0; i < string_vector_size(&line); i++) {
        string x = string_vector_get(&line, i); 
        if(SImap_search(&precedence, x)) {
            while(stack.size && !stringComp(string_vector_back(&stack), openPar) && 
                SImap_get(&precedence, string_vector_back(&stack)) > SImap_get(&precedence, x) 
            ) {
                string_vector_add(&evaluation, string_vector_pop(&stack)); 
            }
            string_vector_add(&stack, x); 
        }
        else if(stringComp(x, openPar)) {
            string_vector_add(&stack, x); 
        }
        else if(stringComp(x, closePar)) {
            while(stack.size && !stringComp(string_vector_back(&stack), openPar)) {
                string_vector_add(&evaluation, string_vector_pop(&stack)); 
            }
            string_vector_pop(&stack); 
        }
        else{
            string_vector_add(&evaluation, x); 
        }
    }
    evaluation = string_vector_cat(&evaluation, &stack); 
    for(int i = 0; i < evaluation.size; i++) printf("%s ", string_vector_get(&evaluation, i).items);
    printf("\n"); 

    vector(stack2, string_vector, string); 
    string comma; stringInitialize(comma, ","); 
    for(int i = 2; i < evaluation.size; i++) {
        string x = string_vector_get(&evaluation, i); 
        if(!SImap_search(&precedence, x)) {
            string_vector_add(&stack2, x); 
        }
        else if(stringComp(x, comma)) {
            string back = string_vector_back(&stack2); 
            string_cat(&back, &comma); 
        }
        else{
            string first = string_vector_pop(&stack2); 
            string expr; 
            //handle where variables are matrices 
            if(stringComp(SSmap_get(&varmap, first), matrix)) {
                if(stringComp(x, add) || stringComp(x, sub) || stringComp(x, mul)) {
                    string second = string_vector_pop(&stack2);
                    if(!stringComp(SSmap_get(&varmap, second), matrix)) {
                        //give error
                    }
                    if(stringComp(x, add)) {
                       stringInitialize(expr, "matrixAdd(")
                    }
                    else if(stringComp(x, sub)) {
                        stringInitialize(expr, "matrixSub(")
                    }
                    else if(stringComp(x, mul)) {
                        stringInitialize(expr, "matrixMul(")
                    }
                    expr = string_cat(&expr, &second); 
                    expr = string_extend(&expr, ", "); 
                }
                else{
                    expr = string_extend(&x, "("); 
                }
                expr = string_cat(&expr, &first);
                expr = string_extend(&expr, ")");  
                SSmap_insert(&varmap, expr, matrix); 
            }
            else{
                stringInitialize(expr, ""); 
                string second = string_vector_pop(&stack2);
                if(stringComp(x, add) || stringComp(x, sub) || stringComp(x, mul)) {
                    expr = string_cat(&expr, &second); 
                    expr = string_cat(&expr, &x); 
                    expr = string_cat(&expr, &first);
                }

            }
            string_vector_add(&stack2, expr); 
        }
    }
    for(int i = 0; i < stack2.size; i++) printf("%s ", string_vector_get(&stack2, i).items);
    printf("\n"); 
}

void handle_statement(string_vector line, int line_number, FILE* out) {
    string name = string_vector_get(&line, 0); 
    //put matrix declarations to file
    if( !strcmp(string_vector_get(&line, 2).items, "{") ) {
        fputs(name.items, out);
        fputs(".vals = {", out); 
        int i; 
        for(i = 3; i < string_vector_size(&line) - 2; i++) {
            fputs(string_vector_get(&line, i).items, out);
            fputs(", ", out); 
        }
        fputs(string_vector_get(&line, i++).items, out);
        fputs("};\n", out);
    }
    else{
        //evaluate and put right hand side 
        evaluate(line, out); 
    }
}

void handle_print(string_vector line, int line_number, FILE* out) {
    string name = string_vector_get(&line, 2); 
    string type = SSmap_get(&varmap, name); 
    fputs("print_", out); 
    fputs(type.items, out); 
    fputs("(", out); 
    fputs(name.items, out);
    fputs(");\n", out); 
}


int main(int argc, char * argv[]) {
    CImap_init(&specials);
    CImap_insert(&specials, ' ', 1);
    CImap_insert(&specials, ':', 1);
    CImap_insert(&specials, ',', 1);
    CImap_insert(&specials, ')', 1);
    CImap_insert(&specials, '[', 1);
    CImap_insert(&specials, '(', 1);
    CImap_insert(&specials, ']', 1);
    CImap_insert(&specials, '{', 1);
    CImap_insert(&specials, '}', 1);
    CImap_insert(&specials, '*', 1);
    CImap_insert(&specials, '=', 1);
    CImap_insert(&specials, '-', 1);
    CImap_insert(&specials, '+', 1);

    stringInitialize(scalar, "scalar"); 
    stringInitialize(vec, "vector"); 
    stringInitialize(matrix, "matrix"); 
    stringInitialize(print, "print"); 
    SImap_init(&types); 
    SImap_insert(&types, scalar, 1); 
    SImap_insert(&types, vec, 1); 
    SImap_insert(&types, matrix, 1);   

    stringInitialize(add, "+"); 
    stringInitialize(sub, "-"); 
    stringInitialize(mul, "*"); 
    stringInitialize(choose, "choose"); 
    stringInitialize(tr, "tr"); 
    stringInitialize(sqr, "sqrt"); 
    SImap_init(&precedence); 
    SImap_insert(&precedence, sub, 0);
    SImap_insert(&precedence, add, 1);
    SImap_insert(&precedence, mul, 2);
    SImap_insert(&precedence, choose, 3); 
    SImap_insert(&precedence, tr, 3);
    SImap_insert(&precedence, sqr, 3); 



    FILE *fp; 
    if(argc != 2) {
        printf("Give filename as command line argument"); 
        return (1); 
    }
    fp = fopen(argv[1], "r"); 
    if(fp == NULL) {
        printf("cannot open %s\n", argv[1]); 
        return (1); 
    }
    string line = {argv[1], 10}; 
    string file_name = read_until(line, '.');
    FILE *out = fopen(strcat(file_name.items, ".c"), "w"); 
    fputs("#include \"base.h\" \nint main() { \n", out); 

    SSmap_init(&varmap); 
    int line_number = 1; 
    while(!feof(fp)) {
        string line = read_line(fp); 
        string_vector parsed_line = parse_line(line);

        if( SImap_search(&types, string_vector_get(&parsed_line, 0)) ) {
            handle_definition(parsed_line, line_number, out);
        }
        else if( SSmap_search(&varmap, string_vector_get(&parsed_line, 0)) ){
            handle_statement(parsed_line, line_number, out); 
        }
        else if( stringComp(string_vector_get(&parsed_line, 0), print) ) {
            handle_print(parsed_line, line_number, out);
        }
    }
    fputs("}", out); 
    fclose(fp); 
    fclose(out); 
    return 0; 
}