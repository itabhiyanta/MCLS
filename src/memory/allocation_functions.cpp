
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */
#include<iostream>

/*****************************************************************************
Prototype:    double **    double_Matrix(int row, int col) 
Description:     To allocate a  matrix of double type. 
Input value: 
    int row --- the row number of the matrix 
    int col ---  column number of the matrix 
Return value:     a pointer to the matrix 
*****************************************************************************/
double **double_Matrix(int row, int column){
    	int i;
    	double **m;



    	m=(double **) calloc((unsigned) row,sizeof(double*));

    	for(i=0;i<row;i++){

        	m[i]=(double *) calloc((unsigned) column,sizeof(double));
    		if(m[i]==NULL){
			fprintf(stderr,"Allocation failed in double_Matrix with \n \
		        arguments row %i column %i \n",row,column);
			exit(1);
    		}
	}
    	if(m==NULL){
		fprintf(stderr,"Allocation failed in double_Matrix with \n \
			        arguments row %i column %i \n",row,column);
		exit(1);
    	}
   
    return m;
}



/*****************************************************************************
Prototype:    double *double_Vector(int nh) 
Description:    To allocate a vector of double type. 
Input value: 
    int nh --- length of the vector 
Return value:    pointer to the vector 
*****************************************************************************/
double *double_Vector( int nh){
    	double *v;


     	v=(double *)calloc((unsigned) nh,sizeof(double)); 
   	
	if(v==NULL){
		fprintf(stderr,"Allocation failed in double_Vector with\n  \
		        	argument nh %i \n",nh);
		exit(1);
    	}

    	return v;
}




/*****************************************************************************
Prototype:  double ***  double_Matrix2( int row, int column, int width) Description:     
To allocate a three-dimension array of double type. 
Input value: 
    int row --- row number of the matrix 
    int col --- column number of the matrix 
    int width --- width of the matrix 
Return value:  a pointer to the three-dimension array 
*****************************************************************************/
double ***double_Matrix2(int row, int column, int width)
{
    	int i,j;
    	double ***m;


    	m=(double ***) calloc((unsigned) row,sizeof(double**));

    	for(i=0;i<row;i++){

        	m[i]=(double **) calloc((unsigned) column,sizeof(double*)); 

        	for(j=0;j<column;j++){

            		m[i][j]=(double *) calloc((unsigned) width,sizeof(double)); 

    		}
    	}

	if(m==NULL){
	
		fprintf(stderr,"Allocation failed in double_Matrix2 with \n \
		        arguments row %i column %i width %i \n",row,column,width);
		exit(1);
    	}

    	return m;

}

/*****************************************************************************
Prototype:  double ***  double_Matrix2( int row, int column, int width) Description:     
To allocate a three-dimension array of double type. 
Input value: 
    int row --- row number of the matrix 
    int col --- column number of the matrix 
    int width --- width of the matrix 
Return value:  a pointer to the three-dimension array 
*****************************************************************************/
double ****double_Matrix3(int row, int column, int width, int height)
{
    	int i,j,k;
    	double ****m;

    	m=(double ****) calloc((unsigned) row,sizeof(double**));
    	for(i=0;i<row;i++){

        	m[i]=(double ***) calloc((unsigned) column,sizeof(double**)); 

        	for(j=0;j<column;j++){

            		m[i][j]=(double **) calloc((unsigned) width,sizeof(double*)); 

        		for(k=0;k<width;k++){

            			m[i][j][k]=(double *) calloc((unsigned) height,sizeof(double)); 

			}
		}
    	}

    	if(m==NULL){
		fprintf(stderr,"Allocation failed in double_Matrix2 with \n \
		        arguments row %i column %i width %i \n",row,column,width);
		exit(1);
    	}

    	return m;

}




/*****************************************************************************
Prototype:  void  free_double_Matrix( double **m, int row, int column) 
Description:     To free a matrix of double type. 
Input value: 
    double ** m --- pointer to a matrix of double type. 
    int row --- row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
Note: no reference is needed to the last dimension
*****************************************************************************/
void free_double_Matrix(double **m, int row)
{
    	int i;

    	for(i=row-1;i>=0;i--){ 
		free((char*) m[i]);
	}

        free((char*) m);
}

/*****************************************************************************
Prototype:  void free_double_Matrix2(double ***m, int row, int column)    
Description:     To free a 3D array of double type. 
Input value: 
    double *** m --- pointer to a matrix of double type. 
    int row --- row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
*****************************************************************************/
void ***free_double_Matrix2(double ***m, int row, int column)
{
    	int i,j;
//    	double ***m;

    	for(i=0;i<row;i++){

 
        	for(j=0;j<column;j++){

            		free(m[i][j]);

    		}
    		free(m[i]);
    	}
    	free(m);
}

/*****************************************************************************
Prototype:  void free_double_Matrix2(double ***m, int row, int column)    
Description:     To free a 3D array of double type. 
Input value: 
    double *** m --- pointer to a matrix of double type. 
    int row --- row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
*****************************************************************************/
void ***free_double_Matrix3(double ****m, int connections, int row, int column)
{
    	int i,j,k;
//    	double ***m;

    	for(i=0;i<connections;i++)
	{
	    for(j=0;j<row;j++)
	    {

 
        	for(k=0;k<column;k++){

            		free(m[i][j][k]);

    		}
    		free(m[i][j]);
	    }
	    free(m[i]);
	}
    	free(m);
}

/****************************************************************************
Prototype:  void  free_double_Vector(double *v, int nh) 
Description:     To free a vector of  double type. 
Input value: 
    double * v --- pointer to a vector of double type 
    int nh --- length of the vector 
Return value:     none 
****************************************************************************/
void free_double_Vector(double *v)
{
    	free((char *) v);
}



/**************************************************************************** Prototype:  void  free_INT_Matrix( int **m, int row, int column) 
Description:     To free a matrix of integer type. 
Input value: 
    int ** m --- pointer to a matrix of integer type. 
    int row ---  row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
****************************************************************************/
void free_INT_Matrix(int **m, int row, int column)
{
    	int i;

    	for(i=row-1;i>=0;i--){
		free((char*) m[i]);
	}

    	free((char*) m);

}


/****************************************************************************
Prototype:    void    free_INT_Matrix( int **m, int row, int column) 
Description:     To free a matrix of integer type. 
Input value: 
    int ** m --- pointer to a matrix of integer type. 
    int row ---  row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
*****************************************************************************/
void free_INT_Vector(int *v, int nh)
{
    	free((char *) v);
}

/****************************************************************************
Prototype:    void    free_INT_Matrix( int **m, int row, int column) 
Description:     To free a matrix of integer type. 
Input value: 
    int ** m --- pointer to a matrix of integer type. 
    int row ---  row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
*****************************************************************************/
void free_LONG_Vector(long *v, int nh)
{
    	free((char *) v);
}




/*****************************************************************************
Prototype:    int **    INT_Matrix( int row, int col) 
Description:     To allocates a matrix of  integer type 
Input value: 
    int row --- row number of the matrix 
    int col --- column number of the matrix 
Return value:     a pointer to the matrix 
*****************************************************************************/
int **INT_Matrix(int row, int column)
{
    	int i;
    	int **m;


    	m=(int **) calloc((unsigned) row,sizeof(int *)); 

    	for(i=0;i<row;i++){

        	m[i]=(int *) calloc((unsigned) column,sizeof(int)); 
	}

    	if(m==NULL){
		fprintf(stderr,"Allocation failed in INT_Matrix with \n \
		        arguments row %i column %i  \n",row,column);
		exit(1);
    	}
    	return m;
}



/*****************************************************************************
Prototype:    int ***    INT_Matrix2( int row, int column, int width) 
Description:     To allocate a three dimension array of double type. 
Input value: 
    int row --- row number of the matrix 
    int col --- column number of the matrix 
    int width --- width of the matrix 
Return value:	a pointer to a three-dimension matrix of integer type.
*****************************************************************************/
int ***INT_Matrix2( int row, int column, int width)
{
    	int i,j;
    	int ***m;

    	m=(int ***) calloc((unsigned) row,sizeof(int**)); 

    	for(i=0;i<row;i++){

	        m[i]=(int **) calloc((unsigned) column,sizeof(int*)); 

        	for(j=0;j<column;j++){

	        	m[i][j]=(int *) calloc((unsigned) width,sizeof(int)); 
		}
    	}

    	if(m==NULL){
		fprintf(stderr,"Allocation failed in INT_Matrix2 with\n  \
		        arguments row %i column %i width %i \n",row,column,width);
		exit(1);
    	}

    	return m;
}




/*****************************************************************************
Prototype:  void free_CMPLX_Matrix( complex **m, int row, int column) 
Description:     To free a  matrix of  complex type. 
Input value: 
    complex ** m --- pointer to a matrix of complex type. 
    int row --- row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
******************************************************************************/
int *INT_Vector(int nh)
{
    	int *v;

    	v=(int *)calloc((unsigned) nh,sizeof(int)); 
    	return v;
}

/*****************************************************************************
Prototype:  void free_CMPLX_Matrix( complex **m, int row, int column) 
Description:     To free a  matrix of  complex type. 
Input value: 
    complex ** m --- pointer to a matrix of complex type. 
    int row --- row number of the matrix 
    int column --- column number of the matrix 
Return value:     none 
******************************************************************************/
long *LONG_Vector(long nh)
{
    	long *v;

    	v=(long *)calloc((long ) nh,sizeof(long)); 
    	return v;
}




/***************************************************************************
Prototype:  void  Null_double_Matrix(  *Vec, int LenX, int LenY) 
Description:     To initialize  a matrix of double type to zero. 
Input value: 
    int * Vec --- pointer to a matrix of double type 
    int LenX --- row length 
    int LenY --- column length 
Return value:     none 
***************************************************************************/
void Null_double_Matrix(double **Vec,int LenX, int LenY)
{
    	int ii,jj;

    	for(ii=0;ii<LenX;ii++){
        	for(jj=0;jj<LenY;jj++){
            		Vec[ii][jj] = 0.0;
		}
	}
}



/***************************************************************************
Prototype:    void    Null_doubleVector( int *Vec, int LenX) 
Description:     To initialize  a vector of float type to zero. 
Input value: 
    int * Vec --- pointer to a vector of float type 
    int LenX --- length of the vector 
Return value:     none 
***************************************************************************/
void Null_double_Vector(double *Vec,int LenX)
{
    	int ii;

    	for(ii=0;ii<LenX;ii++){
        	Vec[ii] = 0.0;
	}

}



/****************************************************************************
Prototype:    void    Null_INTMatrix( int *Vec, int LenX, int LenY) 
Description:     To initialize a matrix of integer type to zero. 
Input value: 
    int * Vec --- pointer to a matrix of integer type 
    int LenX --- row length 
    int LenY --- column length 
Return value:     none 
****************************************************************************/
void Null_INTMatrix(int **Vec,int LenX,int LenY)
{
    	int ii,jj;

    	for(ii=0;ii<LenX;ii++){
        	for(jj=0;jj<LenY;jj++){
            		Vec[ii][jj] = 0;
		}
	}

}



/****************************************************************************
Prototype:  void  Null_INTVector( int *Vec, int LenX) 
Description:     To initialize  a vector of integer type to zero. 
Input value: 
    int * Vec --- pointer to a vector of integer type 
    int LenX --- length of the vector 
Return value:     none 
*****************************************************************************/
void Null_INTVector(int *Vec,int LenX)
{
    	int ii;

    	for(ii=0;ii<LenX;ii++){
        	Vec[ii] = 0;
	}

}



