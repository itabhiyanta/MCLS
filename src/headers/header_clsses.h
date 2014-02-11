/*%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*         classes 	    */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class coordinate
{
public:
  double x1,x2,x3;
  coordinate(double xx1=0, double xx2=0, double xx3=0){x1=xx1;x2=xx2;x3=xx3;}
};

class vector
{
public:
  double u1,u2,u3;
  vector(double u1, double u2, double u3);
  vector( void);
};
vector::vector(double u1_default=0, double u2_default=0.0, double u3_default=0.0)
{
  u1=u1_default;
  u2=u2_default;
  u3=u3_default;
}
vector::vector(void)
{
  u1=0.0;
  u2=0.0;
  u3=0.0;
}
const coordinate default_coordinate;

class bubble
{
public:
  double principle_axis_x1;
  double principle_axis_x2;
  double principle_axis_x3;
  int label;
  coordinate center_location;
  bubble(int number, coordinate bubble_center, double principle_axis_x1, double principle_axis_x2, double principle_axis_x3);
};
bubble::bubble(int number=1, coordinate bubble_center=default_coordinate, 
		double princp_axis_x1=0, double princp_axis_x2=0 , double princp_axis_x3=0)
{
  label=number;
  center_location=bubble_center;
  principle_axis_x1=princp_axis_x1;
  principle_axis_x2=princp_axis_x2;
  principle_axis_x3=princp_axis_x3;
};

enum variable{velocity_u1, velocity_u2, velocity_u3, level_set, pressure_field};
enum boundary_conditions_type{dirichlet, neumann, periodic};
enum boundary_conditions_rule{constant, function};
enum cell_centerings{cell_centered, vertex_centered};
enum geometry{bubbly_flow, wavy_flow};

class surface
{
public:
  int active;
  int orientation;
  double height;
};

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
boundary_variable::boundary_variable(variable varname=velocity_u1,
				     boundary_conditions_type bound_type=neumann,
				     boundary_conditions_rule bound_rule=constant,
				     cell_centerings  cell_cent=cell_centered,
					double bound_value =0.0)
{
variable_name=varname;
boundary_condition_type=bound_type;
boundary_condition_rule=bound_rule;
cell_centering=cell_cent;
boundary_condition_value=bound_value;
};
boundary_variable::boundary_variable(variable varname)
{
variable_name=varname;
boundary_condition_type=neumann;
boundary_condition_rule=constant;
cell_centering=cell_centered;
boundary_condition_value=0.0;
};

class boundary_face
{
public:
    boundary_variable boundary_variables[5];
    boundary_face(void);
   
};
boundary_face::boundary_face(void)
{
    boundary_variables[0].variable_name=velocity_u1;
    boundary_variables[1].variable_name=velocity_u2;
    boundary_variables[2].variable_name=velocity_u3;
    boundary_variables[3].variable_name=level_set;
    boundary_variables[4].variable_name=pressure_field;

    
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
restart_parameters::restart_parameters(void)
{
      start_from_restart_file=0;
      write_solution_to_restart_file=1;
      name_restart_file_to_read="restart_file_mcls";
      name_restart_file_to_write="restart_file_mcls";
};     