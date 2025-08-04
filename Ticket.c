#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Ticket {
    int pnr;
    char name[50];
    int age;
    char train_name[50];
    char source[30];
    char destination[30];
};

void view_trains() {
    printf("\nAvailable Trains:\n");
    printf("1. Chennai Express - Chennai to Bangalore\n");
    printf("2. Mumbai Express - Mumbai to Delhi\n");
    printf("3. Kolkata Express - Kolkata to Hyderabad\n");
}

int generate_pnr() {
    FILE *file = fopen("pnr.txt", "r+");
    int pnr;
    if (file == NULL) {
        file = fopen("pnr.txt", "w");
        pnr = 1000;
    } else {
        fscanf(file, "%d", &pnr);
        pnr++;
        rewind(file);
    }
    fprintf(file, "%d", pnr);
    fclose(file);
    return pnr;
}

void book_ticket() {
    FILE *file = fopen("tickets.txt", "a");
    struct Ticket t;

    t.pnr = generate_pnr();

    printf("Enter your name: ");
    scanf(" %[^\n]", t.name);
    printf("Enter your age: ");
    scanf("%d", &t.age);
    
    view_trains();
    int choice;
    printf("Choose train (1-3): ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            strcpy(t.train_name, "Chennai Express");
            strcpy(t.source, "Chennai");
            strcpy(t.destination, "Bangalore");
            break;
        case 2:
            strcpy(t.train_name, "Mumbai Express");
            strcpy(t.source, "Mumbai");
            strcpy(t.destination, "Delhi");
            break;
        case 3:
            strcpy(t.train_name, "Kolkata Express");
            strcpy(t.source, "Kolkata");
            strcpy(t.destination, "Hyderabad");
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    fwrite(&t, sizeof(t), 1, file);
    fclose(file);
    printf("Ticket booked successfully! Your PNR: %d\n", t.pnr);
}

void view_ticket() {
    FILE *file = fopen("tickets.txt", "r");
    struct Ticket t;
    int search_pnr, found = 0;
    printf("Enter PNR to search: ");
    scanf("%d", &search_pnr);

    while(fread(&t, sizeof(t), 1, file)) {
        if (t.pnr == search_pnr) {
            printf("\n--- Ticket Details ---\n");
            printf("PNR: %d\n", t.pnr);
            printf("Name: %s\n", t.name);
            printf("Age: %d\n", t.age);
            printf("Train: %s\n", t.train_name);
            printf("From: %s\n", t.source);
            printf("To: %s\n", t.destination);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found)
        printf("No ticket found with PNR %d\n", search_pnr);
}

void cancel_ticket() {
    FILE *file = fopen("tickets.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Ticket t;
    int cancel_pnr, found = 0;
    printf("Enter PNR to cancel: ");
    scanf("%d", &cancel_pnr);

    while(fread(&t, sizeof(t), 1, file)) {
        if (t.pnr == cancel_pnr) {
            printf("Ticket with PNR %d cancelled.\n", cancel_pnr);
            found = 1;
        } else {
            fwrite(&t, sizeof(t), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);
    remove("tickets.txt");
    rename("temp.txt", "tickets.txt");
    if (!found)
        printf("PNR not found.\n");
}

int main() {
    int choice;
    do {
        printf("\n--- Railway Reservation System ---\n");
        printf("1. View Trains\n");
        printf("2. Book Ticket\n");
        printf("3. View Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: view_trains(); break;
            case 2: book_ticket(); break;
            case 3: view_ticket(); break;
            case 4: cancel_ticket(); break;
            case 5: printf("Exiting system.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(choice != 5);
    return 0;
}
