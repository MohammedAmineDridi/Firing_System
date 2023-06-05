#include<stdio.h>
#include<math.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 100
#define MAX_BUFFER_ARGS 5
#define VERIF_SIMULATOR
#define PRINT_RESULT

double radiansToDegrees(double radians) {
    return radians * (180.0 / M_PI);
}

void Search_Rotation_Manivelle_Angle(float r ,float l ,float x)
{
	float alpha_angle;
	printf("PARAMS : r = %f / l = %f / x = %f \n",r,l,x);
	float a = pow(r,2)/(2*l);
	float b = r;
	float c = x-r-( (pow(r,2)) / (2*l) );
	printf("a = %f / b = %f / c = %f \n",a,b,c);
	float delta = pow(b,2)-(4*a*c);
	printf("Delta = %f \n",delta);  // Delta > 0 
	
	if (delta < 0){
		printf("pas de solution \n");
	} else if (delta == 0){
		// unique solution 
		float x1 = -b/(2*a);
		printf("unique solution = %f \n",x1);
	} else {
		// Delta > 0 : 2 solutions .
		float x1 = (-b-sqrt(delta))/(2*a);
		float x2 = (-b+sqrt(delta))/(2*a);
		if (x1 >= -1 && x1 <= 1) {
			printf("solution is x1 : %f \n",x1);
			alpha_angle = radiansToDegrees(acos(x1));
			printf("angle alpha = %f \n",alpha_angle );
		} else if (x2 >= -1 && x2 <= 1) {
			printf("solution is x2 : %f \n",x2);
			alpha_angle = radiansToDegrees(acos(x2));
			printf("angle alpha = %f \n",alpha_angle );
		}	
	}
	
	#ifdef PRINT_RESULT
		printf("\n");
		printf("************************************************ \n");
		printf("to reach distance of X = %f (mm) , you have to put : Angle = %f deg \n",x,alpha_angle);
		printf("************************************************ \n");
	#endif
}

void main(int argc, char *argv[])
{
	float r,l,n;
	printf("Enter raduis of manivelle (mm) :");
    scanf("%f", &r);
    printf("Enter length of bielle (mm) :");
    scanf("%f", &l);
    printf("Enter rotation velocity of the manivelle (tr/min) :");
    scanf("%f", &n);
	char init_R_buffer[MAX_BUFFER_ARGS] , init_L_buffer[MAX_BUFFER_ARGS] , init_N_buffer[MAX_BUFFER_ARGS];
	char Full_args_Buffer[MAX]; 
	char script_path[MAX] = "python bielle_manivelle_automated_simulation.py";
	char final_sentence_script[MAX]; 
	// l (elongation distance is an argument from the first prog) .
	Search_Rotation_Manivelle_Angle(r,l,atof(argv[1])/10 ); // x = l1 = 144.1/10 = 14.41 (elongation distance) (cm->mm)
	
	#ifdef VERIF_SIMULATOR
		snprintf(init_R_buffer,sizeof(init_R_buffer),"%d",(int)r);
		snprintf(init_L_buffer,sizeof(init_L_buffer),"%d",(int)l);
		snprintf(init_N_buffer,sizeof(init_N_buffer),"%d",(int)n);
		sprintf(Full_args_Buffer, "%s %s %s",init_R_buffer,init_L_buffer,init_N_buffer); 
		sprintf(final_sentence_script,"%s %s",script_path,Full_args_Buffer);
		system(final_sentence_script);
	#endif	
}
