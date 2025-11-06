//
// Created by USER on 22/10/2025.
//

#include "functions.h"

cell *create_cell(int arr, float proba) {
    cell *new_cell = malloc(sizeof(cell));
    if (new_cell == NULL) {
        fprintf(stderr, "Memory allocation error (create_cell)\n");
        exit(EXIT_FAILURE);
    }
    new_cell->arr = arr;
    new_cell->proba = proba;
    new_cell->next = NULL;
    return new_cell;
}

list create_list() {
    list l;
    l.head = NULL;
    return l;
}

void add_cell(list *l, int arr, float proba) {
    cell *new_cell = create_cell(arr, proba);
    new_cell->next = l->head;
    l->head = new_cell;
}

void display_list(const list *l) {
    cell *current = l->head;
    while (current != NULL) {
        printf(" -> (%d, %.2f)", current->arr, current->proba);
        current = current->next;
    }
    printf("\n");
}

a_list *create_a_list(int size) {
    a_list *graph = malloc(sizeof(a_list));
    if (graph == NULL) {
        fprintf(stderr, "Memory allocation error (create_a_list)\n");
        exit(EXIT_FAILURE);
    }

    graph->size = size;
    graph->array = malloc(size * sizeof(list));
    if (graph->array == NULL) {
        fprintf(stderr, "Memory allocation error (create_a_list array)\n");
        free(graph);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        graph->array[i] = create_list();
    }

    return graph;
}

void display_a_list(const a_list *graph) {
    for (int i = 0; i < graph->size; i++) {
        printf("Vertex %d:", i);
        display_list(&graph->array[i]);
    }
}

a_list readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, start, end;
    float proba;
    a_list *list;
    if (file == NULL)
    {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }
    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1)
    {
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }
    list = create_a_list(nbvert);
    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3)
    {
        // we obtain, for each line of the file, the values
        // start, end and proba
        cell *new_cell = create_cell(start, proba);
        list->array[start].head = new_cell;
    }
    fclose(file);
    return *list;
}

int check_graph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Array to store sum of probabilities for each vertex
    float *sums = calloc(nbvert + 1, sizeof(float)); // +1 for 1-indexed vertices
    if (sums == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int start, end;
    float proba;
    int valid = 1;

    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3) {
        // Adjust for 1-indexed vertices (if that's your format)
        if (start < 1 || start > nbvert) {
            fprintf(stderr, "Invalid start vertex: %d (expected 1-%d)\n", start, nbvert);
            valid = 0;
            break;
        }
        sums[start] += proba;
    }

    // Check if probabilities for each vertex sum to ~1
    if (valid) {
        for (int i = 1; i <= nbvert; i++) {
            if (sums[i] > 0 && (sums[i] < 0.99 || sums[i] > 1.01)) {
                printf("Vertex %d probabilities sum to %f (should be ~1.0)\n", i, sums[i]);
                valid = 0;
                break;
            }
        }
    }

    free(sums);
    fclose(file);
    return valid;
}


char *getId(int num) {
    if (num <= 0) {
        char *result = malloc(1);
        if (result) result[0] = '\0';
        return result;
    }
    int len = 0;
    for (int temp = num; temp > 0; temp = (temp - 1) / 26) len++;

    char *result = malloc(len + 1);
    if (!result) return NULL;

    result[len] = '\0';
    for (int i = len - 1; i >= 0; i--) {
        num--;
        result[i] = 'A' + (num % 26);
        num /= 26;
    }
    return result;
}

