#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
enum variable{velocity_u1, velocity_u2, velocity_u3, level_set, pressure};
enum boundary_conditions_type{dirichlet, neumann, periodic};
enum boundary_conditions_rule{constant, function};
enum cell_centerings{cell_centered, vertex_centered};
enum geometry{bubbly_flow, wavy_flow};


class boundary_variable
{
public:
  variable variable_name;
  boundary_conditions_type boundary_condition_type;
  boundary_conditions_rule boundary_condition_rule;
  cell_centerings cell_centering;
  double boundary_condition_value;
  boundary_variable(variable varname, boundary_conditions_type bound_type,
				     boundary_conditions_rule bound_rule,
				     cell_centerings  cell_cent,
					double bound_value );
  boundary_variable(variable varname);
};

class boundary_face
{
public:
    boundary_variable boundary_variables[5];
    boundary_face(void);
   
};
class coordinate
{
public:
  double x1,x2,x3;
  coordinate(double xx1=0, double xx2=0, double xx3=0){x1=xx1;x2=xx2;x3=xx3;}
};
class bubble
{
public:
  double principle_axis_x1;
  double principle_axis_x2;
  double principle_axis_x3;
  int label;
  coordinate center_location;
  bubble(int number, coordinate bubble_center, double bubble_radius);
};
class surface
{
public:
  int active;
  int orientation;
  double height;
};
class vector
{
public:
  double u1,u2,u3;
};
class restart_parameters
{
public:
      int start_from_restart_file;		
      int write_solution_to_restart_file;
      string name_restart_file_to_write;
      string name_restart_file_to_read;
      restart_parameters(void);
};
/********************************************************************************/
/********************************************************************************/
/*  initialize_computation						     	       */
/*  Description : perform initialization for the computation       		*/
/*  Programmer	: Duncan van der Heul       					       */
/*  Date	: 03-01       							       */
/*  Update	:        							       */
/********************************************************************************/
void initialize_computation( 
      double *** &u_1_velocity_new, 				// velocity field at new time level x1 direction
      double *** &u_2_velocity_new, 				// velocity field at new time level x2 direction
      double *** &u_3_velocity_new,				// velocity field at new time level x3 direction
      double *** &u_1_velocity_old, 				// velocity field at old time level x1 direction
      double *** &u_2_velocity_old, 				// velocity field at old time level x2 direction
      double *** &u_3_velocity_old,				// velocity field at old time level x3 direction
      double *** &pressure,					// pressure
      double *** &level_set_old,				// level-set field at old time level
      double *** &level_set_new,				// level-set field at new time level
      double *** &volume_of_fluid,				// volume of fluid field
      double *** &curvature,					// interface curvature
      double *** &unsmoothed_curvature,			// interface curvature without smoothing
      double *** & momentum_source_term_u_1,              // source term of the momentum equation in x1 direction
                                                          // defined on all u1 points (including boundaries)
      double *** & momentum_source_term_u_2,              // source term of the momentum equation in x2 direction
                                                          // defined on all u1 points (including boundaries)
      double *** & momentum_source_term_u_3,              // source term of the momentum equation in x3 direction
                                                          // defined on all u1 points (including boundaries)
      double *** & surface_tension_body_force_x1,         // source term of the momentum equation in x1 direction
                                                          // defined on all u1 points (including boundaries)
      double *** & surface_tension_body_force_x2,         // source term of the momentum equation in x2 direction
                                                          // defined on all u1 points (including boundaries)
      double *** & surface_tension_body_force_x3,         // source term of the momentum equation in x3 direction
                                                          // defined on all u1 points (including boundaries)
      double *** & scaled_density_u1,                     // scaled density for the controlvolumes
                                                          // of the momentum equation in x1 direction
      double *** & scaled_density_u2,                     // scaled density for the controlvolumes
                                                          // of the momentum equation in x2 direction
      double *** & scaled_density_u3,                     // scaled density for the controlvolumes
                                                          // of the momentum equation in x3 direction
      boundary_face boundary_faces[6],			// array with all the information
								// for the boundary conditions 
      int &number_primary_cells_i,				// number of primary (pressure) cells in x1 direction
      int &number_primary_cells_j,				// number of primary (pressure) cells in x2 direction
      int &number_primary_cells_k,				// number of primary (pressure) cells in x3 direction
      double &actual_time_step_level_set,			// time step used for level-set advection
								// possibly subscycling
      double &mesh_width_x1,					// grid spacing in x1 direction (uniform)
      double &mesh_width_x2,					// grid spacing in x2 direction (uniform)
      double &mesh_width_x3,					// grid spacing in x3 direction (uniform)
      int &apply_mass_distribution_algorithm,    		// =1:apply the mass redistribution algorithm
								// to avoid numerical vapour
      int &apply_mass_conservation_correction,    		// =1:apply the mass conservation correction
								// to the level-set field
      double &volume_of_fluid_tolerance,			// tolerance in volume of fluid field
								// for admissable values
      double &lower_bound_derivatives,			// lower bound for the first partial derivatives
								// to consider it a limiting case of vanishing
								// partial derivatives
      int &number_vof_2_level_set_iterations,		// number of OUTER iterations in the conversion 
								// from volume of fluid to level-set
      int &number_iterations_ridder,				// maximum number of iterations allowed in the
								// nonlinear root finding algorithm
								// this is the number of INNER iterations
      double &vof_2_level_set_tolerance,			// tolerance in the conversion from volume
								// of fluid value to level-set value
      double &cfl_number_reinitialization,			// courant-friedrichs-lewy number for the reinitialization
								// equation
      double &cfl_number_navier_stokes,			// courant-friedrichs-lewy number for the navier-stokes
								// equations
      int &maximum_reinitialization_steps,			// maximum number of time steps in the reinitialization
								// algorithm
      double &tolerance_reinitialization,			// stop the reinitialization when the infinite norm of
								// the pseudo time derivative has fallen below this
								// tolerance value
      int &number_matrix_connections,			// number of connections in momentum matrix				       
      vector &gravity,					// gravitational acceleration vector 
      double &actual_time_step_navier_stokes,		// time step used for level-set advection
								// computed from all stability restrictions and 
								// possibly subscycling
      double &rho_plus_over_rho_minus,			// ratio of the densities of the two phases
      double &smoothing_distance_factor,			// the smoothing distance is smoothing_distance_factor
								// times the smallest mesh width
      double &rho_minus_over_mu_minus,			// this was the 'Reynolds' number
								// in the original implementation of Sander
      double &mu_plus_over_mu_minus,				// ratio of the viscosities of both phases
      double &tolerance_pressure,	  			// the tolerance with which the system for the pressure is solved	
      int &maximum_iterations_allowed_pressure,		// maximum number of iterations allowed for the
								// conjugate gradient method
      double &tolerance_velocity,	  			// the tolerance with which the system for the momentum predictor is solved	
      int &maximum_iterations_allowed_velocity,		// maximum number of iterations allowed for the
								// conjugate gradient method
      int &continuous_surface_force_model,       		// =1, the continuous surface force model is applied
					        		// =0, the exact interface boundary conditions are applied
      int &source_terms_in_momentum_predictor,   		// =1, the source terms are applied in the momentum predictor
					        		// equation
					        		// =0, the source terms are applied in the momentum corrector
      double &sigma_over_rho_minus,				// sigma / rho_minus (scaled sigma)
      double &time_step_restriction_global,      		// upper bound on the time step, set by user
      int &fixed_time_step,					// =1 the time step is not restricted by 
								// physical constraints 
								// =0 the time step is adapted to the physical
								// constraints
      int &apply_curvature_smoothing,			// =1, apply curvature smoothing
								// =0, use unsmoothed curvature
      int &number_curvature_smoothing_steps,		// number of iterations applied in the
								// curvature smoothing algorithm
      int &apply_curvature_smoothing_filter,		// =1, apply curvature smoothing filter
								// =0, do not apply curvature smoothing filter
      int &number_of_subcycles,				// for every time step for the Navier-Stokes
								// equations number_of_subcycles steps are taken
								// with the interface evolution equations
      int &vtk_output,					// =1, write output in vtk format
								// =0, skip output in vtk format
      int &tecplot_output,					// =1, write output in tecplot format
								// =0, skip output in tecplot format
      double &time_interval_for_output,			// interval in time for which the solution is written to file
						
  
      double &domain_size_x1,					// length of edges of domain in x1 direction 
      double &domain_size_x2,					// length of edges of domain in x1 direction 
      double &domain_size_x3,					// length of edges of domain in x1 direction 
      geometry &flow_type,					// the kind of initial condition that has to be applied
      bubble *the_bubbles,					// array with definitions of the bubbles
      int &number_of_bubbles,					// number of bubbles in the initial condition
      surface *the_free_surfaces,				// array with definitions of the free surfaces
      int &number_of_free_surfaces, 				// number of bubbles in the domain (<10)
      double &start_time_simulation,				// starting time for the simulation
      double &end_time_simulation,				// end time for the simulation
      vector &initial_velocity,				// initial velocity field at t=0
      restart_parameters &my_restart_parameters, 		// all parameters for reading/writing restart files
      int &maximum_number_mass_redistribution_iterations, // number of iterations allowed to make
								// the volume of fluid field valid
								// these are the sweeps on the vof error
      double &time_step_mass_redistribution,		// time step for the mass redistribution
								// algorithm
      double &redistribution_vof_tolerance,			// threshold value of time-derivative 
      int &number_of_phases,					// number of phases in the domain:
								// 1: single phase flow
								// 2: two phase flow
      int &debugging_mode,					// =1, program is run in 
      								// debugging mode with additional output and checks
      								// =0, progam is run in production mode
      double &time_interval_for_reinitialization 		// time interval between reinitialization
								// of level-set field
 
	   )
{
      void set_parameters(					// set parameters for the computation
         int &number_primary_cells_i,		
         int &number_primary_cells_j,		
         int &number_primary_cells_k,		
         double &actual_time_step_level_set,	
         double &mesh_width_x1,			
         double &mesh_width_x2,			
         double &mesh_width_x3,			
         int &apply_mass_distribution_algorithm,    
         int &apply_mass_conservation_correction,   
         double &volume_of_fluid_tolerance,		
         double &lower_bound_derivatives,		
         int &number_vof_2_level_set_iterations,	
         int &number_iterations_ridder,		
         double &vof_2_level_set_tolerance,		
         double &cfl_number_reinitialization,	
         double &cfl_number_navier_stokes,		
         int &maximum_reinitialization_steps,	
         double &tolerance_reinitialization,	
      	  int &number_matrix_connections,			     
         vector &gravity,				
         double &actual_time_step_navier_stokes,	
         double &rho_plus_over_rho_minus,		
         double &smoothing_distance_factor,		
         double &rho_minus_over_mu_minus,		
         double &mu_plus_over_mu_minus,		
         double &tolerance_pressure,	  		
         int &maximum_iterations_allowed_pressure,	
         double &tolerance_velocity,	  		
         int &maximum_iterations_allowed_velocity,	
         int &continuous_surface_force_model,       
         int &source_terms_in_momentum_predictor,   
         double &sigma_over_rho_minus,		
         double &time_step_restriction_global,      
         int &fixed_time_step,			
         int &apply_curvature_smoothing,		
         int &number_curvature_smoothing_steps,	
         int &apply_curvature_smoothing_filter,	
         int &number_of_subcycles,			
         int &vtk_output,				
         int &tecplot_output,			
         double &time_interval_for_output,		
         double &domain_size_x1,			
         double &domain_size_x2,			
         double &domain_size_x3, 			
         geometry &flow_type,			 
         bubble *the_bubbles,			 
         int &number_of_bubbles,			 
         surface *the_free_surfaces,		 
         int &number_of_free_surfaces,
	 double &start_time_simulation,		
         double &end_time_simulation,
	 vector &initial_velocity,
	 restart_parameters &my_restart_parameters,
      	  int &maximum_number_mass_redistribution_iterations, 
         double &time_step_mass_redistribution,	
         double &redistribution_vof_tolerance, 
         int &number_of_phases,
	  int &debugging_mode,
	  double &time_interval_for_reinitialization
	   );

  void initialize_all_variables(				// initialize all variables
      double ***u_1_velocity_new, 			
      double ***u_2_velocity_new, 			
      double ***u_3_velocity_new,			
      double ***u_1_velocity_old, 			
      double ***u_2_velocity_old, 			
      double ***u_3_velocity_old,			
      double ***pressure,				
      double ***level_set_old,				
      double ***volume_of_fluid,	
      double ***curvature,
      double ***unsmoothed_curvature,
      double ***momentum_source_term_u_1,
      double ***momentum_source_term_u_2,
      double ***momentum_source_term_u_3,
      double ***surface_tension_body_force_x1,
      double ***surface_tension_body_force_x2,
      double ***surface_tension_body_force_x3,
      double ***scaled_density_u1,
      double ***scaled_density_u2,
      double ***scaled_density_u3,
      boundary_face boundary_faces[6],			
      geometry flow_type,				
      bubble *the_bubbles,				
      int number_of_bubbles,				
      surface *the_free_surfaces,			
      int number_of_free_surfaces, 			
      double lower_bound_derivatives,			
      double mesh_width_x1,				
      double mesh_width_x2,				
      double mesh_width_x3,				
      int number_primary_cells_i,			
      int number_primary_cells_j,			
      int number_primary_cells_k,
      vector initial_velocity,
      restart_parameters my_restart_parameters,
      double &start_time_simulation,
      int apply_curvature_smoothing, 
      int number_curvature_smoothing_steps,
      int apply_curvature_smoothing_filter,
      double smoothing_distance_factor,
      int debugging_mode,
      vector gravity,                                                              
      double tolerance,
      double actual_time_step_navier_stokes,
      double rho_plus_over_rho_minus,
      double sigma_over_rho_minus,
      int continuous_surface_force_model,
      int source_terms_in_momentum_predictor,           
      int maximum_iterations_allowed
                                                         
     );
    void allocate_main_variables(				// allocate memory for the main variables
      double *** & u_1_velocity_new, 			
      double *** & u_2_velocity_new, 			
      double *** & u_3_velocity_new,			
      double *** & u_1_velocity_old, 			
      double *** & u_2_velocity_old, 			
      double *** & u_3_velocity_old,			
      double *** & pressure,				
      double *** & level_set_old,				
      double *** & level_set_new,				
      double *** & volume_of_fluid,
      double *** & curvature,
      double *** & unsmoothed_curvature,
      double *** & momentum_source_term_u_1,
      double *** & momentum_source_term_u_2,
      double *** & momentum_source_term_u_3,
      double *** & surface_tension_body_force_x1,
      double *** & surface_tension_body_force_x2,
      double *** & surface_tension_body_force_x3,
      double *** & scaled_density_u1,
      double *** & scaled_density_u2,
      double *** & scaled_density_u3,
      int number_primary_cells_i,			
      int number_primary_cells_j,			
      int number_primary_cells_k			
	 );
    void set_boundary_conditions( 				// set the boundary conditions
	  boundary_face boundary_faces[6],
	  vector initial_velocity
	 );
    void output_solution(					// output the solution to file
	  double ***level_set_new, 		
	  double ***volume_of_fluid, 	
	  double ***curvature,
	  double ***smoothed_curvature,
	  double ***u_1_velocity_new, 		
	  double ***u_2_velocity_new, 		
	  double ***u_3_velocity_new,		
	  double ***pressure,			
	  int vtk_output,			
	  int tecplot_output,			
	  int number_primary_cells_i,		
	  int number_primary_cells_j,		
	  int number_primary_cells_k,		
	  double mesh_width_x1,			
	  double mesh_width_x2,			
	  double mesh_width_x3,			
	  int index_of_output_file
	    );

   
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/ 
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/ 
 
/*  compute all necessary parameters */

    set_parameters( number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,		
		      actual_time_step_level_set, mesh_width_x1, mesh_width_x2, mesh_width_x3,			
			apply_mass_distribution_algorithm, apply_mass_conservation_correction,   
			  volume_of_fluid_tolerance, lower_bound_derivatives,		
			    number_vof_2_level_set_iterations, number_iterations_ridder,		
			      vof_2_level_set_tolerance, cfl_number_reinitialization, cfl_number_navier_stokes,		
				maximum_reinitialization_steps, tolerance_reinitialization, number_matrix_connections,				       
				  gravity, actual_time_step_navier_stokes, rho_plus_over_rho_minus, smoothing_distance_factor,		
				    rho_minus_over_mu_minus, mu_plus_over_mu_minus, tolerance_pressure,	  		
				      maximum_iterations_allowed_pressure, tolerance_velocity,
					maximum_iterations_allowed_velocity, continuous_surface_force_model,       
					  source_terms_in_momentum_predictor, sigma_over_rho_minus,		
					    time_step_restriction_global,      
					      fixed_time_step, apply_curvature_smoothing, number_curvature_smoothing_steps,	
						apply_curvature_smoothing_filter,	number_of_subcycles, vtk_output, tecplot_output,			
						  time_interval_for_output, domain_size_x1, domain_size_x2, domain_size_x3,
						    flow_type, the_bubbles, number_of_bubbles, the_free_surfaces,		
						    number_of_free_surfaces, 
						  start_time_simulation, end_time_simulation, initial_velocity,
						  my_restart_parameters, maximum_number_mass_redistribution_iterations, 
          					time_step_mass_redistribution, redistribution_vof_tolerance, number_of_phases,
					       debugging_mode, time_interval_for_reinitialization
    		);

 

 /* set the boundary conditions */ 
 
   
     set_boundary_conditions(boundary_faces, initial_velocity); 
    

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/ 
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/ 
  
 /* allocate memory for all variables */
 

    allocate_main_variables( u_1_velocity_new, u_2_velocity_new, u_3_velocity_new,			
			      u_1_velocity_old, u_2_velocity_old, u_3_velocity_old,			
				pressure, level_set_old, level_set_new, volume_of_fluid,	
				  curvature, unsmoothed_curvature,
                                 momentum_source_term_u_1, momentum_source_term_u_2, momentum_source_term_u_3,
                                   surface_tension_body_force_x1, surface_tension_body_force_x2, surface_tension_body_force_x3,
                                     scaled_density_u1, scaled_density_u2, scaled_density_u3,
				  number_primary_cells_i, number_primary_cells_j, number_primary_cells_k);
    
 /* initialize all variables */
 
    initialize_all_variables( u_1_velocity_new, u_2_velocity_new, u_3_velocity_new,			
				u_1_velocity_old, u_2_velocity_old, u_3_velocity_old,			
				  pressure, level_set_old, volume_of_fluid, curvature, unsmoothed_curvature,
                                  momentum_source_term_u_1, momentum_source_term_u_2, momentum_source_term_u_3,
                                    surface_tension_body_force_x1, surface_tension_body_force_x2, surface_tension_body_force_x3,
                                      scaled_density_u1, scaled_density_u2, scaled_density_u3,
				          boundary_faces,			
				            flow_type, the_bubbles, number_of_bubbles, the_free_surfaces,			
				              number_of_free_surfaces, lower_bound_derivatives,			
					    mesh_width_x1, mesh_width_x2, mesh_width_x3,				
					  number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,
					initial_velocity, my_restart_parameters, start_time_simulation,
				  	apply_curvature_smoothing, number_curvature_smoothing_steps,
				  apply_curvature_smoothing_filter, smoothing_distance_factor, debugging_mode,
                              gravity, tolerance_pressure, actual_time_step_navier_stokes, rho_plus_over_rho_minus, sigma_over_rho_minus,
                            continuous_surface_force_model, source_terms_in_momentum_predictor, maximum_iterations_allowed_pressure);
     
/* write the initial conditions to file */

    output_solution( level_set_old, volume_of_fluid, curvature, unsmoothed_curvature,		
			  u_1_velocity_new, u_2_velocity_new, u_3_velocity_new, pressure,			
			    vtk_output, tecplot_output,			
			      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,		
				mesh_width_x1, mesh_width_x2, mesh_width_x3, 0);

}    
