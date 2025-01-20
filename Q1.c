#include<stdio.h>
#include<stdlib.h>

void generate_file_name(char* file_name, int n, int format_flag){
    char num[7];
    snprintf(num,sizeof(num), "%06d", n);

    if(format_flag == 0){
        snprintf(file_name,50,"array_%s_asc.out",num);
    }

    else snprintf(file_name,50,"array_%s_bin.out",num);
}

void print_to_file(double** arr,int n, int format_flag){
    char file_name[50];

    generate_file_name(file_name,n,format_flag);

    FILE* out = fopen(file_name, format_flag == 0 ? "w" : "wb");
    if (out == NULL) {
        printf("Error: Could not open file '%s'.\n", file_name);
        exit(1);
    }

    
    if(format_flag == 0){// ascii format
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    fprintf(out,"%.15e ",arr[i][j]);
            }
            fprintf(out,"\n");
        }
    }

    //binary format
    else{
        for (int i = 0; i < n; i++) {
            fwrite(arr[i], sizeof(double), n, out);
        }
    }
}


int  main()
{
    int n;
    int format_flag = 0; // change the format flag to generate ascii and binary files accordingly
    FILE *ptr = fopen("input.in","r");

    if(ptr == NULL){
        printf("error,could not open file\n");
        exit(1);
    }

    fscanf(ptr,"%d",&n);
    fclose(ptr);

    // dynamically allocating memory to array of size nxn
    double** arr = (double**)malloc(n*sizeof(double*));

    if(arr == NULL){
        printf("error... memory allocation failed\n");
        exit(1);
    }
    
    for(int i=0;i<n;i++){
        arr[i] = (double*)malloc(n*sizeof(double));

        if(arr == NULL){
        printf("error... memory allocation failed\n");
        exit(1);
        } 
    }
    
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            arr[i][j] = (double)(i+j);
        }
    }

    print_to_file(arr,n,format_flag);
    format_flag = 1;
    print_to_file(arr,n,format_flag);

    // free allocated memory
    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }
    free(arr);


}