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
            clearScreen();
            printf("Vizualizaza specialitatile disponibile\n");
            printf("_________________________\n");
            sleep(1);

            for(int i = 0; i < sizeListaSpecialitati; i++) {
                printf("%d. %s\n", i + 1, listaSpecialitati[i]);
            }
            printf("\n> ");
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
