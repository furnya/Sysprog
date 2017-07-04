#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

int num_pr;
int num_bm;
int num_op;
int** total_demands;
int** current_occupied;
int* available_bm;
int** operations;
int** remaining_demands;
int* current_available;
int** remaining_demands2;
int* current_available2;
FILE* f_in;
FILE* f_out;

void read_file(){
	int line_count = 0;
	char buf[100];
	while(fgets(buf,sizeof(buf),f_in) != NULL){
	  line_count++;
	}
	num_op = line_count-11;
	rewind(f_in);
	fscanf(f_in, "%d", &num_pr);
	fscanf(f_in, "%d", &num_bm);
	total_demands = (int**) malloc(num_pr * sizeof(int*));
	current_occupied = (int**) malloc(num_pr * sizeof(int*));
	available_bm = (int*) malloc(num_bm * sizeof(int));
	operations = (int**) malloc(num_op * sizeof(int*));
	for(int i=0;i<num_pr;i++){
		total_demands[i] = (int*) malloc(num_bm * sizeof(int));
		current_occupied[i] = (int*) malloc(num_bm * sizeof(int));
	}
	fscanf(f_in,"\n");
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			fscanf(f_in,"%d", &total_demands[i][j]);
		}
	}
	fscanf(f_in,"\n");
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			fscanf(f_in,"%d", &current_occupied[i][j]);
		}
	}
	fscanf(f_in,"\n");
	for(int j=0;j<num_bm;j++){
		fscanf(f_in,"%d", &available_bm[j]);
	}
	fscanf(f_in,"\n");
	for(int i=0;i<num_op;i++){
		operations[i] = (int*) malloc(4 * sizeof(int));
		fgets(buf,sizeof(buf),f_in);
		if(buf[0]=='A'){
			operations[i][0] = 1;
			sscanf(buf,"A %d %d %d",&operations[i][1],&operations[i][2],&operations[i][3]);
		}else{
			operations[i][0] = 0;
			sscanf(buf,"R %d %d %d",&operations[i][1],&operations[i][2],&operations[i][3]);
		}
	}
}

void print_part1(){
	fprintf(f_out, "Prozesse:  %d / Betriebsmittel:  %d\n\n",num_pr,num_bm);
	fprintf(f_out, "Gesamtanforderungen:\n");
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			fprintf(f_out, " %d ", total_demands[i][j]);
		}
		fprintf(f_out,"\n");
	}
	fprintf(f_out, "\nBelegungen:\n");
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			fprintf(f_out, " %d ", current_occupied[i][j]);
		}
		fprintf(f_out,"\n");
	}
	fprintf(f_out, "\nverfügbar: ");
	for(int j=0;j<num_bm;j++){
		fprintf(f_out, " %d ", available_bm[j]);
	}
	fprintf(f_out,"\n");
}

void calculate_rest(){
	remaining_demands = (int**) malloc(num_pr * sizeof(int*));
	remaining_demands2 = (int**) malloc(num_pr * sizeof(int*));
	for(int i=0;i<num_pr;i++){
		remaining_demands[i] = (int*) malloc(num_bm * sizeof(int));
		remaining_demands2[i] = (int*) malloc(num_bm * sizeof(int));
	}
	current_available = (int*) malloc(num_bm * sizeof(int));
	current_available2 = (int*) malloc(num_bm * sizeof(int));
	for(int j=0;j<num_bm;j++){
		current_available[j] = available_bm[j];
	}
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			remaining_demands[i][j] = total_demands[i][j] - current_occupied[i][j];
			remaining_demands2[i][j] = remaining_demands[i][j];
			current_available[j] -= current_occupied[i][j];
			current_available2[j] = current_available[j];
		}
	}
}

void copy_matrixes(){
	for(int j=0;j<num_bm;j++){
		current_available2[j] = current_available[j];
	}
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			remaining_demands2[i][j] = remaining_demands[i][j];
		}
	}
}

void print_rest(){
	fprintf(f_out, "\nRestanforderungen:\n");
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			fprintf(f_out, " %d ", remaining_demands[i][j]);
		}
		fprintf(f_out,"\n");
	}
	fprintf(f_out, "\nfrei: ");
	for(int j=0;j<num_bm;j++){
		fprintf(f_out, " %d ", current_available[j]);
	}
}

int find_process(){
	for(int i=0;i<num_pr;i++){
		for(int j=0;j<num_bm;j++){
			if(remaining_demands2[i][j] > current_available2[j]){
				break;
			}
			if(j==num_bm-1){
				return i;
			}
		}
	}
	return -1;
}

bool calculate_banker(){
	int process;
	for(int i=0;i<num_pr;i++){
		process = find_process();
		if(process==-1){
			return false;
		}
		for(int j=0;j<num_bm;j++){
			current_available2[j] += current_occupied[process][j];
			remaining_demands2[process][j] = INT_MAX;
		}
	}
	return true;
}

void deadlock_avoidance(){
	int* op;
	for(int o=0;o<num_op;o++){
		op = operations[o];
		copy_matrixes();
		if(op[0]==0){
			if(op[3] <= current_occupied[op[1]][op[2]]){
				current_occupied[op[1]][op[2]] -= op[3];
				current_available[op[2]] += op[3];
			}else{
				continue;
			}
		}else{
			if(op[3] > current_available[op[2]] || op[3] > remaining_demands[op[1]][op[2]]){
				continue;
			}
			current_available[op[2]] -= op[3];
			current_occupied[op[1]][op[2]] += op[3];
			remaining_demands[op[1]][op[2]] -= op[3];
			if(!calculate_banker()){
				current_available[op[2]] += op[3];
				current_occupied[op[1]][op[2]] -= op[3];
				remaining_demands[op[1]][op[2]] += op[3];
				continue;
			}
		}
		fprintf(f_out, "\nOperation:");
		if(op[0]==0){
			fprintf(f_out, " R ");
		}else{
			fprintf(f_out, " A ");
		}
		fprintf(f_out, "%d %d %d\n",op[1],op[2],op[3]);
		print_rest();
		fprintf(f_out, "\n");
	}
}

int main(int argc, char const *argv[])
{
	f_in = fopen(argv[1],"r");
	read_file();
	fclose(f_in);
	f_out = fopen(argv[2],"w");
	print_part1();
	calculate_rest();
	print_rest();
	if(calculate_banker()){
		fprintf(f_out, "\n\nSICHER");
	}else{
		fprintf(f_out, "\n\nUNSICHER");
	}
	deadlock_avoidance();
	fclose(f_out);
	return 0;
}