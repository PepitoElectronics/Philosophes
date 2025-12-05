#include "SolutionEtudiant.hpp"
#include "Header_Prof.h"

// pointeur sur pthread qui devra contenir l adresse mémoire du tid du thread timer
pthread_t *timerThread;

// ptr pour stockages id
int *threadIds;

// tableau de pointeurs sur sem_t --> contient des pointeurs vers les espaces mémoire des sémaphores
sem_t **semAutorisation;

// tableau de pointeurs sur sem_t --> contient des pointeurs vers les espaces mémoire des sémaphores
sem_t **semFinDeService;

// tableau  de séma pour synchronisation au démarrage
sem_t **semProgrammeReady;

pthread_mutex_t mutexCompteur;
pthread_mutex_t mutexCsv;
/***********variables programme**********************/
// début d'exécution
bool start_cond = true;
// conteur de cycles
int eat_counter = 0;
int group = 0;
int scenario = 0;

// timerThread flag
bool timerRunning = false;
bool sem_posted = false;
bool next_group_ready = false;

int nbPhilosEating;

double total_execution_time[NB_PHILOSOPHES] = {0.0}; // Initialisation à 0
int execution_count[NB_PHILOSOPHES] = {0}; // Initialisation à 0
double average_execution_time[NB_PHILOSOPHES] = {0.0}; // Initialisation à 0

thread_local double elapsed_time;

#ifdef SOLUTION_1

void fonctionOrdonnancer()
{
	if (etatsPhilosophes[0] == 1 && etatsPhilosophes[2] == 1 && etatsPhilosophes[3] == 1 && etatsPhilosophes[1] == 1)
	{
		// gr 1 eat
		actualiserEtAfficherEtatsPhilosophes(0, 2);
		actualiserEtAfficherEtatsPhilosophes(2, 2);
		// actualiserEtAfficherEtatsPhilosophes(4,2);
	}
	else if (etatsPhilosophes[0] == 3 && etatsPhilosophes[2] == 3 && etatsPhilosophes[1] == 1 && etatsPhilosophes[3] == 1)
	{
		// gr2 eat
		actualiserEtAfficherEtatsPhilosophes(1, 2);
		actualiserEtAfficherEtatsPhilosophes(3, 2);
		// gr1 think
		actualiserEtAfficherEtatsPhilosophes(0, 4);
		actualiserEtAfficherEtatsPhilosophes(2, 4);
	}
	else if (etatsPhilosophes[3] == 3 && etatsPhilosophes[1] == 3 && etatsPhilosophes[4] == 1)
	{
		// gr3 eat
		actualiserEtAfficherEtatsPhilosophes(4, 2);
		// gr2 think
		actualiserEtAfficherEtatsPhilosophes(1, 4);
		actualiserEtAfficherEtatsPhilosophes(3, 4);
	}

	else if (etatsPhilosophes[4] == 3 && etatsPhilosophes[0] == 5 && etatsPhilosophes[2] == 5 && etatsPhilosophes[1] == 5 && etatsPhilosophes[3] == 5)
	{
		// gr1 and gr 2 hungry
		actualiserEtAfficherEtatsPhilosophes(0, 0);
		actualiserEtAfficherEtatsPhilosophes(2, 0);
		actualiserEtAfficherEtatsPhilosophes(1, 0);
		actualiserEtAfficherEtatsPhilosophes(3, 0);
		// gr 3 think
		actualiserEtAfficherEtatsPhilosophes(4, 4);
	}
	if (etatsPhilosophes[4] == 5)
	{
		actualiserEtAfficherEtatsPhilosophes(4, 0);
	}

	/*switch(state_counter){
		case 0:
			actualiserEtAfficherEtatsPhilosophes(0,2);
			actualiserEtAfficherEtatsPhilosophes(2,2);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
	}
	*/

	/*for(int i; i<NB_PHILOSOPHES;i++){
		//check if someone is ready to eat
		pthread_mutex_lock(&mutexEtats);
		if(etatsPhilosophes[i] == 1){

			// Modify the state
			etatsPhilosophes[i] = 2;
			// Unlock the mutex after finishing the access

		}
		else if(etatsPhilosophes[i] == 3){

			// Modify the state
			etatsPhilosophes[i] = 4;
			// Unlock the mutex after finishing the access

		}
		else if(etatsPhilosophes[i] == 5){
etatsPhilosophes[0]== 1 && etatsPhilosophes[2] == 1
			// Modify the state
			etatsPhilosophes[i] = 0;
			// Unlock the mutex after finishing the access

		}
		pthread_mutex_unlock(&mutexEtats);
	}*/
	// std::cout << "mutex not lock"<< std::endl;
	// pthread_mutex_lock(&mutexEtats);
	// std::cout << "mutexlocked"<< std::endl;

	/*//MAUVAIS CHOIX !!!!!!!!!!!!!!!!!!!
	if(etatsPhilosophes[0]== 1 && etatsPhilosophes[2] == 1 &&etatsPhilosophes[4] == 1 && etatsPhilosophes[3] == 1 && etatsPhilosophes[1] == 1){
		//gr 1 eat
		actualiserEtAfficherEtatsPhilosophes(0,2);
		actualiserEtAfficherEtatsPhilosophes(2,2);
		actualiserEtAfficherEtatsPhilosophes(4,2);
	}
	else if(etatsPhilosophes[0]== 3 && etatsPhilosophes[2] == 3 &&etatsPhilosophes[4] == 3 &&etatsPhilosophes[3] == 1 && etatsPhilosophes[1] == 1){
		//gr 1 ended eat -> think
		actualiserEtAfficherEtatsPhilosophes(0,3);
		actualiserEtAfficherEtatsPhilosophes(2,3);
		actualiserEtAfficherEtatsPhilosophes(4,3);
		// gr2 eat
		actualiserEtAfficherEtatsPhilosophes(1,2);
		actualiserEtAfficherEtatsPhilosophes(3,2);
	}
	else if(etatsPhilosophes[0]== 5 && etatsPhilosophes[2] == 5 &&etatsPhilosophes[4] == 5 &&etatsPhilosophes[3] == 3 && etatsPhilosophes[1] == 3){
		// gr1 hungry again
		actualiserEtAfficherEtatsPhilosophes(0,0);
		actualiserEtAfficherEtatsPhilosophes(2,0);
		actualiserEtAfficherEtatsPhilosophes(4,0);
		// gr2 think
		actualiserEtAfficherEtatsPhilosophes(1,4);
		actualiserEtAfficherEtatsPhilosophes(3,4);
	}
	else if(etatsPhilosophes[0]== 1 && etatsPhilosophes[2] == 1 &&etatsPhilosophes[4] == 1 &&etatsPhilosophes[3] == 5 && etatsPhilosophes[1] == 5){
		// gr1 eat
		actualiserEtAfficherEtatsPhilosophes(0,2);
		actualiserEtAfficherEtatsPhilosophes(2,2);
		actualiserEtAfficherEtatsPhilosophes(4,2);
		//gr2 hungry again
		actualiserEtAfficherEtatsPhilosophes(1,0);
		actualiserEtAfficherEtatsPhilosophes(3,0);
	}
	//pthread_mutex_unlock(&mutexEtats);
	//std::cout << "mutexunlocked"<< std::endl;
	 */
}

void fonctionOrdonnancer_auto()
{
	switch (NB_PHILOSOPHES % 2)
	{
	case 0: // nb philos paire
		/* situation initiale */
		if (start_cond)
		{
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0)
				{
					actualiserEtAfficherEtatsPhilosophes(i, 2);
				}
			}
			start_cond = false;
		}
		/* reste du prog */
		else
		{
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (etatsPhilosophes[i] == 1 && etatsPhilosophes[(i + 1) % NB_PHILOSOPHES] != 2 && etatsPhilosophes[(i - 1 + NB_PHILOSOPHES) % NB_PHILOSOPHES] != 2)
				{
					actualiserEtAfficherEtatsPhilosophes(i, 2);
				}
			}
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (etatsPhilosophes[i] == 3)
				{
					actualiserEtAfficherEtatsPhilosophes(i, 4);
				}
			}
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (etatsPhilosophes[i] == 5)
				{
					actualiserEtAfficherEtatsPhilosophes(i, 0);
				}
			}
		}
		break;
	case 1: // nb philos impaire
		if (start_cond)
		{
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
				{
					actualiserEtAfficherEtatsPhilosophes(i, 2);
				}
				start_cond = false;
			}
		}
		else
		{
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (etatsPhilosophes[i] == 1 && etatsPhilosophes[(i + 1) % NB_PHILOSOPHES] != 2 && etatsPhilosophes[(i - 1 + NB_PHILOSOPHES) % NB_PHILOSOPHES] != 2)
				{
					actualiserEtAfficherEtatsPhilosophes(i, 2);
				}
			}
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (etatsPhilosophes[i] == 3)
				{
					actualiserEtAfficherEtatsPhilosophes(i, 4);
				}
			}
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (etatsPhilosophes[i] == 5)
				{
					actualiserEtAfficherEtatsPhilosophes(i, 0);
				}
			}
		}
		break;
	}
}

void fonctionOrdonnancerWithSemaphores()
{
	/*Logique simple - on utilise les sémaphores pour donner les autorisations */
	if (start_cond)
	{
		for (int i = 0; i < NB_PHILOSOPHES; i++)
		{
			if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
			{
				sem_post(semAutorisation[i]);
			}
		}
		start_cond = false;
		eat_counter++;
	}
	else
	{
		switch (eat_counter)
		{
		case 0:
			sem_wait(semAutorisation[NB_PHILOSOPHES - 1]);
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
				{
					sem_post(semAutorisation[i]);
				}
			}
			eat_counter++;
			break;
		case 1:
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
				{
					sem_wait(semAutorisation[i]);
				}
			}
			for (int i = 1; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 1)
				{
					sem_post(semAutorisation[i]);
				}
			}
			eat_counter++;
			break;
		case 2:
			for (int i = 1; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 1)
				{
					sem_wait(semAutorisation[i]);
				}
			}
			sem_post(semAutorisation[NB_PHILOSOPHES - 1]);
			eat_counter = 0;
			break;
		}
	}
}

bool philosReadyToEatImpair()
{
	// group = eat_counter%NB_PHILOSOPHES;
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		if ((i % 2 == 0) && (i < (NB_PHILOSOPHES - 2)))
		{
			pthread_mutex_lock(&mutexEtats);
			if (etatsPhilosophes[(i + group + 1) % NB_PHILOSOPHES] != 'F')
			{
				pthread_mutex_unlock(&mutexEtats);
				return false;
			}
			pthread_mutex_unlock(&mutexEtats);
		}
	}
	return true;
}

bool philosEndedToEatImpair()
{
	// group = eat_counter%NB_PHILOSOPHES;
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		pthread_mutex_lock(&mutexEtats);
		if (etatsPhilosophes[i] == 'M')
		{
			pthread_mutex_unlock(&mutexEtats);
			return false;
		}
		pthread_mutex_unlock(&mutexEtats);
	}
	return true;
}

bool philosReadyToEatPair()
{
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		if (i % 2 == 0 && group % 2 == 0)
		{
			pthread_mutex_lock(&mutexEtats);
			if (etatsPhilosophes[(i + 1) % NB_PHILOSOPHES] != 'F')
			{
				pthread_mutex_unlock(&mutexEtats);
				return false;
			}
			pthread_mutex_unlock(&mutexEtats);
		}
		else if (i % 2 == 0 && group % 2 == 1)
		{
			pthread_mutex_lock(&mutexEtats);
			if (etatsPhilosophes[i] != 'F')
			{
				pthread_mutex_unlock(&mutexEtats);
				return false;
			}
			pthread_mutex_unlock(&mutexEtats);
		}
	}
	return true;
}

bool philosEndedToEatPair()
{
	// group = eat_counter%NB_PHILOSOPHES;
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		pthread_mutex_lock(&mutexEtats);
		if (etatsPhilosophes[i] == 'M')
		{
			pthread_mutex_unlock(&mutexEtats);
			return false;
		}
		pthread_mutex_unlock(&mutexEtats);
	}
	return true;
}

void fonctionOrdonnancerWithSemaphoresFull()
{
	/*Logique simple - on utilise les sémaphores pour donner les autorisations */

	switch (scenario)
	{
	case 1:
		next_group_ready = philosReadyToEatImpair();
		if ((next_group_ready == true) && (start_cond == false))
		{

			// on wait tous les semaphores autorisation distribué avant de poster les suivants
			// avant d en poster l ordo doit posséder tous les sem
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				// int value;
				// sem_getvalue(semAutorisation[(i)],&value);
				// std::cout<<value<<std::endl;
				if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
				{
					if (group == 0 && i == 0)
					{
						sem_wait(semAutorisation[NB_PHILOSOPHES - 1]);
					}
					else
					{
						sem_wait(semAutorisation[(i + group - 1) % NB_PHILOSOPHES]);
					}
				}
			}
			// on actualise le groupe
			//  on poste les sémaphores d autorisation
			group = eat_counter % NB_PHILOSOPHES; // groupe actuel
			pthread_mutex_lock(&mutexCout);
			std::cout << "group :" << group << std::endl;
			pthread_mutex_unlock(&mutexCout);
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				// int value;
				// sem_getvalue(semAutorisation[(i+group)%NB_PHILOSOPHES],&value);
				if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
				{
					// while(etatsPhilosophes[(i+group)%NB_PHILOSOPHES]!='A');
					sem_post(semAutorisation[(i + group) % NB_PHILOSOPHES]);
					pthread_mutex_lock(&mutexCout);
					std::cout << "philo id posted :" << (i + group) % NB_PHILOSOPHES << std::endl;
					pthread_mutex_unlock(&mutexCout);
					// std::cout<<value<<std::endl;
					// sem_posted = true;
				}
			}
			eat_counter++;
			// group = eat_counter%NB_PHILOSOPHES;
		}
		else if (!next_group_ready && !start_cond)
		{
			// group = eat_counter%NB_PHILOSOPHES; // groupe actuel
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				int value;
				sem_getvalue(semAutorisation[i], &value);
				if (value == 1)
				{
					sem_wait(semAutorisation[i]);
				}
			}
		}
		break;

	case 0:
		if (start_cond)
		{
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && i < (NB_PHILOSOPHES))
				{
					sem_post(semAutorisation[i]);
				}
			}
			start_cond = false;
			eat_counter++;
		}
		else
		{
			group = eat_counter % NB_PHILOSOPHES; // groupe actuel
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i == 0 && group == 0)
				{
					sem_wait(semAutorisation[NB_PHILOSOPHES - 1]);
				}
				else if (i % 2 == 0 && i < (NB_PHILOSOPHES))
				{
					sem_wait(semAutorisation[(i + group - 1) % NB_PHILOSOPHES]);
				}
			}

			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && i < (NB_PHILOSOPHES))
				{
					while (etatsPhilosophes[(i + group) % NB_PHILOSOPHES] != 'F')
						;
					// sem_post(semAutorisation[(i+group)%NB_PHILOSOPHES]);
				}
			}

			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && i < (NB_PHILOSOPHES))
				{
					// while(etatsPhilosophes[(i+group)%NB_PHILOSOPHES]!='A');
					sem_post(semAutorisation[(i + group) % NB_PHILOSOPHES]);
				}
			}
			eat_counter++;
		}
		break;
	}
}

void OrdonnancerAvecSemaphoresFinal()
{
	switch (scenario)
	{
	case 1:
		// next_group_ready = philosReadyToEatImpair();
		pthread_mutex_lock(&mutexCompteur);
		if (philosReadyToEatImpair() && philosEndedToEatImpair() && nbPhilosEating == 0)
		{
			// on actualise le groupe
			// on poste les sémaphores d autorisation
			group = eat_counter % NB_PHILOSOPHES; // groupe actuel
			/*pthread_mutex_lock(&mutexCout);
			std::cout<<"group :"<<group<<std::endl;
			pthread_mutex_unlock(&mutexCout);*/
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				// int value;
				// sem_getvalue(semAutorisation[(i+group)%NB_PHILOSOPHES],&value);
				if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
				{
					// while(etatsPhilosophes[(i+group)%NB_PHILOSOPHES]!='A');
					pthread_mutex_lock(&mutexCout);
					std::cout << "philo id posted :" << (i + group) % NB_PHILOSOPHES << std::endl;
					pthread_mutex_unlock(&mutexCout);
					sem_post(semAutorisation[(i + group) % NB_PHILOSOPHES]);
					// std::cout<<value<<std::endl;
					// sem_posted = true;
				}
			}
			eat_counter++;
			nbPhilosEating = (NB_PHILOSOPHES - 1) / 2;
			// group = eat_counter%NB_PHILOSOPHES;
		}
		pthread_mutex_unlock(&mutexCompteur);
		break;
	case 0:
		// next_group_ready = philosReadyToEatImpair();
		pthread_mutex_lock(&mutexCompteur);
		if (philosReadyToEatPair() && philosEndedToEatPair() && nbPhilosEating == 0)
		{
			// on actualise le groupe
			// on poste les sémaphores d autorisation
			group = eat_counter % NB_PHILOSOPHES; // groupe actuel
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && group % 2 == 0)
				{
					pthread_mutex_lock(&mutexCout);
					std::cout << "philo id posted :" << i << std::endl;
					pthread_mutex_unlock(&mutexCout);
					sem_post(semAutorisation[(i)]);
				}
				else if (i % 2 == 1 && group % 2 == 1)
				{
					pthread_mutex_lock(&mutexCout);
					std::cout << "philo id posted :" << i << std::endl;
					pthread_mutex_unlock(&mutexCout);
					sem_post(semAutorisation[(i)]);
				}
			}
			eat_counter++;
			nbPhilosEating = (NB_PHILOSOPHES) / 2;
			;
		}
		pthread_mutex_unlock(&mutexCompteur);
		break;
	}
}

void initialisation()
{
	start_cond = true;
	timerRunning = false;
	scenario = NB_PHILOSOPHES % 2;
	switch (scenario)
	{
	case 0:
		nbPhilosEating = (NB_PHILOSOPHES) / 2;
		break;
	case 1:
		nbPhilosEating = (NB_PHILOSOPHES - 1) / 2;
		break;
	}
	// Lock the mutex
	pthread_mutex_lock(&mutexEtats);
	// Allocate memory
	etatsPhilosophes = (char *)malloc(NB_PHILOSOPHES * sizeof(char));
	// Unlock the mutex
	pthread_mutex_unlock(&mutexEtats);

	// timer alloc memorry
	timerThread = (pthread_t *)malloc(sizeof(pthread_t));

	// Allocation de mémoire dynamique pour les semaphores ( fourchettes et autorisations)
	semFourchettes = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semAutorisation = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semProgrammeReady = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semFinDeService = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));

	// Creation et ini des sem des frouchettes
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semFourchettes[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semFourchettes[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semFourchettes[i], 0, 1) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem des autorisations -- on wait instant pour éviter que les philosophes puissent les acquérir
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semAutorisation[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semAutorisation[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semAutorisation[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem des autorisations -- on wait instant pour éviter que les philosophes puissent les acquérir
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semProgrammeReady[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semProgrammeReady[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semProgrammeReady[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 0
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem pour la fin de l action manger
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semFinDeService[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semFinDeService[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semFinDeService[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		etatsPhilosophes[i] = 'F';
	}

	/* ******************** Création des threads ***************** */
	// Allocation mémoire des ID
	threadIds = (int *)malloc(NB_PHILOSOPHES * sizeof(int));
	// allocation mémoire des threads
	threadsPhilosophes = (pthread_t *)malloc(NB_PHILOSOPHES * sizeof(pthread_t));

	pthread_create(&timerThread[0], NULL, &timerFunction, NULL);

	for (int j = 0; j < NB_PHILOSOPHES; j++)
	{
		// pthread_attr_t attr;
		// pthread_attr_init(&attr);
		// int a =pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
		// cout<<a<<endl;
		threadIds[j] = j;
		// pthread_create(&threadsPhilosophes[j], NULL, &vieDuPhilosophe, &threadIds[j]);
		if (pthread_create(&threadsPhilosophes[j], NULL, &vieDuPhilosophe, &threadIds[j]) != 0)
		{

			// std::cout<<"Thread creation failed"<<std::endl;
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Thread initialization failed for philo %d\n", j);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	pthread_mutex_lock(&mutexCout);
	fprintf(stderr, "\033[0;32mThread initialization successfully ended for philos\033[0m\n");
	pthread_mutex_unlock(&mutexCout);

	// timerRunning = true;
	// std::cout <<debug<< std::endl;
}

void *timerFunction(void *arg)
{
	// Configuration du thread : il sera annulable à partir de n'importe quel point de préemption
	// (appel bloquant, appel système, etc...)
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		sem_wait(semProgrammeReady[i]);
	}
	usleep(1000);
	// impair
	if (start_cond && scenario == 1)
	{
		for (int i = 0; i < NB_PHILOSOPHES; i++)
		{
			if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
			{
				sem_post(semAutorisation[i]);
				std::cout << "philo id posted :" << (i + group) % NB_PHILOSOPHES << std::endl;
			}
		}
		eat_counter++;
		pthread_mutex_lock(&mutexCompteur);
		nbPhilosEating = (NB_PHILOSOPHES - 1) / 2;
		pthread_mutex_unlock(&mutexCompteur);
		start_cond = false;
		// group = eat_counter%NB_PHILOSOPHES;
	}
	else if (start_cond && scenario == 0)
	{
		for (int i = 0; i < NB_PHILOSOPHES; i++)
		{
			if (i % 2 == 0 && i < (NB_PHILOSOPHES))
			{
				sem_post(semAutorisation[i]);
				std::cout << "philo id posted :" << (i + group) % NB_PHILOSOPHES << std::endl;
			}
		}
		eat_counter++;
		pthread_mutex_lock(&mutexCompteur);
		nbPhilosEating = (NB_PHILOSOPHES) / 2;
		pthread_mutex_unlock(&mutexCompteur);
		start_cond = false;
	}
	while (1)
	{
		// appel ordonnanceur
		// fonctionOrdonnancer();
		// fonctionOrdonnancer_auto();
		// fonctionOrdonnancerWithSemaphores();
		// fonctionOrdonnancerWithSemaphoresFull();
		OrdonnancerAvecSemaphoresFinal();
		// fprintf(stderr,"nb philos eating",nbPhilosEating);
		//  time count
		// std::cout<<group<<std::endl;
		// std::cout << "Elapsed time: " << difftime(currentTime, instantDebut) << " seconds." << std::endl;
		// Sleep
		// std::this_thread::sleep_for(std::chrono::microseconds(10));
		// usleep(1);
		pthread_testcancel(); // point où l'annulation du thread est permise
	}
	return NULL;
}

void *vieDuPhilosophe(void *idPtr)
{
	int id = *((int *)idPtr);
	// Configuration du thread : il sera annulable à partir de n'importe quel point de préemption
	// (appel bloquant, appel système, etc...)
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	struct timespec start, end;
	double elapsed_time = 0.0;
	// while(!timerRunning){}
	fprintf(stderr, "\033[0;32mPhilo with ID %d is ready\033[0m\n", id);
	sem_post(semProgrammeReady[id]);
	clock_gettime(CLOCK_MONOTONIC, &start);
	while (1)
	{

		// ***** À implémenter : *****
		// - structure permettant le contrôle du philosphe
		// - prise/relâchement des fourchettes de gauche et de droite, au bon moment
		// - ordres de changement d'état et d'actualisation de l'affichage dans la foulée
		//     (grâce à : void actualiserEtAfficherEtatsPhilosophes(int, char))
		// - simulation des actions "manger" et "penser" par des appels à usleep(...)
		pthread_mutex_lock(&mutexEtats);
		char state = etatsPhilosophes[id];
		pthread_mutex_unlock(&mutexEtats);

		switch (state) {
			case 'F':
				//std::cout << "Philo : "<< id << " is hungry" << std::endl;
				sem_wait(semAutorisation[id]);
				pthread_testcancel(); // point où l'annulation du thread est permise
				//actualiserEtAfficherEtatsPhilosophes(id,2);
				sem_wait(semFourchettes[id]);
				//usleep(10000);
				sem_wait(semFourchettes[(id+1)%NB_PHILOSOPHES]);
				clock_gettime(CLOCK_MONOTONIC, &end);
				elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
				pthread_mutex_lock(&mutexCsv);
				total_execution_time[id] += elapsed_time;
				execution_count[id]++;
				pthread_mutex_lock(&mutexCout);
				std::cout << "Temps d'attente du philo"<< id << " = " << elapsed_time <<std::endl;
				pthread_mutex_unlock(&mutexCout);
				pthread_mutex_unlock(&mutexCsv);
				actualiserEtAfficherEtatsPhilosophes(id,'M');

			// etatsPhilosophes[id] = 1;
			break;

		case 'M':
			// std::cout << "Philo : "<< id << " managed to get forks and is eating" << std::endl;
			// std::cout << "Philo : "<< id << " is eating" << std::endl;
			manger();
			sem_post(semFourchettes[id]);
			// usleep(10000);
			sem_post(semFourchettes[(id + 1) % NB_PHILOSOPHES]);
			// std::cout << "Philo "<< id << "ended eating success and dropped forks" << std::endl;
			// actualiserEtAfficherEtatsPhilosophes(id,4);
			actualiserEtAfficherEtatsPhilosophes(id, 'P');
			pthread_mutex_lock(&mutexCompteur);
			nbPhilosEating--;
			pthread_mutex_unlock(&mutexCompteur);
			break;

		case 'P':
			// std::cout << "Philo : "<< id << " is thinking" << std::endl;
			penser();
			// std::cout << "Philo : "<< id << " done thinking" << std::endl;
			actualiserEtAfficherEtatsPhilosophes(id, 'F');
			elapsed_time = 0.0;
			clock_gettime(CLOCK_MONOTONIC, &start);
			// std::cout << "Philo : "<< id << " is waiting for an order" << std::endl;
			break;

		default:
			break;
			// code to be executed if expression doesn't match any of the constants
		}

		// SPIN LOCK
		/*if (hungry) {
			sem_wait(semFourchettes[id]);
			usleep(10000);
			sem_wait(semFourchettes[id_1]);
			std::cout << "Philo "<< id << "have forks and ready to eat" << std::endl;
			hungry = 0;
			time_for_eat = 1;
		}

		else if(time_for_eat){
			//sleep((float)rand() % (float)DUREE_MANGE_MAX_S);
			double randomValue = static_cast<double>(rand()) / RAND_MAX * DUREE_MANGE_MAX_S;
			sleep(randomValue);
			sem_post(semFourchettes[id]);
			usleep(10000);
			sem_post(semFourchettes[id_1]);
			std::cout << "Philo "<< id << "ended eating success" << std::endl;
			time_for_eat = 0;
			time_for_think = 1;
		}
		else if(time_for_think){
			//sleep((float)rand() % (float)DUREE_PENSE_MAX_S);
			double randomValue = static_cast<double>(rand()) / RAND_MAX * DUREE_MANGE_MAX_S; // better to  use random than  rand
			sleep(randomValue);
			std::cout << "Philo "<< id << "ended thinking and hungry again" << std::endl;
			time_for_think = 0;
			hungry = 1;
		}*/
		pthread_testcancel(); // point où l'annulation du thread est permise
	}
	return NULL;
}

void actualiserEtAfficherEtatsPhilosophes(int idPhilosopheChangeant, char nouvelEtat)
{
	// ***** À IMPLÉMENTER : PROTECTION ÉTATS *****
	// ***** À IMPLÉMENTER : PROTECTION CONSOLE *****

	char *color = "";
	switch (nouvelEtat)
	{
	case 'M':
		color = ANSI_COLOR_GREEN;
		break;
	case 'F':
		color = ANSI_COLOR_RED;
		break;
	case 'P':
		color = ANSI_COLOR_BLUE;
		break;
	}
	pthread_mutex_lock(&mutexEtats);
	etatsPhilosophes[idPhilosopheChangeant] = nouvelEtat;
	pthread_mutex_unlock(&mutexEtats);
	pthread_mutex_lock(&mutexCout);
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		if (i == idPhilosopheChangeant)
			std::cout << color << "*";
		else
			std::cout << " ";
		std::cout << etatsPhilosophes[i];
		if (i == idPhilosopheChangeant)
			std::cout << "*" << ANSI_COLOR_RESET;
		else
			std::cout << "  ";
	}
	//std::cout << "                 (t=" << difftime(time(NULL), instantDebut) << ")" << std::endl;
	std::cout <<" "<< std::endl;
	pthread_mutex_unlock(&mutexCout);
}

#endif // #ifdef OLUTION_1

#ifdef SOLUTION_2

/*Approche asynchrone
 *
 * On va ici ne pas utiliser l os pour l ordonnancement
 * à la place ce seront les threads eux même qui feront le travail
 * on va dans chaque thread inspecter l état des autres threads
 *
 * */

bool philosReadyToEatImpair()
{
	// group = eat_counter%NB_PHILOSOPHES;
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		if ((i % 2 == 0) && (i < (NB_PHILOSOPHES - 2)))
		{
			// pthread_mutex_lock(&mutexEtats);
			if (etatsPhilosophes[(i + group + 1) % NB_PHILOSOPHES] != 'F')
			{
				// pthread_mutex_unlock(&mutexEtats);
				return false;
			}
			// pthread_mutex_unlock(&mutexEtats);
		}
	}
	return true;
}

bool philosEndedToEatImpair()
{
	// group = eat_counter%NB_PHILOSOPHES;
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pthread_mutex_lock(&mutexEtats);
		if (etatsPhilosophes[i] == 'M')
		{
			// pthread_mutex_unlock(&mutexEtats);
			return false;
		}
		// pthread_mutex_unlock(&mutexEtats);
	}
	return true;
}

bool philosReadyToEatPair()
{
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		if (i % 2 == 0 && group % 2 == 0)
		{
			//pthread_mutex_lock(&mutexEtats);
			if (etatsPhilosophes[(i + 1) % NB_PHILOSOPHES] != 'F')
			{
				//pthread_mutex_unlock(&mutexEtats);
				return false;
			}
			//pthread_mutex_unlock(&mutexEtats);
		}
		else if (i % 2 == 0 && group % 2 == 1)
		{
			//pthread_mutex_lock(&mutexEtats);
			if (etatsPhilosophes[i] != 'F')
			{
				//pthread_mutex_unlock(&mutexEtats);
				return false;
			}
			//pthread_mutex_unlock(&mutexEtats);
		}
	}
	return true;
}

bool philosEndedToEatPair()
{
	// group = eat_counter%NB_PHILOSOPHES;
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		//pthread_mutex_lock(&mutexEtats);
		if (etatsPhilosophes[i] == 'M')
		{
			//pthread_mutex_unlock(&mutexEtats);
			return false;
		}
		//pthread_mutex_unlock(&mutexEtats);
	}
	return true;
}

void OrdonnancerAvecSemaphoresFinal()
{
	switch (scenario)
	{
	case 1:
		// next_group_ready = philosReadyToEatImpair();
		pthread_mutex_lock(&mutexCompteur);
		if (philosReadyToEatImpair() && philosEndedToEatImpair() && nbPhilosEating == 0)
		{
			// on actualise le groupe
			// on poste les sémaphores d autorisation
			group = eat_counter % NB_PHILOSOPHES; // groupe actuel
			/*pthread_mutex_lock(&mutexCout);
			std::cout<<"group :"<<group<<std::endl;
			pthread_mutex_unlock(&mutexCout);*/
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				// int value;
				// sem_getvalue(semAutorisation[(i+group)%NB_PHILOSOPHES],&value);
				if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
				{
					// while(etatsPhilosophes[(i+group)%NB_PHILOSOPHES]!='A');
					pthread_mutex_lock(&mutexCout);
					std::cout << "philo id posted :" << (i + group) % NB_PHILOSOPHES << std::endl;
					pthread_mutex_unlock(&mutexCout);
					sem_post(semAutorisation[(i + group) % NB_PHILOSOPHES]);
					// std::cout<<value<<std::endl;
					// sem_posted = true;
				}
			}
			eat_counter++;
			nbPhilosEating = (NB_PHILOSOPHES - 1) / 2;
			// group = eat_counter%NB_PHILOSOPHES;
		}
		pthread_mutex_unlock(&mutexCompteur);
		break;
	case 0:
		// next_group_ready = philosReadyToEatImpair();
		pthread_mutex_lock(&mutexCompteur);
		if (philosReadyToEatPair() && philosEndedToEatPair() && nbPhilosEating == 0)
		{
			// on actualise le groupe
			// on poste les sémaphores d autorisation
			group = eat_counter % NB_PHILOSOPHES; // groupe actuel
			for (int i = 0; i < NB_PHILOSOPHES; i++)
			{
				if (i % 2 == 0 && group % 2 == 0)
				{
					pthread_mutex_lock(&mutexCout);
					std::cout << "philo id posted :" << i << std::endl;
					pthread_mutex_unlock(&mutexCout);
					sem_post(semAutorisation[(i)]);
				}
				else if (i % 2 == 1 && group % 2 == 1)
				{
					pthread_mutex_lock(&mutexCout);
					std::cout << "philo id posted :" << i << std::endl;
					pthread_mutex_unlock(&mutexCout);
					sem_post(semAutorisation[(i)]);
				}
			}
			eat_counter++;
			nbPhilosEating = (NB_PHILOSOPHES) / 2;
			;
		}
		pthread_mutex_unlock(&mutexCompteur);
		break;
	}
}

void initialisation()
{
	start_cond = true;
	timerRunning = false;
	scenario = NB_PHILOSOPHES % 2;
	switch (scenario)
	{
	case 0:
		nbPhilosEating = (NB_PHILOSOPHES) / 2;
		break;
	case 1:
		nbPhilosEating = (NB_PHILOSOPHES - 1) / 2;
		break;
	}

	/****************** ALLOCATION ET SEMAPHORES ********************************/
	pthread_mutex_lock(&mutexEtats);
	// Allocate memory
	etatsPhilosophes = (char *)malloc(NB_PHILOSOPHES * sizeof(char));
	pthread_mutex_unlock(&mutexEtats);

	// Allocation de mémoire dynamique pour les semaphores ( fourchettes et autorisations)
	semFourchettes = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semAutorisation = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semProgrammeReady = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));

	// Creation et ini des sem des frouchettes
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semFourchettes[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semFourchettes[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semFourchettes[i], 0, 1) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem des autorisations
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semAutorisation[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semAutorisation[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semAutorisation[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem des programme pret
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semProgrammeReady[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semProgrammeReady[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semProgrammeReady[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 0
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		etatsPhilosophes[i] = 'F';
	}

	/* ******************** Création des threads ***************** */
	// Allocation mémoire des ID
	threadIds = (int *)malloc(NB_PHILOSOPHES * sizeof(int));
	// allocation mémoire des threads
	threadsPhilosophes = (pthread_t *)malloc(NB_PHILOSOPHES * sizeof(pthread_t));

	for (int j = 0; j < NB_PHILOSOPHES; j++)
	{
		// pthread_attr_t attr;
		// pthread_attr_init(&attr);
		// int a =pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
		// cout<<a<<endl;
		threadIds[j] = j;
		// pthread_create(&threadsPhilosophes[j], NULL, &vieDuPhilosophe, &threadIds[j]);
		if (pthread_create(&threadsPhilosophes[j], NULL, &vieDuPhilosophe, &threadIds[j]) != 0)
		{

			// std::cout<<"Thread creation failed"<<std::endl;
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Thread initialization failed for philo %d\n", j);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	pthread_mutex_lock(&mutexCout);
	fprintf(stderr, "\033[0;32mThread initialization successfully ended for philos\033[0m\n");
	pthread_mutex_unlock(&mutexCout);

	/********************************* INIT ordonnanceur ************************************/
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		sem_wait(semProgrammeReady[i]);
	}
	usleep(1000);
	// impair
	if (start_cond && scenario == 1)
	{
		for (int i = 0; i < NB_PHILOSOPHES; i++)
		{
			if (i % 2 == 0 && i < (NB_PHILOSOPHES - 2))
			{
				sem_post(semAutorisation[i]);
				std::cout << "philo id posted :" << (i + group) % NB_PHILOSOPHES << std::endl;
			}
		}
		eat_counter++;
		pthread_mutex_lock(&mutexCompteur);
		nbPhilosEating = (NB_PHILOSOPHES - 1) / 2;
		pthread_mutex_unlock(&mutexCompteur);
		start_cond = false;
		// group = eat_counter%NB_PHILOSOPHES;
	}
	else if (start_cond && scenario == 0)
	{
		for (int i = 0; i < NB_PHILOSOPHES; i++)
		{
			if (i % 2 == 0 && i < (NB_PHILOSOPHES))
			{
				sem_post(semAutorisation[i]);
				std::cout << "philo id posted :" << (i + group) % NB_PHILOSOPHES << std::endl;
			}
		}
		eat_counter++;
		pthread_mutex_lock(&mutexCompteur);
		nbPhilosEating = (NB_PHILOSOPHES) / 2;
		pthread_mutex_unlock(&mutexCompteur);
		start_cond = false;
	}
	// timerRunning = true;
	// std::cout <<debug<< std::endl;
}

void *vieDuPhilosophe(void *idPtr){
	int id = *((int *)idPtr);
	// Configuration du thread : il sera annulable à partir de n'importe quel point de préemption
	// (appel bloquant, appel système, etc...)
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	// while(!timerRunning){}
	struct timespec start, end;

	fprintf(stderr, "\033[0;32mPhilo with ID %d is ready\033[0m\n", id);
	sem_post(semProgrammeReady[id]);
	elapsed_time = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);
	while (1)
	{

		// ***** À implémenter : *****
		// - structure permettant le contrôle du philosphe
		// - prise/relâchement des fourchettes de gauche et de droite, au bon moment
		// - ordres de changement d'état et d'actualisation de l'affichage dans la foulée
		//     (grâce à : void actualiserEtAfficherEtatsPhilosophes(int, char))
		// - simulation des actions "manger" et "penser" par des appels à usleep(...)
		pthread_mutex_lock(&mutexEtats);
		char state = etatsPhilosophes[id];
		pthread_mutex_unlock(&mutexEtats);
		
		switch (state)
		{
		case 'F':
        			// std::cout << "Philo : "<< id << " is hungry" << std::endl;
			sem_wait(semAutorisation[id]);
			//pthread_testcancel(); // point où l'annulation du thread est permise
			// actualiserEtAfficherEtatsPhilosophes(id,2);
			sem_wait(semFourchettes[id]);
			// usleep(10000);
			sem_wait(semFourchettes[(id + 1) % NB_PHILOSOPHES]);
			
			clock_gettime(CLOCK_MONOTONIC, &end);
			elapsed_time = (end.tv_sec - start.tv_sec) + ((end.tv_nsec - start.tv_nsec) / 1e9);
			if (elapsed_time < 0) {
   		 		// adjust for nanosecond wrap-around
    				elapsed_time = (end.tv_sec - start.tv_sec - 1) + ((1e9 + end.tv_nsec - start.tv_nsec) / 1e9);
			}

			pthread_mutex_lock(&mutexCsv);
			total_execution_time[id] += elapsed_time;
			execution_count[id]++;
			pthread_mutex_unlock(&mutexCsv);
			
			actualiserEtAfficherEtatsPhilosophes(id, 'M');


			// etatsPhilosophes[id] = 1;
			break;

		case 'M':
			// std::cout << "Philo : "<< id << " managed to get forks and is eating" << std::endl;
			// std::cout << "Philo : "<< id << " is eating" << std::endl;
			manger();
			sem_post(semFourchettes[id]);
			// usleep(10000);
			sem_post(semFourchettes[(id + 1) % NB_PHILOSOPHES]);
			// std::cout << "Philo "<< id << "ended eating success and dropped forks" << std::endl;
			// actualiserEtAfficherEtatsPhilosophes(id,4);
			actualiserEtAfficherEtatsPhilosophes(id, 'P');
			pthread_mutex_lock(&mutexCompteur);
			nbPhilosEating--;
			pthread_mutex_unlock(&mutexCompteur);
			break;

		case 'P':
			// std::cout << "Philo : "<< id << " is thinking" << std::endl;
			penser();
			// std::cout << "Philo : "<< id << " done thinking" << std::endl;
			actualiserEtAfficherEtatsPhilosophes(id, 'F');
			elapsed_time = 0.0;
			clock_gettime(CLOCK_MONOTONIC, &start);
			// std::cout << "Philo : "<< id << " is waiting for an order" << std::endl;
			break;

		default:
			break;
			// code to be executed if expression doesn't match any of the constants
		}
		pthread_testcancel(); // point où l'annulation du thread est permise
	}
	return NULL;
}

void actualiserEtAfficherEtatsPhilosophes(int idPhilosopheChangeant, char nouvelEtat)
{
	// ***** À IMPLÉMENTER : PROTECTION ÉTATS *****
	// ***** À IMPLÉMENTER : PROTECTION CONSOLE *****
	char *color = "";
	pthread_mutex_lock(&mutexEtats);
	etatsPhilosophes[idPhilosopheChangeant] = nouvelEtat;
	bool printtime = 0;
	switch (nouvelEtat)
	{
		case 'M':
			color = ANSI_COLOR_GREEN;
			printtime = 1;
			break;
		case 'F':
			color = ANSI_COLOR_RED;
			OrdonnancerAvecSemaphoresFinal();
			break;
		case 'P':
			color = ANSI_COLOR_BLUE;
			OrdonnancerAvecSemaphoresFinal();
			break;
	}
	pthread_mutex_unlock(&mutexEtats);
	pthread_mutex_lock(&mutexCout);
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		if (i == idPhilosopheChangeant)
			std::cout << color << "*";
		else
			std::cout << " ";
		std::cout << etatsPhilosophes[i];
		if (i == idPhilosopheChangeant)
			std::cout << "*" << ANSI_COLOR_RESET;
		else
			std::cout << "  ";
	}
	
	if(printtime){
		std::cout << "Temps d'attente du philo"<< idPhilosopheChangeant << " = " << elapsed_time <<std::endl;
		printtime = 0;
	}
	else{
		std::cout<< " " <<std::endl;
	}	
pthread_mutex_unlock(&mutexCout);
}
#endif

#ifdef SOLUTION_3 // interblocage

void *timerFunction(void *arg)
{
	// Configuration du thread : il sera annulable à partir de n'importe quel point de préemption
	// (appel bloquant, appel système, etc...)
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	// start_cond = true;
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		sem_wait(semProgrammeReady[i]);
	}
	usleep(1000);
	while (1)
	{
		// appel ordonnanceur
		// fonctionOrdonnancer();
		// fonctionOrdonnancer_auto();
		// fonctionOrdonnancerWithSemaphores();
		// fonctionOrdonnancerWithSemaphoresFull();
		// OrdonnancerAvecSemaphoresFinal();
		// fprintf(stderr,"nb philos eating",nbPhilosEating);
		// time count
		// std::cout<<group<<std::endl;
		time_t currentTime = time(NULL);
		// std::cout << "Elapsed time: " << difftime(currentTime, instantDebut) << " seconds." << std::endl;

		// Sleep
		// std::this_thread::sleep_for(std::chrono::microseconds(10));
		// usleep(1);
		pthread_testcancel(); // point où l'annulation du thread est permise
	}
	return NULL;
}

void initialisation()
{
	start_cond = true;
	timerRunning = false;
	scenario = NB_PHILOSOPHES % 2;
	switch (scenario)
	{
	case 0:
		nbPhilosEating = (NB_PHILOSOPHES) / 2;
		break;
	case 1:
		nbPhilosEating = (NB_PHILOSOPHES - 1) / 2;
		break;
	}
	// Lock the mutex
	pthread_mutex_lock(&mutexEtats);
	// Allocate memory
	etatsPhilosophes = (char *)malloc(NB_PHILOSOPHES * sizeof(char));
	// Unlock the mutex
	pthread_mutex_unlock(&mutexEtats);

	// timer alloc memorry
	timerThread = (pthread_t *)malloc(sizeof(pthread_t));

	// Allocation de mémoire dynamique pour les semaphores ( fourchettes et autorisations)
	semFourchettes = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semAutorisation = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semProgrammeReady = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));
	semFinDeService = (sem_t **)malloc(NB_PHILOSOPHES * sizeof(sem_t *));

	// Creation et ini des sem des frouchettes
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semFourchettes[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semFourchettes[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semFourchettes[i], 0, 1) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem des autorisations -- on wait instant pour éviter que les philosophes puissent les acquérir
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semAutorisation[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semAutorisation[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semAutorisation[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem des autorisations -- on wait instant pour éviter que les philosophes puissent les acquérir
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semProgrammeReady[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semProgrammeReady[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semProgrammeReady[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 0
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	// Creation et ini des sem pour la fin de l action manger
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		// pour chaque élement du tableau faire une allocation dynamique
		// le tableau  étant un tableau de pointeurs
		// chaque poiteur nécéssite l'allocation d'un espace en mémoire et un espace mémoire vers lequels il pointe
		// c'est ce dernier qu'on réserve ici
		semFinDeService[i] = (sem_t *)malloc(sizeof(sem_t));

		/* Gestion des erreurs*/
		if (semFinDeService[i] == NULL)
		{
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Memory allocation failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}

		if (sem_init(semFinDeService[i], 0, 0) != 0)
		{ // Initialize semaphore with an initial value of 1 --> free for use
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Semaphore initialization failed for semaphore %d\n", i);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		etatsPhilosophes[i] = 'F';
	}

	/* ******************** Création des threads ***************** */
	// Allocation mémoire des ID
	threadIds = (int *)malloc(NB_PHILOSOPHES * sizeof(int));
	// allocation mémoire des threads
	threadsPhilosophes = (pthread_t *)malloc(NB_PHILOSOPHES * sizeof(pthread_t));

	pthread_create(&timerThread[0], NULL, &timerFunction, NULL);

	for (int j = 0; j < NB_PHILOSOPHES; j++)
	{
		// pthread_attr_t attr;
		// pthread_attr_init(&attr);
		// int a =pthread_attr_setschedpolicy(&attr,SCHED_FIFO);
		// cout<<a<<endl;
		threadIds[j] = j;
		// pthread_create(&threadsPhilosophes[j], NULL, &vieDuPhilosophe, &threadIds[j]);
		if (pthread_create(&threadsPhilosophes[j], NULL, &vieDuPhilosophe, &threadIds[j]) != 0)
		{

			// std::cout<<"Thread creation failed"<<std::endl;
			pthread_mutex_lock(&mutexCout);
			fprintf(stderr, "Thread initialization failed for philo %d\n", j);
			pthread_mutex_unlock(&mutexCout);
			exit(EXIT_FAILURE);
		}
	}

	pthread_mutex_lock(&mutexCout);
	fprintf(stderr, "\033[0;32mThread initialization successfully ended for philos\033[0m\n");
	pthread_mutex_unlock(&mutexCout);

	// timerRunning = true;
	// std::cout <<debug<< std::endl;
}

void *vieDuPhilosophe(void *idPtr)
{
	int id = *((int *)idPtr);
	// Configuration du thread : il sera annulable à partir de n'importe quel point de préemption
	// (appel bloquant, appel système, etc...)
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	// while(!timerRunning){}
	fprintf(stderr, "\033[0;32mPhilo with ID %d is ready\033[0m\n", id);
	sleep(1);
	sem_post(semProgrammeReady[id]);
	actualiserEtAfficherEtatsPhilosophes(id, 'F');
	while (1)
	{

		// ***** À implémenter : *****
		// - structure permettant le contrôle du philosphe
		// - prise/relâchement des fourchettes de gauche et de droite, au bon moment
		// - ordres de changement d'état et d'actualisation de l'affichage dans la foulée
		//     (grâce à : void actualiserEtAfficherEtatsPhilosophes(int, char))
		// - simulation des actions "manger" et "penser" par des appels à usleep(...)
		pthread_mutex_lock(&mutexEtats);
		char state = etatsPhilosophes[id];
		pthread_mutex_unlock(&mutexEtats);
		switch (state)
		{
		case 'F':
			// std::cout << "Philo : "<< id << " is hungry" << std::endl;
			// sem_wait(semAutorisation[id]);
			// actualiserEtAfficherEtatsPhilosophes(id,2);
			sem_wait(semFourchettes[id]);
			sleep(10);
			sem_wait(semFourchettes[(id + 1) % NB_PHILOSOPHES]);
			actualiserEtAfficherEtatsPhilosophes(id, 'M');

			// etatsPhilosophes[id] = 1;
			break;

		case 'M':
			// std::cout << "Philo : "<< id << " managed to get forks and is eating" << std::endl;
			// std::cout << "Philo : "<< id << " is eating" << std::endl;
			manger();
			sem_post(semFourchettes[id]);
			// usleep(10000);
			sem_post(semFourchettes[(id + 1) % NB_PHILOSOPHES]);
			// std::cout << "Philo "<< id << "ended eating success and dropped forks" << std::endl;
			// actualiserEtAfficherEtatsPhilosophes(id,4);
			actualiserEtAfficherEtatsPhilosophes(id, 'P');
			pthread_mutex_lock(&mutexCompteur);
			nbPhilosEating--;
			pthread_mutex_unlock(&mutexCompteur);
			break;

		case 'P':
			// std::cout << "Philo : "<< id << " is thinking" << std::endl;
			penser();
			// std::cout << "Philo : "<< id << " done thinking" << std::endl;
			actualiserEtAfficherEtatsPhilosophes(id, 'F');
			// std::cout << "Philo : "<< id << " is waiting for an order" << std::endl;
			break;

		default:
			break;
		}

		// SPIN LOCK
		/*if (hungry) {
			sem_wait(semFourchettes[id]);
			usleep(10000);
			sem_wait(semFourchettes[id_1]);
			std::cout << "Philo "<< id << "have forks and ready to eat" << std::endl;
			hungry = 0;
			time_for_eat = 1;
		}

		else if(time_for_eat){
			//sleep((float)rand() % (float)DUREE_MANGE_MAX_S);
			double randomValue = static_cast<double>(rand()) / RAND_MAX * DUREE_MANGE_MAX_S;
			sleep(randomValue);
			sem_post(semFourchettes[id]);
			usleep(10000);
			sem_post(semFourchettes[id_1]);
			std::cout << "Philo "<< id << "ended eating success" << std::endl;
			time_for_eat = 0;
			time_for_think = 1;
		}
		else if(time_for_think){
			//sleep((float)rand() % (float)DUREE_PENSE_MAX_S);
			double randomValue = static_cast<double>(rand()) / RAND_MAX * DUREE_MANGE_MAX_S; // better to  use random than  rand
			sleep(randomValue);
			std::cout << "Philo "<< id << "ended thinking and hungry again" << std::endl;
			time_for_think = 0;
			hungry = 1;
		}*/
		pthread_testcancel(); // point où l'annulation du thread est permise
	}
	return NULL;
}

void actualiserEtAfficherEtatsPhilosophes(int idPhilosopheChangeant, char nouvelEtat)
{
	// ***** À IMPLÉMENTER : PROTECTION ÉTATS *****
	// ***** À IMPLÉMENTER : PROTECTION CONSOLE *****

	char *color = "";
	switch (nouvelEtat)
	{
	case 'M':
		color = ANSI_COLOR_GREEN;
		break;
	case 'F':
		color = ANSI_COLOR_RED;
		break;
	case 'P':
		color = ANSI_COLOR_BLUE;
		break;
	}
	pthread_mutex_lock(&mutexEtats);
	etatsPhilosophes[idPhilosopheChangeant] = nouvelEtat;
	pthread_mutex_unlock(&mutexEtats);
	pthread_mutex_lock(&mutexCout);
	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		if (i == idPhilosopheChangeant)
			std::cout << color << "*";
		else
			std::cout << " ";
		std::cout << etatsPhilosophes[i];
		if (i == idPhilosopheChangeant)
			std::cout << "*" << ANSI_COLOR_RESET;
		else
			std::cout << "  ";
	}
	std::cout << "                 (t=" << difftime(time(NULL), instantDebut) << ")" << std::endl;
	pthread_mutex_unlock(&mutexCout);
}

#endif
/********************************* Fonction globales **********************************************/
void penser(void)
{
	double randomValue = (double)rand() / RAND_MAX * DUREE_PENSE_MAX_S;
	sleep((useconds_t)(randomValue));
}
void manger(void)
{
	double randomValue = (double)rand() / RAND_MAX * DUREE_MANGE_MAX_S;
	sleep((useconds_t)(randomValue));
}
void writeDataToCSV() {
    std::ofstream outputFile("philosopher_data.csv");

    // Header
    outputFile << "Philosopher ID,Total Execution Time (s),Execution Count,Average Execution Time (s)\n";

    for (int i = 0; i < NB_PHILOSOPHES; ++i) {
    	average_execution_time[i] = total_execution_time[i] / execution_count[i];
        outputFile << i << ","
                   << total_execution_time[i] << ","
                   << execution_count[i] << ","
                   << average_execution_time[i] << "\n";
    }

    outputFile.close();
}

void terminerProgramme()
{
	// Step 1: Cancel all philosopher threads
   	for (int i = 0; i < NB_PHILOSOPHES; i++) {
        	pthread_cancel(threadsPhilosophes[i]);
    	}

   	 // Step 2: Wait for all threads to actually terminate
    	for (int i = 0; i < NB_PHILOSOPHES; i++) {
        	pthread_join(threadsPhilosophes[i], nullptr);
    	}

	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		sem_destroy(semFourchettes[i]);
		free(semFourchettes[i]);
	}

	free(semFourchettes);
	std::cout << "Forks destroyed" << std::endl;

	for (int i = 0; i < NB_PHILOSOPHES; i++)
	{
		sem_destroy(semAutorisation[i]);
		free(semAutorisation[i]);
	}
	free(semAutorisation);
	free(threadsPhilosophes);
	free(threadIds);
	free(etatsPhilosophes);
	free(timerThread);

	std::cout << "Philos destroyed" << std::endl;

	/*for(int i = 0; i<NB_PHILOSOPHES; i++){
		double average_execution_time = total_execution_time[i] / execution_count[i];
		// Stocker le temps d'exécution dans un fichier CSV
		std::ofstream csv_file("execution_times.csv", std::ios::app);
		csv_file << "Moyenne de temps pour philo avec ID"<< i << "=" << average_execution_time  << std::endl;
		csv_file.close();
	}*/
	writeDataToCSV();
	timerRunning = false;
	// pthread_join(&timerThread);
	// exit(EXIT_SUCCESS);
}
