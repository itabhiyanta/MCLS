#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
/********************************************************************************/
/********************************************************************************/
/*  Function to write cell centered scalar field to file                        */
/*  method. 									*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/*  										*/
/*  										*/
/*  										*/
/*  										*/
/********************************************************************************/

      void  write_vertex_centered_vector_field_vtk( 
	ofstream& output_stream, 			// stream connected to output file
	string vector_name,				// name of the scalar field to write to file 
	string look_up_table_name,			// name of the look-up table
	double ***vertex_centered_vector_field_1, 	// cell centered vector field, x1 component
	double ***vertex_centered_vector_field_2, 	// cell centered vector field, x2 component
	double ***vertex_centered_vector_field_3, 	// cell centered vector field, x3 component
	int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
	int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
	int number_primary_cells_k			// number of primary (pressure) cells in x3 direction
	    )
  {
	
      int i_index, j_index, k_index; 		// local variables for loop indexing
      int full_row=2;				// a full row of 3*3=9 numbers has been written to file

      /* write short header */
      
      output_stream << "VECTORS " <<vector_name << " float \n";
 	    
      for(k_index=0;k_index<number_primary_cells_k+1;k_index++)
      {
	  for(j_index=0;j_index<number_primary_cells_j+1;j_index++)
	  {
	      for(i_index=0;i_index<number_primary_cells_i+1;i_index++)
	      {
		   full_row--;
		   output_stream <<vertex_centered_vector_field_1[i_index][j_index][k_index] << " ";
		   output_stream <<vertex_centered_vector_field_2[i_index][j_index][k_index] << " ";
		   output_stream <<vertex_centered_vector_field_3[i_index][j_index][k_index] << " ";
// 		   if(!full_row)
// 		   {
			output_stream<<"\n";
			full_row=2;
// 		   }
	      }
	  }
      }
      
      if(full_row!=2)
      {
	  output_stream<<"\n";
      }
  }
      
