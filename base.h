typedef struct matrix {
	int n;
	int m; 
	int *vals; 
} matrix;


matrix createMatrix(int n, int m) {
	int *ptr; 
	ptr = (int *) malloc(n * m * sizeof(int)); 
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			*(ptr + i * m + j) = 0;
		}
	}
	matrix mat = {n, m, ptr}; 
	return mat; 
}

matrix changeMatrix(matrix mat, int * vals) {
    mat.vals = realloc(vals, sizeof(int) * mat.n * mat.m); 
}

matrix tranposeMatrix(matrix mat) {
	int *ptr; 
	ptr = (int *) malloc(mat.n * mat.m * sizeof(int)); 
	for(int i = 0; i < mat.m; i++) {
		for(int j = 0; j < mat.n; j++) {
			*(ptr + i * mat.n + j) = *(mat.vals + j * mat.m + i);  
		}
	} 
	matrix transposed = {mat.m, mat.n, ptr}; 
	return transposed; 
}


void printMatrix(matrix mat) {
	for(int i = 0; i < mat.n; i++) {
		for(int j = 0; j < mat.m; j++) {
			printf("%d ", getMatrixValue(mat, i, j)); 
		}
		printf("\n");
	}
}