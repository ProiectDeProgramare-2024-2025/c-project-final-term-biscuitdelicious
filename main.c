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
void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void menuOne() {
    printf("Bun venit la MediConsult.\n");
    sleep(1);
    printf("_________________________"); // len 25
    printf("\nTe rog sa alegi din urmatoarele optiuni: \n\n");
    printf("(1) Vizualizeaza specialitatile disponibile \n(2) Iesi din aplicatie\n");

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

    menuOne();
    int userOption = -1;
    scanf("%d", &userOption);

    if(userOption != 1 || userOption != 2) {
        printf("(X) Valoare invalida. Incearca din nou\n\n\n\n");
        sleep(2);
        menuOne();
    }

    return 0;
}


/* Specialitati v
 * Medici v
 * Doresti sa programezi o consultatie? Y/N
 * Y -> Numele, Prenumele, Telefonul, Specialitatea, intervalul orar(voi pune eu cand e liber medicul), data
 */
