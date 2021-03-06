#include "../headers/array.h"
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>

/********************************************************************************/
/********************************************************************************/
/*  Function to compute the actual time step size for advancing the solution    */
/*  to the new time level.							*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* The time step is either fixed or adaptive. In the latter case two criteria   */
/* have to be respected:                     					*/
/* -capillary time step constraint                                         	*/
/* -convective time step constraint                                             */
/********************************************************************************/
//

EXPORT void compute_time_step_size(
      Array3<double> u_1_velocity_new, 		// velocity field at new time level x1 direction
      Array3<double> u_2_velocity_new, 		// velocity field at new time level x2 direction
      Array3<double> u_3_velocity_new,		// velocity field at new time level x3 direction
      double rho_plus_over_rho_minus,		// ratio of the densities of the two phases
      double maximum_weighted_curvature,	// maximum 'active' value of the curvature 
						// used to evaluate the capillary time step
						// restriction 
      double sigma_over_rho_minus,		// sigma / rho_minus (scaled sigma)
      double cfl_number_navier_stokes,		// courant-friedrichs-lewy number for the navier-stokes
						// equations
      int number_primary_cells_i,		// number of primary (pressure) cells in x1 direction
      int number_primary_cells_j,		// number of primary (pressure) cells in x2 direction
      int number_primary_cells_k,		// number of primary (pressure) cells in x3 direction
      double mesh_width_x1,			// grid spacing in x1 direction (uniform)
      double mesh_width_x2,			// grid spacing in x2 direction (uniform)
      double mesh_width_x3,			// grid spacing in x3 direction (uniform)
      double time_step_restriction_global,	// upper bound on the time step, set by user
      int fixed_time_step,			// =1 the time step is not restricted by 
						// physical constraints 
						// =0 the time step is adapted to the physical
						// constraints
      double actual_time_step_navier_stokes	// the time step used to advance the flow field
						// to the new time level
	   )
      {


//       double time_step_restriction_surface_tension; 		// time step restriction due to 
// 								// surface tension
//       double time_step_restriction_convection;	    		// time step restriction due to convection
      double time_step_restriction;				// time step restriction
      double one_over_time_step_restriction;	    		// reciprocal value of the time step 
								// restriction
      double one_over_time_step_restriction_convection;		// reciprocal value of the time step 
								// restriction due to convection
      double one_over_time_step_restriction_surface_tension;	// reciprocal value of the time step 
								// restriction due to surface tension
      double smallest_mesh_width; 		    		// smaller mesh width in the grid
      double tentative_maximum_combined_u_over_mesh_width;	// tentative value for the maximum value
								// of velocity/meshwidth
      double maximum_combined_u_over_mesh_width=0;			// maximum value of the velocity/mesh_width
      int i_index, j_index, k_index; 				// local variables for loop indexing

      
      if(!fixed_time_step)
      {
      /* the time step is determined by the convective and capillary time step constraints */		
	
	
	  smallest_mesh_width=std::min(mesh_width_x1, std::min(mesh_width_x2, mesh_width_x3));
      
      /* evaluate one over the capillary time step constraint */
      
	  one_over_time_step_restriction_surface_tension=sqrt( (sigma_over_rho_minus*maximum_weighted_curvature)/
					      (0.5*(1.0+rho_plus_over_rho_minus)*smallest_mesh_width));
      
      /* evaluate one the time step constraint based on the courant friedrichs lewy number */
      
      
      /* first compute the combined U_max/(mesh_width)  */
      /* this formulation in the reciprocal value is chosen to avoid division by a zero velocity field */
      
	  for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
	  {
	      for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
	      {
		  for(k_index=1;k_index<number_primary_cells_k+1;k_index++)
		  {
		      tentative_maximum_combined_u_over_mesh_width=
			  fabs(u_1_velocity_new[i_index][j_index][k_index]/mesh_width_x1)+
			  fabs(u_2_velocity_new[i_index][j_index][k_index]/mesh_width_x2)+
			  fabs(u_3_velocity_new[i_index][j_index][k_index]/mesh_width_x3);
		       maximum_combined_u_over_mesh_width= std::max(maximum_combined_u_over_mesh_width,
								tentative_maximum_combined_u_over_mesh_width);
		      
		  }  
  
	      }  
     
	  } 
	  
      /* the local courant friedrichs lewy number of the flow is equal to the specified */
      /* courant friedrichs lewy number */
      
	  one_over_time_step_restriction_convection=maximum_combined_u_over_mesh_width/cfl_number_navier_stokes;

      /* determine which of the two restrictions on the time step is the more strict one */
      /* naturally, the LARGEST of the two RECIPROCAL values is the stricter */
      
	  one_over_time_step_restriction=std::max(one_over_time_step_restriction_convection, 
						one_over_time_step_restriction_surface_tension);

      /* take the reciprocal value of the reciprocal value */
      
	  time_step_restriction=1.0/one_over_time_step_restriction;
      
      
      /* the new time step size is the smaller of the restrictions and the explicitly set upper bound */
      
	  actual_time_step_navier_stokes=std::min(time_step_restriction, time_step_restriction_global);
      }
      else	
      {
	
	  actual_time_step_navier_stokes=time_step_restriction_global;
      }
      
}
