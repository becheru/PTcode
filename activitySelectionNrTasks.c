#include <stdio.h>
#include <stdlib.h>

// the bulding block of a heap
struct g_node{
    int activity;
    struct g_node *next;
};

// adds a new activity in a heap
void push_activity(struct g_node *head, int activity){
    struct g_node * new_activity = malloc(sizeof(struct g_node));
    new_activity -> activity = activity;
    new_activity -> next = head -> next;
    head -> next = new_activity;
}

//removes an activity from a heap
void pop_activity(struct g_node *head){
    struct g_node *poped_activity = head -> next;
    if (head -> next != NULL){
        head -> next = poped_activity -> next;
        free(poped_activity);
    }
}

// print the elements from a heap, the heap keeping the solutions
void print_solution(struct g_node *solution_list){
    struct g_node *iterator = solution_list;

    if (solution_list -> next == NULL){
        printf("There are no solutions\n");
    }else{
        printf("The solutions are:\n");
    }

    while (iterator -> next != NULL){
        iterator = iterator -> next;
        printf(" %d", iterator -> activity);
    }

    printf("\n");
}

// removes all elements from a heap with the expcetion of the head
void empty_list(struct g_node *solution_list){
    struct g_node *iterator;
    struct g_node *temp;

    if (solution_list -> next == NULL) {
        //printf("List already empty\n");
    }else{
        iterator = solution_list->next;
        solution_list->next = NULL;

        while (iterator->next != NULL) {
            temp = iterator;
            iterator = iterator->next;
            free(temp);
        }

        free(iterator);
        //printf("List empty\n");
    }
}

// verifies if a given activity has been used to build the current solution
// if yes it returns 1
// otherwise it returns 0
int activity_chosen(struct g_node *solution_list, int activity){
    struct g_node *iterator;
    if (solution_list -> next == NULL){
        return 0;
    }else{
        iterator = solution_list;
    }

    while (iterator -> next != NULL){
        iterator = iterator -> next;
        if (activity == iterator -> activity){
            return 1;
        }
    }

    return 0;
}

// verifies if a given activity can be added to the curret solution
// the activity must not overlap with other activities present in the solution
// as our list of solution is already sorted we need to verify just for the last activity added to the solution
int activity_compatible(int *start_time, int *end_time,struct g_node *solution_list, int activity){
    int last_chosen_activity;

    if (solution_list -> next == NULL){
        return 1;
    }else{
        last_chosen_activity = solution_list -> next -> activity;
        if (start_time[activity] >= end_time[last_chosen_activity]){
            return 1;
        }else{
            return 0;
        }
    }
}

//determines the number of activies in the solution heap
int solution_length(struct g_node *solution_list){
    int counter = 0;
    struct g_node *iterator = solution_list;

    while (iterator-> next != NULL){
        iterator = iterator -> next;
        counter++;
    }

    return counter;
}

//compares the current solution with the best solution
//the comparison is based on the higher number of activities
// if the current solution is better than it returns 1
//otherwise it returns 0
int compare_solutions(struct g_node *head_current_solution, struct g_node *head_best_solution){
    if (solution_length(head_current_solution) > solution_length(head_best_solution)){
        return 1;
    }else{
        return 0;
    }
}


// saves the current solution as the new best solution
//the current methods overwrites the previous best solution
void save_as_best_solution(struct g_node *head_current_solution, struct g_node *head_best_solution){
    struct g_node *iterator = head_current_solution;
    empty_list(head_best_solution);


    while (iterator->next != NULL){
        iterator = iterator -> next;
        push_activity(head_best_solution,iterator -> activity);
    }
}


void greedy_solution(int *start_time, int *end_time, struct g_node *solution_list, int nr_activities) {
    int last_selected_activity;
    int current_activity;

    // The first activity always gets selected
    push_activity(solution_list, 0);
    last_selected_activity = 0;


    // Consider rest of the activities
    for (current_activity = 1; current_activity < nr_activities; current_activity++) {
        // If this activity has start time greater than or
        // equal to the finish time of previously selected
        // activity, then select it
        if (start_time[current_activity] >= end_time[last_selected_activity]) {
            push_activity(solution_list, current_activity);
            last_selected_activity = current_activity;
        }
    }
}

void backtracking_solution(int *start_time, int *end_time, struct g_node *solution_list, struct g_node *best_solution_list, int nr_activities) {
    int current_activity;

    print_solution(solution_list);

    if (compare_solutions(solution_list,best_solution_list) == 1){
        save_as_best_solution(solution_list,best_solution_list);
    }

    for (current_activity = 0; current_activity < nr_activities ; current_activity++) {
        if ((activity_chosen(solution_list, current_activity) == 0) &&
            (activity_compatible(start_time, end_time, solution_list, current_activity)==1)) {
                push_activity(solution_list, current_activity);
                backtracking_solution(start_time, end_time, solution_list, best_solution_list, nr_activities);
                pop_activity(solution_list);
        }
    }
}


int main() {
// greedy ok
// backtraking ok
    int start_time[] =  {1, 3, 4, 5};
    int end_time[] =  {2, 4, 5, 6};

// greedy ok
// backtraking ok
//    int start_time[] =  {1, 3, 0, 5, 8, 5};
//    int end_time[] =  {2, 4, 6, 7, 9, 9};

// greedy ok
// backtraking ok
//    int start_time[] =  {3, 0, 1, 1, 8, 5};
//    int end_time[] =  {4, 6, 7, 9, 9, 9};

    int nr_activities = sizeof(start_time) / sizeof(start_time[0]);
    struct g_node *solution_list = malloc(sizeof(struct g_node));
    solution_list -> next = NULL;
    struct g_node *best_solution_list = malloc(sizeof(struct g_node));
    best_solution_list -> next = NULL;


    greedy_solution(start_time, end_time, solution_list, nr_activities);
    print_solution(solution_list);
    empty_list(solution_list);

    backtracking_solution(start_time, end_time, solution_list, best_solution_list, nr_activities);
    printf("The best solution is: \n");
    print_solution(best_solution_list);
    empty_list(best_solution_list);

    free(solution_list);
    free(best_solution_list);

    return 0;
}