#include "../headers/array.h"
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>

/********************************************************************************/
/*  Function to include the boundary conditions into the momentum equation      */
/*  conditions									*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* In the current implementation the generation of the matrix, right-hand-side  */
/* and the application of the boundary conditions is completely separated.      */
/* In the current function the matrix is build WITHOUT considering the          */
/* boundary conditions. Next the boundary conditions are considered in a        */
/* separate 'matrix folding' function that eliminates all known values  and     */
/* discards any reference to virtual values.                                    */
/* The tasks are:								*/
/* 1) Determine the domain of unknowns						*/
/* 2) remove any connections outside the domain	using the boundary conditions	*/
/*                                                                              */
/********************************************************************************/
EXPORT void fold_momentum_rhside_u3(
      boundary_face boundary_faces[6],			// array with all the information
							// for the boundary conditions 
      Array1<double> momentum_rhside_u3,			// momentum rhside velocity x1 direction
      Array2<double> momentum_matrix_u3,			// momentum matrix velocity x1 direction
      int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
      int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
      int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
      double mesh_width_x1,				// grid spacing in x1 direction (uniform)
      double mesh_width_x2,				// grid spacing in x2 direction (uniform)
      double mesh_width_x3				// grid spacing in x3 direction (uniform)
	   )
      
{
    double boundary_value;		// value of the boundary condition prescibed:
					// dirichlet-> function value
					// neumann -> normal derivative
    int start_index_i;			// lowest index i_index in active part of matrix
    int start_index_j;			// lowest index j_index in active part of matrix
    int start_index_k;			// lowest index k_index in active part of matrix
    int final_index_i;			// highest index i_index in active part of matrix
    int final_index_j;			// highest index j_index in active part of matrix
    int final_index_k;			// highest index k_index in active part of matrix
    int cell_label_boundary;		// index of the matrix element to be manipulated
    int face_index;			// index of the face to be manipulated
    int i_index, j_index, k_index;  	// local variables for loop indexing
    int one_dimensional_index;		// index of point in 1-D array

    
   /* set default values for start and final index of the unknowns */
   
    start_index_i=1;
    final_index_i=number_primary_cells_i;
    start_index_j=1;
    final_index_j=number_primary_cells_j;
    start_index_k=0;
    final_index_k=number_primary_cells_k;
     
   /* start the loop over all faces for the matrix folding */
   
   
       /******************************************************************/
       /*   +/- I-index faces						 */
       /******************************************************************/
       for(face_index=0;face_index<=1; face_index++)
       {
	  if(face_index==0)
	  {
	      cell_label_boundary=final_index_i;
	  }
	  else
	  {
	      cell_label_boundary=start_index_i;
	  }
	  
	  boundary_value=boundary_faces[face_index].boundary_variables[1].boundary_condition_value;
			
	  if(boundary_faces[face_index].boundary_variables[2].boundary_condition_type==dirichlet)
	  {
	      
		  /* DIRICHLET BOUNDARY CONDITION */
			
		if(face_index==0)
		{
		    for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{
			
			/* the boundary condition is applied using linear interpolation */
			/* U_virtual=2*U_boundary - U_real */
			/* the coefficient of the virtual cell x 2 x boundary value */
			/* is subtracted from the rhsde */
			
			    one_dimensional_index=map_index_u3(cell_label_boundary,j_index,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
				2.0*boundary_value*
				    momentum_matrix_u3[1][one_dimensional_index];
			
			
			}	  
  
		    }  
		}  
		else
		{
		    for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{
			
			/* the boundary condition is applied using linear interpolation */
			/* U_virtual=2*U_boundary - U_real */
			/* the coefficient of the virtual cell x 2 x boundary value */
			/* is subtracted from the rhsde */

			    one_dimensional_index=map_index_u3(cell_label_boundary,j_index,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
				2.0*boundary_value*
				    momentum_matrix_u3[4][one_dimensional_index];
			
			}	  
  
		    }  
		}
	  }
	  else
	  {
	      if(boundary_faces[face_index].boundary_variables[2].boundary_condition_type==neumann)
	      {
		  /* NEUMANN BOUNDARY CONDITION */
			
		if(face_index==0)
		{
		    for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{

			/* the boundary condition is applied using second order approximation */
			/* to the normal derivative */
			/* U_virtual=U_real + dx* dU/dn */
			/* the contribution of the virtual cell is moved to the central */
			/* coefficient with equal sign */
			/* and the nonexistent connection is set to zero */

			    one_dimensional_index=map_index_u3(cell_label_boundary,j_index,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
			      momentum_matrix_u3[2][one_dimensional_index]*
				boundary_value*mesh_width_x1;
			
			}	  
  
		    }  
		}  
		else
		{
		    for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{

			/* the boundary condition is applied using second order approximation */
			/* to the normal derivative */
			/* U_virtual=U_real + dx* dU/dn */
			/* the contribution of the virtual cell is moved to the central */
			/* coefficient with equal sign */
			/* and the nonexistent connection is set to zero */

			    one_dimensional_index=map_index_u3(cell_label_boundary,j_index,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
			      momentum_matrix_u3[5][one_dimensional_index]*
				boundary_value*mesh_width_x1;
			
			}	  
  
		    }  
		}
			
	      }  
	      else
	      {
		  std::cerr<< "**************************************************** \n";
		  std::cerr<<"ERROR \n";
		  std::cerr<<"you are trying to impose a boundary condition that \n";
		  std::cerr<<"is not implemented yet, check your input";
		  std::cerr<<"in function fold_momentum_rhside_u3 line 268 \n";
		  std::cerr<<"**************************************************** \n";
		  exit(1);
	      }
	  }
  
       }  

       
       /******************************************************************/
       /*   +/- J-index faces						 */
       /******************************************************************/

 
       for(face_index=2;face_index<=3; face_index++)
       {
	  if(face_index==2)
	  {
	      cell_label_boundary=final_index_j;
	  }
	  else
	  {
	      cell_label_boundary=start_index_j;
	  }

	  boundary_value=boundary_faces[face_index].boundary_variables[2].boundary_condition_value;
	  
	  if(boundary_faces[face_index].boundary_variables[2].boundary_condition_type==dirichlet)
	  {
	      
		  /* DIRICHLET BOUNDARY CONDITION */
			
		if(face_index==2){
		    for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{

			/* the boundary condition is applied using linear interpolation */
			/* U_virtual=2*U_boundary - U_real */
			/* the coefficient of the virtual cell x 2 x boundary value */
			/* is subtracted from the rhsde */

			    one_dimensional_index=map_index_u3(i_index,cell_label_boundary,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
				2.0*boundary_value*
				    momentum_matrix_u3[2][one_dimensional_index];
			
			}	  
  
		    }  
		}  
		else
		{
		    for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{

			/* the boundary condition is applied using linear interpolation */
			/* U_virtual=2*U_boundary - U_real */
			/* the coefficient of the virtual cell x 2 x boundary value */
			/* is subtracted from the rhsde */

			    one_dimensional_index=map_index_u3(i_index,cell_label_boundary,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
				2.0*boundary_value*
				    momentum_matrix_u3[5][one_dimensional_index];
			
			
			}	  
  
		    }  
		}
	  }
	  else
	  {
	      if(boundary_faces[face_index].boundary_variables[2].boundary_condition_type==neumann)
	      {
		  /* NEUMANN BOUNDARY CONDITION */

		  if(face_index==2){
		    for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{

			/* the boundary condition is applied using second order approximation */
			/* to the normal derivative */
			/* U_virtual=U_real + dx* dU/dn */
			/* the contribution of the virtual cell is moved to the central */
			/* coefficient with equal sign */
			/* and the nonexistent connection is set to zero */

			    one_dimensional_index=map_index_u3(i_index,cell_label_boundary,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
			      momentum_matrix_u3[2][one_dimensional_index]*
				boundary_value*mesh_width_x2;
			
			}	  
  
		    }  
		}  
		else
		{
		    for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
		    {
			for(k_index=1;k_index<number_primary_cells_k;k_index++)
			{

			/* the boundary condition is applied using second order approximation */
			/* to the normal derivative */
			/* U_virtual=U_real + dx* dU/dn */
			/* the contribution of the virtual cell is moved to the central */
			/* coefficient with equal sign */
			/* and the nonexistent connection is set to zero */

			    one_dimensional_index=map_index_u3(i_index,cell_label_boundary,k_index,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]-=
			      momentum_matrix_u3[5][one_dimensional_index]*
				boundary_value*mesh_width_x2;
			
			}	  
  
		    }  
		}
			
	      }  
	      else
	      {
		  std::cerr<< "**************************************************** \n";
		  std::cerr<<"ERROR \n";
		  std::cerr<<"you are trying to impose a boundary condition that \n";
		  std::cerr<<"is not implemented yet, check your input";
		  std::cerr<<"in function fold_momentum_rhside_u3 line 414 \n";
		  std::cerr<<"**************************************************** \n";
		  exit(1);
	      }
	  }
  
       }  

      
       /******************************************************************/
       /*   +/- K-index faces						 */
       /******************************************************************/
      
       for(face_index=4;face_index<=5; face_index++)
       {
	  if(face_index==4)
	  {
	      cell_label_boundary=final_index_k;
	  }
	  else
	  {
	      cell_label_boundary=start_index_k;
	  }

	  boundary_value=boundary_faces[face_index].boundary_variables[2].boundary_condition_value;
			
	  if(boundary_faces[face_index].boundary_variables[2].boundary_condition_type==dirichlet)
	  {
		    for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
		    {
			for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
			{
			/* U_real=U_boundary */
			    one_dimensional_index=map_index_u3(i_index,j_index,cell_label_boundary,
				      number_primary_cells_i, number_primary_cells_j,  
								  number_primary_cells_k);

			    momentum_rhside_u3[one_dimensional_index]=boundary_value;
			}	  
  
		    } 
		  
	  }  
	  else
	  {
		  std::cerr<< "**************************************************** \n";
		  std::cerr<<"ERROR \n";
		  std::cerr<<"you are trying to impose a boundary condition that \n";
		  std::cerr<<"is not implemented yet, check your input";
		  std::cerr<<"in function fold_momentum_rhside_u3 line 486 \n";
		  std::cerr<<"**************************************************** \n";
		  exit(1);
	  }
  
       }  

       
  
}
