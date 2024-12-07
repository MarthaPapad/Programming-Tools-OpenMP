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
	int i, j, threads;
	long long int points, circle_points;
	double x, y, pi, start_time, end_time;
	
	//declare points and max thread number
	points = (long long int)1e9;
	threads = omp_get_max_threads();
	
	//initialize results' lists
	double pi_values[threads], time[threads];
	
	//open txt files to write the results
	FILE *file_pi = fopen("pi_data.txt", "w");
    FILE *file_time = fopen("time_data.txt", "w");

    //check if both files were opened successfully
    if (file_pi == NULL || file_time == NULL) {
        printf("Error opening files!\n");
        return 1;
    }
	
	for (j=1; j<=threads; j++){
		//initialize circle points and number of threads
		circle_points = 0;
		omp_set_num_threads(j);
		
		//start timer
		start_time = omp_get_wtime();
		
		//parallel programming section 
		#pragma omp parallel for shared(points) private(i,x,y) reduction(+:circle_points) default(none)
		for (i=0; i<points; i++){
			//set seeds 
			unsigned int seed_x = omp_get_thread_num()+1;
			unsigned int seed_y = omp_get_thread_num()+2;
			
			//random coordinates
			x = (double)rand_r(&seed_x)/RAND_MAX;
			y = (double)rand_r(&seed_y)/RAND_MAX;
			
			//test if point is within circle
			if (sqrt(x*x+y*y)<1){
				circle_points += 1;
			}
		}
		
		//end timer
		end_time = omp_get_wtime();
		
		//calculate pi
		pi = 4*(double)circle_points/points;
		printf("The value of pi using a Monte-Carlo method for %lld points and %d threads is %.10f\n", points, j, pi);
		printf("Time of computation is %f seconds\n", end_time-start_time);
		
		//write pi value in pi file
		pi_values[j] = pi;
		fprintf(file_pi, "%.10f\n", pi_values[j]);
		
		//write time value in time file
		time[j] = end_time-start_time;
		fprintf(file_time, "%.10f\n", time[j]);
	}
	
	//close the files
	fclose(file_pi);
    fclose(file_time);
	
	return 0;
}
