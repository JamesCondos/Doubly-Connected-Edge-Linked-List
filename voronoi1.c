#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

int
right(Watchtower *towers, half_edge *h_edge, vertex **vertices) {

    //Define the start and end coordinates for each half-edge
    int inside = 0;
    double x1 = vertices[h_edge->start_vertex]->x;
    double x2 = vertices[h_edge->end_vertex]->x;
    double y1 = vertices[h_edge->start_vertex]->y;
    double y2 = vertices[h_edge->end_vertex]->y;
    double c = 0;
    double gradient  = 0;
    double changey = 0;
    double ypredicted = 0;

    //case 1 for a point lying in a face
    if ((x1 == x2) && (y1 < y2)) {

        if (towers->watchtower_x > x1) {
            inside = 1;
            return inside;
            
        }
      
    }

    //case 2 for a point lying in a face
    if (x2 > x1) {

            //////FIND GRADIENT
             gradient = (y2-y1)/(x2-x1);
            
            c = y2 - gradient * x2;

             ypredicted = gradient * (towers->watchtower_x) + c;

             changey = towers->watchtower_y - ypredicted;

            if (changey <= 0) {
                inside = 1;
                return inside;
            }
    }

    //case 3 for a point lying in a face
    if ((x1 == x2) && (y1 > y2)) {

        if (towers->watchtower_x < x1) {
            inside = 1;
            return inside;
            
        }    
    }

    
    //case 4 for a point lying in a face
    if (x1 > x2) {

            //////FIND GRADIENT
             gradient = (y2-y1)/(x2-x1);
            
            c = y2 - gradient * x2;

             ypredicted = gradient * (towers->watchtower_x) + c;

             changey = towers->watchtower_y - ypredicted;

            if (changey >= 0) {
                inside = 1;
                return inside;
            }
    }

    return 0;
}

//function to prepare the ouput to check whether a 
//givien point is in a specific face
int
prepare_output(Watchtower *towers, half_edge *origin2,
half_edge *traverse, vertex **vertices) {

    int inside = 0;

    if (!right(towers, origin2, vertices )) {

        return inside;

    }   else {
        //now traverse the linked list and check for
        // each point in ever face

        traverse = traverse->next;
        while(traverse != origin2) {

            inside = right(towers, traverse, vertices);

            if (!inside) {

                return inside;
            }
            traverse = traverse->next;

        }


    }

    return inside;

}


int
main(int argc, char *argv[]) {

    FILE * fp; 
    char *file_csv;
    file_csv = argv[1];

    fp = fopen(file_csv, "r"); 

    //define arrays of pointers to store attributes of watchtowers
    Watchtower **watchtower_info = NULL;
	watchtower_info = (Watchtower **)malloc(sizeof(Watchtower*));


    //initialse the longest character size for a line
    char *line = NULL;
    size_t buffsize = 512;
    int total_towers = 0;

    int i = 0;
    char *chunk;
    getline(&line, &buffsize, fp);
    
    while (getline(&line, &buffsize, fp) != EOF) {

        chunk = strtok(line, ",");
        total_towers++;

        while (chunk != NULL) {

            //now record all the data using these arrays and malloc
            watchtower_info[i] = (Watchtower*)malloc(sizeof(Watchtower));

            watchtower_info[i]->ID = (char *)malloc(sizeof(char) *
            (strlen(chunk) + sizeof(char)));
            strcpy(watchtower_info[i]->ID,chunk);
            chunk = strtok(NULL, ",");

            watchtower_info[i]->postcode = (char *)malloc(sizeof(char)*
            (strlen(chunk) + sizeof(char)));
            strcpy(watchtower_info[i]->postcode,chunk);
            chunk = strtok(NULL, ",");

            watchtower_info[i]->pop_served = atoi(chunk);
            chunk = strtok(NULL, ",");

            watchtower_info[i]->name = (char* )malloc(sizeof(char)*
             (strlen(chunk) + sizeof(char)));
            strcpy(watchtower_info[i]->name, chunk);

            chunk = strtok(NULL, ",");

            (watchtower_info[i]->watchtower_x) = strtod(chunk, NULL);
            chunk = strtok(NULL, ",");

            (watchtower_info[i]->watchtower_y) = strtod(chunk, NULL);
            chunk = strtok(NULL, ",");

        }

        i++;
        
        watchtower_info = realloc(watchtower_info, 
        (i+1) * sizeof(Watchtower*));

    }



    //prepare the next file for storing and creating the polygon
    FILE *fp1 = fopen(argv[2], "r");

    //now initialise the following arrays to store data for the polygon
    vertex **vertices = (vertex **) malloc(sizeof(vertex*));
    face **faces = (face **) malloc(sizeof(face*));
    edge **edges = (edge **) malloc(sizeof(edge*));
    int vertices_count = 1;
    int edges_count = 1;
    int faces_count = 1;


    //now intialse each array and create the first polygon
    //that is being read in

    vertices[0] = (vertex *)malloc(sizeof(vertex));
    fscanf(fp1, "%lf %lf\n", &vertices[0]->x, &vertices[0]->y);

    half_edge *origin_half_edge = (half_edge *)malloc(sizeof(half_edge));
    origin_half_edge->start_vertex = 0;

    edges[0] = (edge *)malloc(sizeof(edge));
    edges[0]->half_edge = origin_half_edge;
    origin_half_edge->edge_index = 0;

    faces[0] = (face *)malloc(sizeof(face));
    faces[0]->origin = origin_half_edge;
    origin_half_edge->face_index = 0;


    //read in the first two coordinates and
    // prepare to join up the follwing coordinates

    double x_cor, y_cor;
    fscanf(fp1, "%lf %lf\n", &x_cor, &y_cor);


    half_edge *start_half_edge = origin_half_edge;
    half_edge *new_half_edge = NULL;

    do {
        //store each index for each vertice and store 
        //the following x and y values
        vertices = (vertex **) realloc(vertices, 
        (vertices_count+1) * sizeof(vertex*));

        vertices[vertices_count] = (vertex *)
        malloc(sizeof(vertex));

        vertices[vertices_count]->x = x_cor;
        vertices[vertices_count]->y = y_cor;
        vertices_count += 1;

        //create a new half edge to be able to connect the 
        //following points in the file
        new_half_edge = (half_edge *)malloc(sizeof(half_edge));
        start_half_edge->end_vertex = vertices_count - 1;
        start_half_edge->next = new_half_edge;
        start_half_edge->twin = NULL;
        new_half_edge->start_vertex = vertices_count - 1;
        new_half_edge->prev = start_half_edge;
        new_half_edge->face_index = 0;

        //create dges arrays and store the half edges 
        //to be able to easily access the faces
        edges = (edge **) realloc(edges, (edges_count+1)
         * sizeof(edge*));

        edges[edges_count] = (edge *)malloc(sizeof(edge));
        edges[edges_count]->half_edge = new_half_edge;
        new_half_edge->edge_index = edges_count;
        edges_count += 1;

        start_half_edge = new_half_edge;
    
    } while (fscanf(fp1, "%lf %lf\n", &x_cor, &y_cor) == 2);

    new_half_edge->next = origin_half_edge;
    new_half_edge->twin = NULL;
    new_half_edge->end_vertex = origin_half_edge->start_vertex;
    origin_half_edge->prev = new_half_edge;


    new_half_edge = NULL;
    int edge_1, edge_2;
    half_edge *half_edge1, *half_edge2, *new_twin_half_edge,
    *new_half_edge1, *new_half_edge2, *new_twin_half_edge1,
    *new_twin_half_edge2;

    //now read in the available splits and create each new half
    // edge and store this data
    while (fscanf(stdin,"%d %d\n", &edge_1, &edge_2) != EOF) {

        if (edges[edge_1]->half_edge->face_index == 
        edges[edge_2]->half_edge->face_index) {

            half_edge1 = edges[edge_1]->half_edge;
            half_edge2 = edges[edge_2]->half_edge;
        }
        if (edges[edge_1]->half_edge->twin != NULL) {

            if (edges[edge_1]->half_edge->twin->face_index == 
            edges[edge_2]->half_edge->face_index) {

                half_edge1 = edges[edge_1]->half_edge->twin;
                half_edge2 = edges[edge_2]->half_edge;
            }
        }
        if (edges[edge_2]->half_edge->twin != NULL) {
            if (edges[edge_2]->half_edge->twin->face_index == 
            edges[edge_1]->half_edge->face_index) {
                half_edge2 = edges[edge_2]->half_edge->twin;
                half_edge1 = edges[edge_1]->half_edge;
            }
        }
        if (edges[edge_1]->half_edge->twin != NULL && 
        edges[edge_2]->half_edge->twin != NULL) {
            
            if (edges[edge_1]->half_edge->twin->face_index == 
            edges[edge_2]->half_edge->twin->face_index) {
           
                half_edge1 = edges[edge_1]->half_edge->twin;
                half_edge2 = edges[edge_2]->half_edge->twin;
            }
        }


        //update each vertice array based on the given splits
        vertices = (vertex **) realloc(vertices, (vertices_count+2) * sizeof(vertex*));
        vertices[vertices_count] = (vertex *)malloc(sizeof(vertex));
        vertices[vertices_count+1] = (vertex *)malloc(sizeof(vertex));

        vertices[vertices_count]->x = (vertices[half_edge1->start_vertex]->x+vertices[half_edge1->end_vertex]->x)/2;
        vertices[vertices_count]->y = (vertices[half_edge1->start_vertex]->y+vertices[half_edge1->end_vertex]->y)/2;

        vertices[vertices_count+1]->x = (vertices[half_edge2->start_vertex]->x+vertices[half_edge2->end_vertex]->x)/2;
        vertices[vertices_count+1]->y = (vertices[half_edge2->start_vertex]->y+vertices[half_edge2->end_vertex]->y)/2;
        vertices_count += 2;

        new_half_edge = (half_edge *)malloc(sizeof(half_edge));
        new_half_edge->start_vertex = vertices_count - 2;
        new_half_edge->end_vertex = vertices_count - 1;

        new_half_edge->next = half_edge2;
        new_half_edge->prev = half_edge1;

        new_twin_half_edge = (half_edge *)malloc(sizeof(half_edge));
        new_twin_half_edge->end_vertex = vertices_count - 2;

        new_twin_half_edge->start_vertex = vertices_count - 1;
        new_twin_half_edge->twin = new_half_edge;
        new_half_edge->twin = new_twin_half_edge;

        faces = (face **) realloc(faces, (faces_count+1) * sizeof(face*));
        faces[faces_count] = (face *)malloc(sizeof(face));

        faces[faces_count]->origin = new_twin_half_edge;
        new_twin_half_edge->face_index = faces_count;
        faces_count += 1;

        new_half_edge1 = (half_edge *)malloc(sizeof(half_edge));
        new_half_edge1->start_vertex = vertices_count - 2;
        new_half_edge1->end_vertex = half_edge1->end_vertex;
        new_half_edge1->prev = new_twin_half_edge;
        new_twin_half_edge->next = new_half_edge1;

        new_half_edge2 = (half_edge *)malloc(sizeof(half_edge));
        new_half_edge2->start_vertex = half_edge2->start_vertex;
        new_half_edge2->end_vertex = vertices_count - 1;
        new_half_edge2->next = new_twin_half_edge;
        new_twin_half_edge->prev = new_half_edge2;

        edges = (edge **) realloc(edges, (edges_count+3) * sizeof(edge*));
        edges[edges_count] = (edge *)malloc(sizeof(edge));
        edges[edges_count+1] = (edge *)malloc(sizeof(edge));
        edges[edges_count+2] = (edge *)malloc(sizeof(edge));
        edges[edges_count]->half_edge = new_half_edge;
        edges[edges_count+1]->half_edge = new_half_edge1;
        edges[edges_count+2]->half_edge = new_half_edge2;
        new_half_edge->edge_index = edges_count;
        new_twin_half_edge->edge_index = edges_count;
        new_half_edge1->edge_index = edges_count+1;
        new_half_edge2->edge_index = edges_count+2;
        edges_count += 3;

        if (half_edge1->next == half_edge2) {
            new_half_edge1->next = new_half_edge2;
            new_half_edge2->prev = new_half_edge1;
        } else {
            new_half_edge1->next = half_edge1->next;
            half_edge1->next->prev = new_half_edge1;
            new_half_edge2->prev = half_edge2->prev;
            half_edge2->prev->next = new_half_edge2;
        }

        half_edge1->next = new_half_edge;
        half_edge1->end_vertex = vertices_count - 2;
        half_edge2->prev = new_half_edge;
        half_edge2->start_vertex = vertices_count - 1;


        //check this case for whether a half edge is coming off a twin
        if (half_edge1->twin != NULL) {

            new_twin_half_edge1 = (half_edge *)malloc(sizeof(half_edge));
            new_twin_half_edge1->start_vertex = half_edge1->twin->start_vertex;
            new_twin_half_edge1->end_vertex = vertices_count - 2;
            new_twin_half_edge1->next = half_edge1->twin;

            new_twin_half_edge1->prev = half_edge1->twin->prev;
            half_edge1->twin->prev->next = new_twin_half_edge1;
            new_twin_half_edge1->twin = new_half_edge1;

            new_twin_half_edge1->edge_index = new_half_edge1->edge_index;
            new_half_edge1->twin = new_twin_half_edge1;
            half_edge1->twin->start_vertex = vertices_count - 2;
            half_edge1->twin->prev= new_twin_half_edge1;

        }

        //check this case for whether a half edge is coming off a twin
        if (half_edge2->twin != NULL) {
            new_twin_half_edge2 = (half_edge *)malloc(sizeof(half_edge));
            new_twin_half_edge2->end_vertex = half_edge2->twin->end_vertex;

            new_twin_half_edge2->start_vertex = vertices_count - 2;
            new_twin_half_edge2->next = half_edge2->twin->next;
            half_edge2->twin->next->prev = new_twin_half_edge2;

            new_twin_half_edge2->prev = half_edge2->twin;
            new_twin_half_edge2->twin = new_half_edge2;
            new_twin_half_edge2->edge_index = new_half_edge2->edge_index;
            
            new_half_edge2->twin = new_twin_half_edge2;
            half_edge2->twin->end_vertex = vertices_count - 2;
            half_edge2->twin->next= new_twin_half_edge2;
        }

       
    
    }

    half_edge *traverse;
    for (int i = 0; i < faces_count; i++) {
            half_edge *origin = faces[i]->origin;
            traverse = origin;
            do {
                traverse->face_index = i;
                traverse = traverse->next;
            } while(traverse != origin);
    }
    



    FILE *ptr3;
    ptr3 = fopen(argv[3], "w");
    half_edge *traverse2;
    Watchtower *towers;
    half_edge *origin2;

    int face_array[faces_count];

    //now loop through each face and tower and check whether a watchtower_info
    //is in a given face
    for (int j = 0; j < faces_count; j++) {

        fprintf(ptr3, "%d\n", j);
        int population = 0;
            
        for (int i = 0; i < total_towers; i++) {

            towers = watchtower_info[i];

            origin2 = faces[j]->origin;
            traverse2 = origin2;
            
            //if a watchtower is in a face, print its contents
            if (prepare_output(towers, origin2, traverse2, vertices)) {

                    population += towers->pop_served;
                    fprintf(ptr3, "Watchtower ID: %s, Postcode: %s, Population Served: %d, Watchtower Point of Contact Name: %s, x: %lf, y: %lf\n", towers->ID, towers->postcode, towers->pop_served, towers->name, towers->watchtower_x, towers->watchtower_y);
            }

        }

        face_array[j] = population;
    
    }
    

        //free half edge memeory
        half_edge *temp;
        for (int i = 0; i < faces_count; i++) {
            half_edge *origin = faces[i]->origin;
            traverse = origin;
            do {
                temp = traverse;
                traverse = traverse->next;
                free(temp);
            } while(traverse != origin);
        }
    

    
    for (int k = 0; k < faces_count; k++) {

        fprintf(ptr3, "Face %d population Served: %d\n", k, face_array[k]);

    }

    //now free all the availabe memory and close all files
    for (int i =0; i < total_towers; i++) {
        free(watchtower_info[i]->ID);
        free(watchtower_info[i]->name);
        free(watchtower_info[i]->postcode);
        free(watchtower_info[i]);
     }


    free(watchtower_info);

    for (int i = 0; i < faces_count; i++) {
        free(faces[i]);
    }

    free(faces);

    
    for (int i = 0; i < vertices_count; i++) {
        free(vertices[i]);
        

    }

    free(vertices);

    for (int i = 0; i < edges_count; i++) {
        free(edges[i]);

    }

    free(edges);
    free(line);

    fclose(fp);
    fclose(fp1);
    fclose(ptr3);

    return 0;
}
