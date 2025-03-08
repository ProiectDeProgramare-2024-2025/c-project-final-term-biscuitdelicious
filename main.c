#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define PAIRS 20
#define STR_LEN 40


// Hard code un dictionar
struct Dictionary {
    char key[STR_LEN];
    char value[STR_LEN][STR_LEN];
};


// Clear the terminal screen
void clearScreen() {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    fflush(stdout);
}

void menuOne() {
    printf("Bun venit la MediConsult.\n");
    sleep(1);
    printf("_________________________"); // len 25
    printf("\nTe rog sa alegi din urmatoarele optiuni: \n\n");
    printf("(1) Vizualizeaza specialitatile disponibile \n(2) Iesi din aplicatie\n\n> ");
}

void specialtiesMenu(char specialties[][20], int size_list) {
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
    char listaSpecialitati[][20] = {
        "Cardiologie",
        "Pediatrie",
        "Chirurgie",
        "Dietetica",
        "Psihiatrie",
        "Urologie"
    };

    struct Dictionary dict[PAIRS] = {
        {"Dana Cristiana", "Cardiologie", "Pediatrie"},
        {"Vlad Pedrescu", "Urologie", "Psihiatrie", "Chirurgie"},
        {"Daniel Bordas", "Urologie"},
        {"Ionut Leahu", "Cardiologie"},
        {"Romeo Micu", "Chirurgie"}
    };

    int userOption;
    int sizeListaSpecialitati = sizeof(listaSpecialitati) / sizeof(listaSpecialitati[0]);


    while (true) {
        menuOne();
        scanf("%d", &userOption);

        if (userOption == 1) {
            specialtiesMenu(listaSpecialitati, sizeListaSpecialitati);
            scanf("%d", &userOption);

            if (userOption > sizeListaSpecialitati) {
                while (userOption > sizeListaSpecialitati) {
                    printf("Optiunea aleasa nu exista. Te rog sa alegi din nou.\n");
                    sleep(1);
                    clearScreen();
                    specialtiesMenu(listaSpecialitati, sizeListaSpecialitati);
                    scanf("%d", &userOption);
                }
            } else {
                for(int i = 0; i < PAIRS; i++) {
                    for(int j = 0; j < PAIRS; j++) {
                        if (strcmp(dict[i].value[j], listaSpecialitati[userOption - 1]) == 0) {
                            printf("%s\n", dict[i].key);
                        }
                    }
                }
            }


            getchar();
            getchar();
            clearScreen();

        } else if (userOption != 1 || userOption == 2) {
            break;
        }
    }
    return 0;
}


/* Specialitati v
 * Medici v
 * Doresti sa programezi o consultatie? Y/N
 * Y -> Numele, Prenumele, Telefonul, Specialitatea, intervalul orar(voi pune eu cand e liber medicul), data
 */
