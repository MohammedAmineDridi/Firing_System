#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 100
#define MAX_BUFFER_ARGS 5
#define PRINT_PARAMS
#define PRINT_RESULT
#define PRINT_FINAL_RESULT
#define CMD_PYTHON_SIMULATION
#define MANIVELL_ANGLE

double radiansToDegrees(double radians) 
{
    return radians * (180.0 / M_PI);
}

void main()
{
	char init_height_buffer[MAX_BUFFER_ARGS] , init_vi_buffer[MAX_BUFFER_ARGS] , init_angle_buffer[MAX_BUFFER_ARGS];
	char Full_args_Buffer[MAX]; 
	char script_path[MAX] = "python projectile_automated_simulation.py";
	char final_sentence_script[MAX]; 
	char system_bielle_manivelle_request;
	char prog_bielle_manivelle_cmd[MAX];
	char args[MAX_BUFFER_ARGS];

	// params.
	float g = 9.81; // gravity acceleration force
	float k,L0,a,b,m,delta_x,delta_y;
	
    printf("Enter spring constant (N.m) :");
    scanf("%f", &k);
    printf("Enter L0 (cm) :");
    scanf("%f", &L0);
    printf("Enter a (cm) :");
    scanf("%f", &a);
    printf("Enter b (cm) :");
    scanf("%f", &b);
    printf("Enter m (kg) :");
    scanf("%f", &m);
	printf("Enter delta_x (m) :");
    scanf("%f", &delta_x);
	printf("Enter delta_y (m) :");
    scanf("%f", &delta_y);
	printf("----------------------- \n");
	
	// calcul params .
	float l0 = sqrt( (pow(L0,2)) - (pow((b/2),2)) );  // slinghsot bullet idle state distance . 
	float l1 = delta_x; // exmple : every 1m in DX -> l1+= 1cm /  l1 (elongation center distance) prop with distance between src & target .
	delta_x += (l1/100); // update the new DX (after slinghsot elongation) ( l1/100 : bcz 'l1' in cm & DX in 'm' )
	float L1 = sqrt( (pow((l0+l1),2)) + pow((b/2),2) ); // elongation distance of 2 threads
	float alpha = atan((b/2)/(l0+l1));
	float p = 2*k*L1*cos(alpha); // the force needed to elongate the slinghsot threads to 'l1' distance .
	float vi = sqrt( ((float)k/m) * pow((L1/100),2) );  // L1/100 : from (cm) to (m) , bcz : vi(m/s)
	float angle;
	if ( delta_y != 0 ) {
		angle = radiansToDegrees( ( ( acos( (( (g*pow(delta_x,2))/(pow(vi,2)) ) - delta_y) / (sqrt( pow(delta_y,2) + pow(delta_x,2) )) )/2 ) + (atan( (delta_x/delta_y) )) )/2 ) ;	
	} else {
		angle = radiansToDegrees( asin( (2*g*(delta_x/2))/pow(vi,2) ) / 2 );
	}
	
	#ifdef PRINT_PARAMS 
		printf("a = %f (cm) \n" ,a);
		printf("b = %f (cm) \n" ,b);
		printf("L0 = %f (cm) \n" ,L0);
		printf("m = %f (kg) \n" ,m);
		printf("k = %f (Nm) \n" ,k);
		printf("g = %f (m/s^2) \n" ,g);
		printf("DX = %f (m) \n" ,delta_x);
		printf("DY = %f (m) \n" ,delta_y);
		printf("-------------------------------- \n");
	#endif
	
	#ifdef PRINT_RESULT
		printf("l0 = %f (cm) \n" ,l0);
		printf("l1 = %f (cm) \n" ,l1);
		printf("new Dx = %f (m) \n" ,delta_x);
		printf("L1 = %f (cm) \n" ,L1);
		printf("P = %f (N) \n" ,p);
		printf("Vi = %f (m/s) \n" ,vi);
		printf("angle = %f \n",angle);
		printf("-------------------------------- \n");
		printf("\n");
	#endif
	
	#ifdef PRINT_FINAL_RESULT
		printf("************************************************ \n");
		printf("to reach distance of DX = %f (m) and DY = %f (m) ,  you have to put : Vi = %f (m/s) & Angle = %f deg \n" , delta_x,delta_y,vi,angle);
		printf("************************************************ \n");
	#endif	

	#ifdef CMD_PYTHON_SIMULATION
		sleep(7); // sleep 7s before executing the first python prog .
		gcvt(delta_y,MAX_BUFFER_ARGS, init_height_buffer);
		gcvt(vi,MAX_BUFFER_ARGS, init_vi_buffer);
		gcvt(angle,MAX_BUFFER_ARGS, init_angle_buffer);
		sprintf(Full_args_Buffer, "%s %s %s", init_height_buffer, init_vi_buffer , init_angle_buffer ); 
		sprintf(final_sentence_script,"%s %s",script_path,Full_args_Buffer);
		system(final_sentence_script);
	#endif

	#ifdef MANIVELL_ANGLE
		sleep(20); // sleep 20s before entering to the second prog .
		gcvt(l1,MAX_BUFFER_ARGS,args);
		sprintf(prog_bielle_manivelle_cmd,"%s %s","start cmd /k bielle_manivelle.exe",args);
		system(prog_bielle_manivelle_cmd);
	#endif
}



