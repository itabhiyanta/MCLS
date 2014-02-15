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
/*  method. 									*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* The level set field is advected, but does not remain mass conserving even    */
/* when it is advected in a conservative way. In this function the level-set    */
/* field is adapted to the volume of fluid field in an iterative way.           */
/* When the maximum number of correction steps has been applied, and the        */
/* level-set field is still not completely complying with the volume of fluid   */
/* field, the computation is terminated.	                                */
/* For debugging purposes all variables involved in the corrective process are  */
/* written to file for visual inspection.					*/
/********************************************************************************/
   void dump_solution_for_debugging(
	Array3<double> level_set_star,			// level set field
	Array3<double> volume_of_fluid,		 	// volume of fluid field
	Array3<double> level_set_mass_conserving,		// corrected, mass conserving level-set field
	Array3<double> level_set_correction,		        // correction needed to make level-set mass conserving
	Array3<double> volume_of_fluid_deviation,		// difference between the converted, advected level
							// set field and the advected volume of fluid field
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
      void  write_coordinates_vtk( 			// write coordinates in vtk format 
	  ofstream& output_stream, 		
	  int number_primary_cells_i,		
	  int number_primary_cells_j,		
	  int number_primary_cells_k,		
	  double mesh_width_x1,			
	  double mesh_width_x2,			
	  double mesh_width_x3			
       );
      void  write_cell_centered_field_vtk(     	// write cell centered field in vtk format
	  ofstream& output_stream, 			
	  string scalar_name,				
	  string look_up_table_name,				
	  Array3<double> cell_centered_field, 			
	  int number_primary_cells_i,			
	  int number_primary_cells_j,			
	  int number_primary_cells_k			
	    );
      int total_number_primary_cells=		// total number of primary cells
	  number_primary_cells_i*
	    number_primary_cells_j*
	      number_primary_cells_k;
      int total_number_vertices=			// total number of vertices
	  (number_primary_cells_i+1)*
	    (number_primary_cells_j+1)*
	      (number_primary_cells_k+1);
	
       	string scalar_name;				// name of the scalar field to be written 
      	string look_up_table_name;			// name of the look-up table to be used
	
      
      
      /* dump the solution to a vtk file for visualisation */
      
		  ofstream output_vtk ( "debug_interface.vtk");
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

		  
		  scalar_name="volume_of_fluid_deviation";
		  look_up_table_name="vof_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, volume_of_fluid_deviation, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		  /* write the original level-set field */
		  
		  scalar_name="level_set_star";
		  look_up_table_name="lvst_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, level_set_star, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		  /* write the mass conserving level-set field */
		  
		  scalar_name="level_set_mass_conserving";
		  look_up_table_name="lvst_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, level_set_mass_conserving, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);


		  /* write the level-set field correction*/
		  
		  scalar_name="level_set_correction";
		  look_up_table_name="lvst_tbl";
		  
		  write_cell_centered_field_vtk( output_vtk, scalar_name, look_up_table_name, level_set_correction, 		
						number_primary_cells_i, number_primary_cells_j,number_primary_cells_k);

		}