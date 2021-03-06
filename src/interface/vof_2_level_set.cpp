#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>
/********************************************************************************/
/********************************************************************************/
/*  Function to compute the level-set value from the volume of fluid value 	*/
/*  method. 									*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* In Sander's thesis two functions are described:                              */
/* f : this computes the volume of field from the level-set field               */
/* g : this computes the level-set field from the volume of fluid field         */
/* The function g is not given in closed form. The formulation of algorithm     */
/* to find the value of g is now changed to Ridder's algorithm, which gives     */
/* a rate of convergence of sqrt(2), and is Jacobian free and very simple.      */
/* The purpose is to find a root of the nonlinear equation                      */
/* g(phi)=f(phi)-psi=0								*/
/* where f(phi)=vof(phi)							*/
/* Initially we need to provide to values of phi, phi_left and phi_right	*/
/* such that g(phi_left)*g(phi_right)<0	(function values have opposite sign).	*/
/* Later on, we could change it back to the original Newton algorithm or a      */
/* Brent type algorithm which gives second order convergence.                   */
/********************************************************************************/

EXPORT int vof_2_level_set( 
	      double &level_set, 			// level-set field value in this cell
	      double d_level_set_d_x1, 			// first partial derivative in x1 
							// direction of level-set
	      double d_level_set_d_x2, 			// first partial derivative in x2 
							// direction of level-set 
	      double d_level_set_d_x3, 			// first partial derivative in x3 
							// direction of level-set
	      double volume_of_fluid,			// volume of fluid value in this cell
	      double lower_bound_derivatives,    	// lower bound for the first partial derivatives
							// to consider it a limiting case of vanishing
							// partial derivatives
	      double vof_2_level_set_tolerance,		// tolerance in the conversion from volume
							// of fluid value to level-set value
	      int maximum_number_iterations_ridder	// maximum number of iterations allowed in the
							// nonlinear root finding algorithm
	
      )
      {
      double level_set_left;			// left hand level_set value for nonlinear root finding
      double level_set_right;			// right hand level_set value for nonlinear root finding
      double volume_of_fluid_left;		// left hand volume of fluid value for nonlinear root finding
      double volume_of_fluid_right;		// right hand volume of fluid value for nonlinear root finding
      double function_g_left;
      double function_g_right;
 
      /* find two values of the linearized level-set field, such that the function */
      /* g(phi)=f(phi)-psi has opposite sign					   */
      
      level_set_left= -0.5*(fabs(d_level_set_d_x1)+
				fabs(d_level_set_d_x2)+
				  fabs(d_level_set_d_x3));
      level_set_right=-1.0*level_set_left;
      
      
      if(level_set_2_vof(level_set_left, 
			    d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3, 
			      volume_of_fluid_left, lower_bound_derivatives))
      {
	std::cout<< "error in computation of volume of fluid in vof_2_level_set \n";
	std::cout<< "Left state \n";
	std::cout<< " in function vof_2_level_set line 99 \n";
	return 0;
      }
      
      if(level_set_2_vof(level_set_right, 
			    d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3, 
			      volume_of_fluid_right, lower_bound_derivatives))
      {
	std::cout<< "error in computation of volume of fluid in vof_2_level_set \n";
	std::cout<< "Right state \n";
	std::cout<< " in function vof_2_level_set line 109 \n";
	return 1;
      }
      
//       if(volume_of_fluid<0.000001) volume_of_fluid=0.000001;
//       if(volume_of_fluid>0.999999) volume_of_fluid=0.999999;
//       if(volume_of_fluid_left<0.000001) volume_of_fluid_left=0.000001;
//       if(volume_of_fluid_left>0.999999) volume_of_fluid_left=0.999999;
//       if(volume_of_fluid_right<0.000001) volume_of_fluid_right=0.000001;
//       if(volume_of_fluid_right>0.999999) volume_of_fluid_right=0.999999;
      
      function_g_left  = volume_of_fluid_left-volume_of_fluid;
      function_g_right = volume_of_fluid_right-volume_of_fluid;
      
      if(function_g_left*function_g_right>0)
      {
	std::cout<< "error in providing two volume of fluid values of opposite sign \n";
	std::cout<< "to start nonlinear root finding, aborting \n";
	std::cout<< "volume_of_fluid "<< volume_of_fluid << " \n";
	std::cout<< "volume_of_fluid_left "<< volume_of_fluid_left<< " \n";
	std::cout<< "volume_of_fluid_right "<<volume_of_fluid_right << " \n";
	std::cout<< "level_set "<< level_set<< " \n";
	std::cout<< "level_set_left "<< level_set_left<< " \n";
	std::cout<< "level_set_right "<<level_set_right << " \n";
	std::cout<< "function_g_left  "<< function_g_left << " \n";
	std::cout<< "function_g_right "<< function_g_right << " \n";
	std::cout<< "in function vof_2_level_set line 120 \n";
	return 1;
      }
      
      /* use Ridders method to find the root of the equation g(phi) */

//       if(ridders_method(level_set_left, level_set_right, 
// 		      function_g_left, function_g_right,
// 			volume_of_fluid, level_set,
// 			  d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3, 
// 			    vof_2_level_set_tolerance,
// 			      maximum_number_iterations_ridder,	
// 				lower_bound_derivatives	))
//       {
// 	std::cout<< "Nonlinear root finding algorithm failed in vof_2_level_set \n";
// 	std::cout<< " in function vof_2_level_set line 134 \n";
// 	std::cout<< "Aborting \n";
// 	exit(1);
// 	
//       }
      
      if(bisection_method(level_set_left, level_set_right, 
		      function_g_left, function_g_right,
			volume_of_fluid, level_set,
			  d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3, 
			    vof_2_level_set_tolerance,
			      maximum_number_iterations_ridder,	
				lower_bound_derivatives	))
      {
	std::cout<< "Nonlinear root finding algorithm failed in vof_2_level_set \n";
	std::cout<< " in function vof_2_level_set line 134 \n";
	std::cout<< "Aborting \n";
	return 1;
	
      }
      
      return 0;

}     
