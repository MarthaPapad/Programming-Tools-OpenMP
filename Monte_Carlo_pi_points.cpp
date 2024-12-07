// Programming Tools - OpenMP
// Papadopoulou Martha
// AEM: 4438

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <pthread.h>

int main(void){
	
	//initialize variables
	int i, j, size, threads;
	long long int circle_points;
	double x, y, pi;
	
	//initialize lists' size and lists
	size = 8;
	long long int points[size];
	double pi_values[size];
	
	//assign values to points list
	for (j=0; j< size; j++){
		points[j] = pow(10, 2+j);
	}
	
	//get max threads number
	threads = omp_get_max_threads();
	
	//open txt file to write results
	FILE *file_pi = fopen("pi_data_b.txt", "w");
	
	//check if file was opened successfully
    if (file_pi == NULL) {
        printf("Error opening files!\n");
        return 1;
    }
	
	for (j=0; j< size; j++){
		//initialize circle points
		circle_points = 0;
		
		//parallel programming section 
		#pragma omp parallel for shared(points,j) private(i,x,y) reduction(+:circle_points) default(none)
		for (i=0; i<points[j]; i++){
			//set seeds 
			unsigned int seed_x = omp_get_thread_num() + 1 + i;
			unsigned int seed_y = omp_get_thread_num() + 2 + i;
			
			//random coordinates
			x = (double)rand_r(&seed_x)/RAND_MAX;
			y = (double)rand_r(&seed_y)/RAND_MAX;
			
			//test if point is within circle
			if (sqrt(x*x+y*y)<1.0){
				circle_points += 1;
			}
		}
		
		//calculate pi
		pi = 4*(double)circle_points/points[j];
		printf("The value of pi using a Monte-Carlo method for %lld points and %d threads is %.10f\n", points[j], threads, pi);
		
		//write pi value in pi file
		pi_values[j] = pi;
		fprintf(file_pi, "%.10f\n", pi_values[j]);
		
	}
	
	//close file
	fclose(file_pi);
	
	return 0;
}
