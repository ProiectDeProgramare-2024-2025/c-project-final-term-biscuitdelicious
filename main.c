#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/*
*Utilizatorul aplicației poate vizualiza medicii și specialitățile disponibile și poate programa o consultație. DONE
Aplicația permite adăugarea/ștergerea unei programări pentru un anumit pacient cu nume, căutarea unui medic după nume, specialitate și program, programarea consultației și anularea consultației.
Pentru a efectua programarea pentru o consultație utilizatorul trebuie sa specifice numele, prenumele, telefonul, specialitatea, intervalul orar, ziua și data.

 */

// TODO: Crearea unei programari

#define PAIRS 20
#define STR_LEN 40
#define MAX_DAYS 7
#define MAX_SLOTS 5
#define MAX_SPECIALTIES 5



// Hard code un dictionar
struct Dictionary {
    char key[STR_LEN];
    char value[STR_LEN][STR_LEN];
};


typedef struct {
    char name[STR_LEN];
    char surname[STR_LEN];
    char specialties[MAX_SPECIALTIES][STR_LEN];
    char phoneNumber[STR_LEN];

    // True - available, else False
    bool schedule[MAX_DAYS][MAX_SLOTS];

    // "09:00 - 10:00"
    char timeSlots[MAX_SLOTS][STR_LEN];
} Doctor;

// Clear the terminal screen
void clearScreen() {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    fflush(stdout);
}

// Show Welcome Menu
void menuOne() {
    printf("Bun venit la MediConsult.\n");
    sleep(1);
    printf("_________________________"); // len 25
    printf("\nTe rog sa alegi din urmatoarele optiuni: \n\n");
    printf("(1) Vizualizeaza specialitatile disponibile \n(2) Iesi din aplicatie\n\n> ");
}

// Show Specialties Menu
void specialtiesMenu(char specialties[][STR_LEN], int size_list) {
    clearScreen();
    printf("Vizualizaza specialitatile disponibile\n");
    printf("_________________________\n");
    sleep(1);

    for(int i = 0; i < size_list; i++) {
        printf("%d. %s\n", i + 1, specialties[i]);
    }
    printf("\nPentru a alege medicii disponibili, alege una din specialitati.");
    printf("\n> ");
}




int main() {
    const char* timeSlots[MAX_SLOTS] = {
        "09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"
    };

    const char* dayNames[MAX_DAYS] = {
        "Luni", "Marti", "Miercuri", "Joi", "Vineri", "Sambata", "Duminica"
    };

    char listaSpecialitati[][STR_LEN] = {
        "Cardiologie",
        "Pediatrie",
        "Chirurgie",
        "Dietetica",
        "Psihiatrie",
        "Urologie"
    };
    int sizeListaSpecialitati = sizeof(listaSpecialitati) / sizeof(listaSpecialitati[0]);

    Doctor doctors[] = {
        {
            .name = "Dana",
            .surname = "Cristiana",
            .phoneNumber = "0766-901-234",
            .specialties = {"Cardiologie", "Pediatrie"},
            .schedule = {
                {true, true, false, true, false},
                {true, true, false, false, false},
                {false, false, true, true, false},
                {true, false, false, true, true},
                {false, false, false, true, true},
                {false, false, false, false, false},
                {false, false, false, false, false}
            },
            .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
        },
        {
            .name = "Vlad",
            .surname = "Pedrescu",
            .phoneNumber = "0755-345-678",
            .specialties = {"Urologie", "Psihiatrie", "Chirurgie"},
            .schedule = {
                {false, false, true, true, true},
                {true, true, true, false, false},
                {false, false, false, true, true},
                {true, true, false, false, false},
                {false, false, true, true, false},
                {true, true, false, false, false},
                {false, false, false, false, false}
            },
            .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
        },
        {
            .name = "Daniel",
            .surname = "Bordas",
            .phoneNumber = "0744-789-012",
            .specialties = {"Urologie"},
            .schedule = {
                {true, false, false, true, true},
                {false, false, false, false, false},
                {true, true, true, false, false},
                {false, false, true, true, true},
                {true, true, false, false, false},
                {false, false, false, true, true},
                {false, false, false, false, false}
            },
            .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
        },
        {
            .name = "Ionut",
            .surname = "Leahu",
            .phoneNumber = "0733-123-456",
            .specialties = {"Cardiologie"},
            .schedule = {
                {false, false, false, false, false},
                {true, true, true, false, false},
                {false, false, false, true, true},
                {true, true, false, false, false},
                {false, false, true, true, true},
                {false, false, false, false, false},
                {true, true, false, false, false}
            },
            .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
        },
        {
            .name = "Romeo",
            .surname = "Micu",
            .phoneNumber = "0722-456-789",
            .specialties = {"Chirurgie"},
            .schedule = {
                {true, true, true, false, false},
                {false, false, false, true, true},
                {true, true, false, false, false},
                {false, false, true, true, false},
                {true, false, false, true, true},
                {true, true, false, false, false},
                {false, false, false, false, false}
            },
            .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
        }
    };
    int doctorsCount = sizeof(doctors) / sizeof(doctors[0]);

    int userOption;
    char userChoice;

    while (true) {
        menuOne();
        scanf("%d", &userOption);
        getchar();

        if (userOption == 1) {
            specialtiesMenu(listaSpecialitati, sizeListaSpecialitati);
            scanf("%d", &userOption);
            getchar();

            while (userOption < 1 || userOption > sizeListaSpecialitati) {
                printf("Optiunea aleasa nu exista. Te rog sa alegi din nou.\n");
                sleep(1);
                clearScreen();
                specialtiesMenu(listaSpecialitati, sizeListaSpecialitati);
                scanf("%d", &userOption);
                getchar();
            }

            clearScreen();
            char* selectedSpecialty = listaSpecialitati[userOption - 1];
            int count = 0;

            printf("Medici disponibili pentru specialitatea %s:\n", selectedSpecialty);
            printf("_________________________\n");

            for (int i = 0; i < doctorsCount; i++) {
                for (int j = 0; j < MAX_SPECIALTIES; j++) {
                    if (doctors[i].specialties[j][0] != '\0' &&
                        strcmp(selectedSpecialty, doctors[i].specialties[j]) == 0) {
                        printf("%d. %s %s - Tel: %s\n", ++count,
                               doctors[i].name, doctors[i].surname, doctors[i].phoneNumber);
                    }
                }
            }

            if (count == 0) {
                printf("Nu exista medici disponibili pentru aceasta specializare.\n");
                sleep(1);
                printf("Doresti sa selectezi din nou o specialitate? (Y/N) ");
                scanf("%c", &userChoice);

                if (userChoice == 'Y' || userChoice == 'y') {
                    continue;
                }
            } else {
                printf("\nDoresti sa programezi o consultatie? (Y/N) ");
                scanf("%c", &userChoice);

                if (userChoice == 'Y' || userChoice == 'y') {
                    printf("Functionalitatea de programare nu este inca implementata.\n");
                    sleep(2);
                }
            }

            printf("\nApasa [ENTER] pentru a continua\n");
            getchar();
            if (userChoice != 'Y' && userChoice != 'y') {
                getchar();

            }
            clearScreen();
        } else if (userOption == 2) {
            printf("La revedere!\n");
            break;
        } else {
            printf("Optiune invalida. Te rog sa alegi din nou.\n");
            sleep(1);
        }
    }
    return 0;
}

/* Specialitati v
 * Medici v
 * Doresti sa programezi o consultatie? Y/N
 */
