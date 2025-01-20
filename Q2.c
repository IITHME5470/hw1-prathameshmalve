#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// function to generate the matrix name
void generate_mat_name(char* mat_name, int n){
    char num[7];
    snprintf(num,sizeof(num), "%06d", n);

    snprintf(mat_name,200,"./inputfiles/mat_%s.in",num);
}

// Function to generate the vector name
void generate_vec_name(char* vec_name, int n,int i){
    char num[7];
    char vecnum[7];
    snprintf(num,sizeof(num), "%06d", n);
    snprintf(vecnum,sizeof(vecnum), "%06d", i);

    snprintf(vec_name,200,"vec_%s_%s.in",num,vecnum);
}

// Function to check eigenvector
void check_eigen_vector(double** arr, double* vector, int n, FILE* vec_file, char* vec_name){
    double* Ax = (double*)calloc(n, sizeof(double));
    if(Ax == NULL){
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    // Compute Ax = A * vector
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            Ax[i] += arr[i][j] * vector[j];
        }
    }

    double eigenvalue;
    bool is_eigenvector = true;

    if (vector[0] == 0) {
        if (Ax[0] == 0) {
            // If vector[0] == 0 and Ax[0] == 0, continue to check further elements
            for (int i = 1; i < n; i++) {
                if (vector[i] != 0) {
                    eigenvalue = Ax[i] / vector[i];
                    break;
                }
            }
        } else {
            // If Ax[0] is non-zero and vector[0] is zero, not an eigenvector
            is_eigenvector = false;
        }
    } else {
        eigenvalue = Ax[0] / vector[0];
    }

    // Check for the rest of the elements
    if (is_eigenvector) {
        for(int i = 0; i < n; i++){
            if (vector[i] != 0) {
                double curr_ratio = Ax[i] / vector[i];
                if(fabs(curr_ratio - eigenvalue) > 1e-8){
                    is_eigenvector = false;
                    break;
                }
            } 
            else if(Ax[i] != 0) { 
                is_eigenvector = false;
                break;
            }
        }
    }

    if(is_eigenvector){
        printf("%s : Yes : %.15e\n", vec_name, eigenvalue);
        fprintf(vec_file, "\nEigenvalue for matrix_%06d.in: %.15e",n,eigenvalue);
    }
    else {
        printf("%s : Not an eigenvector\n", vec_name);
    }

    free(Ax);
}


int main()
{
    int n;
    FILE *ptr = fopen("input.in","r");

    if(ptr == NULL){
        printf("error,could not open file\n");
        exit(1);
    }

    fscanf(ptr,"%d",&n); //read from input file
    fclose(ptr);

    char mat_name[200];
    generate_mat_name(mat_name,n);
    FILE *matrix_file = fopen(mat_name,"r"); //open the matrix file

    double** arr = (double**)malloc(n*sizeof(double*));

    if(arr == NULL){
        printf("error... memory allocation failed\n");
        exit(1);
    }
    
    //dynamic memory allocation for the matrix
    for(int i=0;i<n;i++){
        arr[i] = (double*)malloc(n*sizeof(double));

        if(arr[i] == NULL){
        printf("error... memory allocation failed\n");
        exit(1);
        } 
    }

    //read from the matrix file
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            fscanf(matrix_file,"%lf,",&arr[i][j]);
        }
    }
    
    fclose(matrix_file);

    // for(int i=0;i<n;i++){
    //     for(int j=0;j<n;j++){
    //         printf("%.15e ",arr[i][j]);
    //     }

    //     printf("\n");
    // }

    //return 0;


    for(int i=1;i<=4;i++){

        char vec_name[100];
        generate_vec_name(vec_name,n,i);
        char vec_path[200];
        snprintf(vec_path,sizeof(vec_path),"./inputfiles/%s",vec_name);
        // printf("Matrix file: %s\n", mat_name);
        // printf("Vector file: %s\n", vec_path);

        //open the vector file
        FILE* vec_file = fopen(vec_path,"r+");
        if(vec_file == NULL){
            printf("Error: Could not open file %s\n", vec_name);
            exit(1);
        }

        double* vector = (double*)malloc(n*sizeof(double));
        if (vector == NULL) {
            printf("Error: Memory allocation failed.\n");
            exit(1);
        }

        for(int i=0;i<n;i++){
            fscanf(vec_file,"%lf,",&vector[i]); // read from vector file and store it in dynamic array
        }

        check_eigen_vector(arr,vector,n,vec_file,vec_name); //check if the vector is eigenvector
        free(vector); // free the dynamic memory allocated to the vector
    }

    // free the memory allocated dynamically to the matrix
    for(int i=0;i<n;i++) free(arr[i]);
    free(arr);

}