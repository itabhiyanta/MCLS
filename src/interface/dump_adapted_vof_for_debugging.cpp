#include "../headers/array.h"

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
/*  Function to the solution of the interface quantities to file for inspection */
/*  method. 										*/
/*  											*/
/*  Programmer	: Duncan van der Heul       						*/
/*  Date	: 10-03-2013       							*/
/*  Update	:        								*/
/********************************************************************************/
/* Notes										*/
/* The level set field is advected, but does not remain mass conserving even    */
/* when it is advected in a conservative way. In this function the level-set    */
/* field is adapted to the volume of fluid field in an iterative way.           */
/* When the maximum number of correction steps has been applied, and the        */
/* level-set field is still not completely complying with the volume of fluid   */
/* field, the computation is terminated.	                                	*/
/* For debugging purposes all variables involved in the corrective process are  */
/* written to file for visual inspection.						*/
/********************************************************************************/
EXPORT void dump_adapted_vof_for_debugging(
	Array3<double> volume_of_fluid,			// volume of fluid field
	Array3<double> vof_after_x1_update,		// volume of fluid field after update
	Array3<double> vof_after_x2_update,		// volume of fluid field after update
	Array3<double> vof_after_x3_update,		// volume of fluid field after update
	Array3<double> level_set_old,
	Array3<double> level_set_after_x1_update,
	Array3<double> level_set_after_x2_update,
	int number_primary_cells_i,			// number of primary (pressure) 
							// cells in x1 direction
	int number_primary_cells_j,			// number of primary (pressure) 
							// cells in x2 direction
	int number_primary_cells_k,			// number of primary (pressure) 
							// cells in x3 direction
	  double mesh_width_x1,			        // grid spacing in x1 direction (uniform)
	  double mesh_width_x2,			        // grid spacing in x2 direction (uniform)
	  double mesh_width_x3			        // grid spacing in x3 direction (uniform)
	
	)
	{
      int total_number_primary_cells=		        // total number of primary cells
	  number_primary_cells_i*
	    number_primary_cells_j*
	      number_primary_cells_k;
	
       	string scalar_name;				// name of the scalar field to be written 
      	string look_up_table_name;			// name of the look-up table to be used
	
      
      
      /* dump the solution to a vtk file for visualisation */
      
		  ofstream output_vtk ( "debug_vof.vtk");
		  if(!output_vtk)
		  {
		      /* the contructor returned a 0-pointer :-( */
		      cout << "Cannot open file.\n";
		      exit(1);
		  }
	    
		  /* write the header for the tecplot file */
		  /* write the header for the tecplot file */
	    
		  output_vtk << "# vtk DataFile Version 3.0 \n";
		  output_vtk << "Solution file MCLS \n";
		  output_vtk << "ASCII \n";
		  output_vtk << "DATASET RECTILINEAR_GRID\n";
		  output_vtk << "DIMENSIONS "<< number_primary_cells_i+1 << " "<< number_primary_cells_j+1;
		  output_vtk << " "<< number_primary_cells_k+1 << "\n";
		  output_vtk.setf(ios::scientific);

		  write_coordinates_vtk( output_vtk, 		
					number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,		
					  mesh_width_x1, mesh_width_x2, mesh_width_x3);
		  
		  /* write all cell centered data to file */

		  output_vtk << "CELL_DATA " << total_number_primary_cells << "\n";
		  
		  
		  /* write the volume of fluid */
		  
		  scalar_name="volume_of_fluid";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, volume_of_fluid, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		  
		  scalar_name="vof_after_x1_update";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, vof_after_x1_update, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		  scalar_name="vof_after_x2_update";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, vof_after_x2_update, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		  scalar_name="vof_after_x3_update";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, vof_after_x3_update, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);


		  scalar_name="level_set_old";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, level_set_old, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		  scalar_name="level_set_after_x1_update";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, level_set_after_x1_update, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		  scalar_name="level_set_after_x2_update";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, level_set_after_x2_update, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);





		}
