typedef struct matrix {
	int n;
	int m; 
	int *vals; 
} matrix;


matrix createMatrix(int n, int m, int initial) {
	int *ptr; 
	ptr = (int *) malloc(n * m * sizeof(int)); 
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			*(ptr + i * m + j) = initial;
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

matrix multiplymatrix(matrix mat1,matrix mat2){
	if(mat1.n != mat2.m){
		//If this is the case matrix cant be multiplied
	}
	// Assuming mat1 can be multiplied by mat2
	int *ptr;
	ptr = (int *) malloc(mat1.m * mat2.n * size0f(int));
	for(int i = 0;i < mat1.m;i++){
		for(int j = 0;j < mat2.n;j++){
			// (i,j) is the coordinate of where we are in the result matrix
			int total = 0;
			for(int k = 0;k < mat1.n;k++){//since mat1.n == mat2.m doesn't matter which one we use
				int* p1 = *(mat1.vals + i*mat1.n + k);
				int* p2 = *(mat2.vals + k*mat2.m + j);
				total += (*p1)*(*p2);
			}
			*(ptr + i*mat1.m+j) = total;
		}
		matrix resultmatrix = matrix{mat1.m,mat2.n,ptr};
		return resultmatrix;
	}

}
