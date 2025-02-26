#include <stdio.h>
#include <string.h>

#define PAIRS 20
#define STR_LEN 20

struct Dictionary {
    char key[STR_LEN];
    char value[STR_LEN];
};

int main() {
    char listaSpecialitati[][20] = {
        'Cardiologie',
        'Pediatrie',
        'Chirurgie',
        'Dietetica',
        'Psihiatrie',
        'Urologie'
    };
}


/* Specialitati
 * Medici
 * Doresti sa programezi o consultatie? Y/N
 * Y -> Numele, Prenumele, Telefonul, Specialitatea, intervalul orar(voi pune eu cand e liber medicul), data
 */
