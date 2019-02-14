/* Project: Knapsack Problem
/* Genetic Algorithm
/* File: knapsack.c
/* by:          _  __  
          )\/) / ` ) ) 
         (  ( (_. /_/         */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define   DEFAULT_N         5
#define   DEFAULT_CAPACITY  15
#define   POPULATION_SIZE   6
#define   MUTATION_PROB     0.1
#define   LOWEST_FITNESS    1

//Problem Models ----------------------------
typedef struct{
	int n;
	short *compartment;
	float capacity;
}Knapsack;

typedef struct{
	float benefit;
	float weight;
}Item;

typedef struct{
	int gene_size;
	short *gene;
	float weight;
	float fitness;
	float mating_probability;
}Individual;

typedef struct{
	int size;
	Individual *individual;
}Population;

//-------------------------------------------

//Functions Prototypes ----------------------
//Initializing Functions --------------------
void Init_Default_Knapsack(Knapsack *);
void Init_Default_Items(Item *, Knapsack);
void Init_Population(Population *, Item *, Knapsack);

//Problem Solving Functions -----------------
int Random_Number(int, int);
bool Small_Random_Probability();
float Weight(Individual, Item *);
float Fitness(Individual, Item *, Knapsack );
void Assign_Mating_Probability(Population *);
Individual Random_Selection(Population);
bool Individuals_Are_Equal(Individual, Individual);
Individual Reproduce(Individual, Individual);
void Mutate(Individual *);
void Clear_Population(Population *);
Individual Genetic_Algorithm(Population, Item *, Knapsack);
void Fill_Knapsack(Knapsack *, Individual);

//Menu Functions ----------------------------


//Printing Functions ------------------------
void Print_Individual(Individual);
void Print_Population(Population);

//Control Functions -------------------------
void Control(Knapsack *, Item **);

//-------------------------------------------

//Test --------------------------------------
int main(int argc, char *argv[]){
	srand(time(NULL));

	Knapsack knapsack;
	Item *item;

	Control(&knapsack, &item);

	return 0;
}

//-------------------------------------------

//Functions Implementation ------------------
//Initializing Functions --------------------
void Init_Default_Knapsack(Knapsack *knapsack){
	knapsack->n = DEFAULT_N;
	knapsack->compartment = (short *) malloc(sizeof(short) * knapsack->n);
	knapsack->capacity = DEFAULT_CAPACITY;
}

void Init_Default_Items(Item *item, Knapsack knapsack){
	item[0].weight = 12;
	item[0].benefit = 4;

	item[1].weight = 2;
	item[1].benefit = 2;
	
	item[2].weight = 1;
	item[2].benefit = 1;
	
	item[3].weight = 4;
	item[3].benefit = 10;
	
	item[4].weight = 1;
	item[4].benefit = 2;	
}

void Init_Population(Population *population, Item *item, Knapsack knapsack){
	population->size = POPULATION_SIZE;
	population->individual = (Individual *) malloc(sizeof(Individual) * population->size);

	int gene_size = knapsack.n;
	int i, j;

	for(i = 0 ; i < population->size ; i++){
		population->individual[i].gene_size = gene_size;
		population->individual[i].gene = (short *) malloc(sizeof(short) * gene_size);
		population->individual[i].weight = 0;

		for(j = 0 ; j < gene_size ; j++)
			if((population->individual[i].gene[j] = Random_Number(0, 1)) == 1)
				population->individual[i].weight += item[j].weight;

		population->individual[i].fitness = Fitness(population->individual[i], item, knapsack);
	}	

	Assign_Mating_Probability(population);
}

//Problem Solving Functions -----------------
int Random_Number(int min, int max){
    return min + (rand() % (max - min + 1));
}

bool Small_Random_Probability(){
	float random_number = Random_Number(0, 100);

	random_number /= 100;

	if(random_number <= MUTATION_PROB)
		return true;

	return false;
}

float Weight(Individual individual, Item *item){
	float weight = 0;
	int gene_size = individual.gene_size;
	int i;

	for(i = 0 ; i < gene_size ; i++)
		if(individual.gene[i] == 1)
			weight += item[i].weight;

	return weight;
}

float Fitness(Individual individual, Item *item, Knapsack knapsack){
	int i;
	float weight = 0;
	float fitness = 0;

	for(i = 0 ; i < individual.gene_size ; i++)
		if(individual.gene[i] == 1){
			weight += item[i].weight;
			fitness += item[i].benefit;
		}

	if(weight > knapsack.capacity)
		return LOWEST_FITNESS;

	return fitness;
}

void Assign_Mating_Probability(Population *population){
	float fitness_sum = 0;
	int i;

	for(i = 0 ; i < population->size ; i++)
		fitness_sum += population->individual[i].fitness;

	for(i = 0 ; i < population->size ; i++)
		population->individual[i].mating_probability = population->individual[i].fitness / fitness_sum;
}

Individual Random_Selection(Population population){
	float random_number = Random_Number(0, 100);
	float aux = 0;
	int i;

	random_number /= 100;

	for(i = 0 ; i < population.size ; i++){
		aux += population.individual[i].mating_probability;
	
		if(random_number <= aux)
			return population.individual[i];
	}

	return population.individual[i - 1];
	// printf("\nRandom_Selection is not working properly!\n");
}

bool Individuals_Are_Equal(Individual x, Individual y){
	int i;

	for(i = 0 ; i < x.gene_size ; i++)
		if(x.gene[i] != y.gene[i])
			return false;

	return true;
}

Individual Reproduce(Individual x, Individual y){
	Individual child;
	child.gene_size = x.gene_size;
	child.gene = (short *) malloc(sizeof(short) * child.gene_size);
	child.weight = 0;

	int gene_size = x.gene_size;
	int c = Random_Number(0, gene_size - 2);
	int i;

	for(i = 0 ; i <= c ; i++)
		child.gene[i] = x.gene[i];

	for(i = c + 1 ; i < gene_size ; i++)
		child.gene[i] = y.gene[i];

	return child;
}

void Mutate(Individual *child){
	int c = Random_Number(0, child->gene_size - 1);

	if(child->gene[c] == 0) child->gene[c] = 1;

	else child->gene[c] = 0;

	// child->gene[c] = (child->gene[c] == 0) ? 1 : 0;
}

void Clear_Population(Population *population){
	int i, j;
	int gene_size = population->individual[0].gene_size;

	for(i = 0 ; i < population->size ; i++){
		population->individual[i].weight = 0;
		population->individual[i].fitness = 0;
		population->individual[i].mating_probability = 0;
	}
}

void Mating_Probability_Sum(Population population){
	float sum = 0;
	int i;

	for(i = 0 ; i < population.size ; i++)
		sum += population.individual[i].mating_probability;

	printf("Mating Probability Sum: %.2f\n", sum);
	system("PAUSE");
}

Individual Genetic_Algorithm(Population population, Item *item, Knapsack knapsack){
	printf("FOI!\n");
	Population new_population;
	new_population.individual = (Individual *) malloc(sizeof(Individual) * population.size);
	new_population.size = population.size;

	int iteration = 1;
	int i, j;

	Individual x, y;
	Individual child;
	Individual best_individual = population.individual[0];

	printf("Best Individual\n");
	Print_Individual(best_individual);
	system("PAUSE");

	while(iteration <= 10000){
		//printf("iteration: %d\n", iteration);
		// printf("Population\n");
		// Print_Population(population);
		// system("PAUSE");
		Clear_Population(&new_population);

		for(i = 0 ; i < population.size ; i++){
			x = Random_Selection(population);
			y = Random_Selection(population);

			// for(j = 1 ; Individuals_Are_Equal(x, y), j <= 500 ; j++)
			// 	y = Random_Selection(population);

			j = 1;
			while(Individuals_Are_Equal(x, y) && j <= 100){
				y = Random_Selection(population);
				j++;
			}

			child = Reproduce(x, y);

			if(Small_Random_Probability())
				Mutate(&child);

			child.weight = Weight(child, item);
			child.fitness = Fitness(child, item, knapsack);

			if(child.fitness > best_individual.fitness){
				best_individual = child;

				printf("Best Individual\n");
				Print_Individual(best_individual);
				system("PAUSE");
			}

			new_population.individual[i] = child;
		}

		Assign_Mating_Probability(&new_population);
		population = new_population;

		iteration++;
	}

	free(new_population.individual);

	return best_individual;
}

void Fill_Knapsack(Knapsack *knapsack, Individual best_individual){
	int i;

	for(i = 0 ; i < knapsack->n ; i++)
		knapsack->compartment[i] = best_individual.gene[i];
}

//Menu Functions ----------------------------


//Printing Functions ------------------------
void Print_Individual(Individual individual){
	int i;

	printf("Weight: %.2f\n", individual.weight);
	printf("Fitness:%.2f\n", individual.fitness);
	printf("Gene: ");

	for(i = 0 ; i < individual.gene_size ; i++)
		printf("%hi", individual.gene[i]);

	printf("\n\n");

	//printf("\nMating Probability: %.2f\n\n", individual.mating_probability);
}

void Print_Population(Population population){
	int i, j;

	for(i = 0 ; i < population.size ; i++){
		printf("Individual %d\n", i + 1);
		Print_Individual(population.individual[i]);
	}
}

//Control Functions -------------------------
void Control(Knapsack *knapsack, Item **item){
	system("clear || cls");
	Init_Default_Knapsack(knapsack);

	*item = (Item *) malloc(sizeof(Item) * knapsack->n);
	Init_Default_Items(*item, *knapsack);

	Population population;
	Init_Population(&population, *item, *knapsack);

	Individual best_individual = Genetic_Algorithm(population, *item, *knapsack);

	printf("\nBest Individual\n");
	Print_Individual(best_individual);
}

//-------------------------------------------