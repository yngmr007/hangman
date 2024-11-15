// © by yngmr, 2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    char c;
    int count;
} char_count;

int random_in_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

char most_likely_char(char** db, int size, char* already_contained){
    if(db == NULL){
        printf("Error: db is NULL\n");
        exit(-1);
    }
    char_count most_likely[] = {{'a', 0},{'b',0},{'c',0},{'d',0},{'e',0},{'f',0},{'g',0},{'h',0},{'i',0},{'j',0},{'k',0},{'l',0},{'m',0},{'n',0},{'o',0},{'p',0},{'q',0},{'r',0},{'s',0},{'t',0},{'u',0},{'v',0},{'w',0},{'x',0},{'y',0},{'z',0}};
    for(int i = 0; i < size; i++){
        if(db[i] == NULL){
            printf("Error: db[%i] is NULL\n", i);
            exit(-1);
        }
        for(int j = 0; j < strlen(db[i]); j++){
            for(int k = 0; k < 26; k++){
                if(db[i][j] == most_likely[k].c){
                    most_likely[k].count++;
                }
            }
        }
    }
    int count_to_max = 0;
    char max;

    if(true){
        for(int i = 0; i < 26; i++){
            for(int j = 0; j < strlen(already_contained); j++){
                if(most_likely[i].c == already_contained[j]){
                    most_likely[i].count = 0;
                }
            }
        }
    }

    for(int i = 0; i < 26; i++){
        if(most_likely[i].count > count_to_max){
            count_to_max = most_likely[i].count;
            max = most_likely[i].c;
        }
    }
    return max;
}

void ascii_art_hangman(int zug){
    if (zug == 0){
        printf("\n\n\n\n\n\n");
        printf("= = = = = =\n");
    }else if (zug == 1){
        printf("  +\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 2){
        printf("  +-----+\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 3){
        printf("  +-----+\n");
        printf("  | /\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 4){
        printf("  +-----+\n");
        printf("  | /   |\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 5){
        printf("  +-----+\n");
        printf("  | /   |\n");
        printf("  |     o\n");
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 6){
        printf("  +-----+\n");
        printf("  | /   |\n");
        printf("  |     o\n");
        printf("  |     |\n");
        printf("  |\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 7){
        printf("  +-----+\n");
        printf("  | /   |\n");
        printf("  |     o\n");
        printf("  |    \\|/\n");
        printf("  |\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 8){
        printf("  +-----+\n");
        printf("  | /   |\n");
        printf("  |     o\n");
        printf("  |    \\|/\n");
        printf("  |    / \\\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }
    else if (zug == 9){
        printf("  +-----+\n");
        printf("  | /   |\n");
        printf("  |     \033[31mx\033[0m\n");
        printf("  |    \\|/\n");
        printf("  |    / \\\n");
        printf("  |\n");
        printf("= = = = = =\n");
    }

}

char** read_db(int* word_count, char *lng) {
    char filename[] = "./wortliste_aa.txt";
    filename[12] = lng[0];
    filename[13] = lng[1];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error in opening file\n");
        exit(ENOMEM);
    }

    fseek(file, 0, SEEK_END);
    int db_size_bytes = ftell(file);  // Gesamtgröße der Datei
    printf("File size: %i bytes\n", db_size_bytes);

    fseek(file, 0, SEEK_SET);
    char *db = malloc((db_size_bytes + 1) * sizeof(char)); // +1 für Null-Terminierung
    if (db == NULL) {
        printf("Memory allocation failed\n");
        exit(ENOMEM );
    }

    fread(db, sizeof(char), db_size_bytes, file);
    db[db_size_bytes] = '\0'; // Null-terminate the buffer

    fclose(file);

    // Zählen der Zeilen
    int line_count = 0;
    for (int i = 0; i < db_size_bytes; i++) {
        if (db[i] == '\n') {
            line_count++;
        }
    }

    // Speicher für das Array der Zeiger allokieren
    char **ptr_db_new = malloc((line_count + 1) * sizeof(char*)); // +1 für mögliche letzte Zeile ohne \n
    if (ptr_db_new == NULL) {
        printf("Memory allocation for ptr_db_new failed\n");
        free(db);
        exit(ENOMEM);
    }

    int k = 0; // Index für ptr_db_new
    ptr_db_new[k++] = db; // Erstes Wort beginnt am Anfang von db

    // Zeiger auf den Anfang jeder Zeile zuweisen
    for (int i = 0; i < db_size_bytes; i++) {
        if (db[i] == '\n') {
            db[i] = '\0'; // Null-terminate the current word
            if (i + 1 < db_size_bytes) {
                ptr_db_new[k++] = &db[i + 1]; // Nächstes Wort beginnt nach dem Newline
            }
        }
    }
    ptr_db_new[k] = NULL; // Null-terminate the pointer array

    *word_count = line_count;
    return ptr_db_new;
}
int* check_containment(char c, const char* wort, int* return_length) {
    int len = strlen(wort);
    int* positions = (int*)malloc(len * sizeof(int));
    int count = 0;

    for(int i = 0; i < len; i++) {
    	if(wort[i] == c){
          	positions[count] = i;
            count++;
    	}
    }
    *return_length = count;
    return positions;
}

void zeug_fuer_den_pc(char **db1, char **db2, int length) {
    // erste manipulation auf der db, deshalb wird getestet ob die db1/2 inhalte haben
    if (db1 == NULL || db2 == NULL) {
        printf("Error: db1 or db2 is NULL\n");
        exit(-1);
    }

    int line_count = 0;
    while (db1[line_count] != NULL) {
        line_count++;
    }

    //testet, ob das wort die richtige länge hat und fügt ggf zu db2 hinzu
    int rest = 0;
    for (int i = 0; i < line_count; i++) {
        if (strlen(db1[i]) == length + 1){
            db2[rest] = db1[i];
            rest++;
        } else {
            db2[i] = NULL;
        }
    }

    //setze db1 auf NULL zurück
    for(int i = 0; i < line_count; i++){
        db1[i] = NULL;
    }

    //kopiere db2 in db1
    for (int i=0; i<rest; i++){
        db1[i] = db2[i];
    }
}

void zeug_fuer_den_pc2(char **db1, char **db2, int length, char contain, int* containat, int returnlength) {
    int stelle = 0;
    int line_count = 0;
    bool warheitswert = false;

    while (db1[line_count] != NULL) {
        line_count++;
    }

    //wenn returnlength 0, dann ist der getestete buchstabe (contain) NICHT vorhanden
    if (returnlength == 0) {
        for (int i = 0; i < line_count; i++){
            warheitswert = true;

             //testet, ob der buchsttabe im wort vorkommt für jedes wort in der db1
            for (int j = 0; j < strlen(db1[i]); j++){
                if(db1[i][j] == contain){
                    warheitswert = false; 
                }
            }
             //nur falls der buchstabe nicht vorhanden ist in einem wort, wird es gespeichert
            if (warheitswert == true){
                db2[stelle] = db1[i];
                stelle++;
            }
        }
    } else {
        for (int i = 0; i < line_count; i++){
            warheitswert = false;

            //testet falls der Buchstabe an der stelle vorkommt, an der er muss und an KEINER anderen
            for (int j = 0; j < returnlength; j++){
                if (db1[i][containat[j]] == contain){
                    warheitswert = true;
                } else {
                    warheitswert = false;
                    break;
                }
            }

            //nur wenn der buchstabe an der stelle(n) vorkommt, wird er gespeichert
            if (warheitswert == true){
                db2[stelle] = db1[i];
                stelle++;
            }

        }
    }

    //setze db1 auf NULL zurück (leeren aber nicht neu erstellen)
    for(int i = 0; i < length; i++){
        db1[i] = NULL;
    }

    //kopiere alle werte von db2 in db1
    for (int i=0; i<stelle; i++){
        db1[i] = db2[i];
    }    

    printf("moegliche woerter: %i ", stelle);
    if(stelle == 1){
        printf(" -> das wort ist: %s\n", db1[0]);
    }
}


void game_starter(char** db, int word_count, char* lng, bool assist) { 
    int zufallszahl = random_in_range(0, word_count - 1); // Dynamische Anpassung der Anzahl der Wörter
    char* zufallswort = db[zufallszahl];

    //zwei weitere Arrays zum arbeiten, das das original-Array mit allen wörtern nie manipuliert wird
    char** array1 = (char**)malloc(word_count * sizeof(char*));
    char** array2 = (char**)malloc(word_count * sizeof(char*));

    for (int i = 0; i < word_count; i++) {
        array1[i] = strdup(db[i]);
        array2[i] = strdup(db[i]);
    }

    int zuege = 0;
    int max_zuege = 9;
    int return_length = 0;

    bool gewonnen = false;

    char buchstabe_geraten;
    char falsche_buchstaben[10];
    char buchstabe_geraten_array[26];
    
    char *leeres_wort = (char*)malloc(strlen(zufallswort) * sizeof(char));

    for(int i = 0; i < strlen(zufallswort); i++) {
        leeres_wort[i] = '_';
    }

    printf("\e[1;1H\e[2J");
    printf("    ####    GAME START    ####\n");
    printf("Zufaelliges Wort: %s, size: %i \n", zufallswort, strlen(zufallswort));
    
    if (assist) zeug_fuer_den_pc(array1, array2, strlen(zufallswort)-1);

    while (zuege < max_zuege) {
        //abbruchbedingung sollte das spiel VOR den maximalen zügen beenden, sollte gewonnen sein
        if(gewonnen) break;
        
        char input[26];

        ascii_art_hangman(zuege);

        //gibt die buchstaben aus in nutzerIface
        for(int i = 0; i < strlen(zufallswort); i++) {
            putchar(leeres_wort[i]);
            putchar(' ');
        }
        
        //gibt die falsch eingegebenen buchstaben aus in rot und stellt die farbe wieder zurück auf std
        printf("\033[31m   ");
        for(int i = 0; i < zuege; i++) {
            putchar(falsche_buchstaben[i]);
            putchar(' ');
        }
        printf("\033[0m"); 

        printf("\nrestliche fehlversuche: %i\n", max_zuege-zuege);

        int line_count = 0;
        while (array1[line_count] != NULL) {
            line_count++;
        }

        //nutzereingabe
        printf("dein buchstabe");

        //gibt tipp aus, wenn dieser bei start aktiviert wurde
        if (assist) printf(" (tipp: %c)", most_likely_char(array1, line_count, leeres_wort));

        printf(": ");

        //liest nutzereingabe
        fgets(input, 26, stdin);
        sscanf(input, "%s", &buchstabe_geraten_array);

        //löscht terminal und zeichnet fenster wieder neu
        printf("\e[1;1H\e[2J");
        printf("    ####    GAME START    ####\n");

        //testet, ob nutzer einen buchstaben oder ein wort eingetippt hat
        if (buchstabe_geraten_array[1] != 0) {
            if(strcmp(buchstabe_geraten_array, zufallswort) == 0) {
                gewonnen = true;
            } else {
                printf("das wort ist nicht %s!\n", buchstabe_geraten_array);
                zuege++;
            }

        } else {
            
            buchstabe_geraten = buchstabe_geraten_array[0];
            int* positionen = check_containment(buchstabe_geraten, zufallswort, &return_length);
            if (assist) zeug_fuer_den_pc2(array1, array2, word_count, buchstabe_geraten, positionen, return_length);

            if (return_length == 0) {
                falsche_buchstaben[zuege] = buchstabe_geraten;
                printf("Der buchstabe %c ist nicht enthalten!\n", buchstabe_geraten);
                zuege++;
            } else {
                putchar('\n');
                for(int i = 0; i < return_length; i++) {
                    leeres_wort[positionen[i]] = buchstabe_geraten;
                }
            }

            for (int i = 0; i < strlen(zufallswort); i++) {
                if (leeres_wort[i] == '_') {
                    break;
                }

                if (i == strlen(zufallswort) - 1) {
                    gewonnen = true;
                }
            }
        }
    }
    if(!gewonnen){
        printf("\033[31mdu hast verloren! das wort das du suchst war: %s\033[0m\n", zufallswort);
    }else{
        printf("\033[32mdu hast gewonnen! das wort das du suchst war: %s\033[0m\n", zufallswort);
    }

    ascii_art_hangman(zuege);
    for(int i = 0; i < strlen(zufallswort); i++) {
        putchar(leeres_wort[i]);
        putchar(' ');
    }
    printf("\n");
    printf("restliche fehlversuche: %i\n", max_zuege-zuege);
    printf(" - - - - - - - - - - - - - - - -");

    free(leeres_wort);
    for (int i = 0; i < word_count; i++) {
        free(array1[i]);
        free(array2[i]);
    }
    free(array1);
    free(array2);
}

void help(){
    printf("Hilfe fuer Hangman\n-hilfe: zeigt diese Hilfe an\n-info: zeigt informationen ueber das Spiel an, zb quellen\n-assist: aktiviere spiele-assitenten\n-sprache (arg): startet das spiel in der sprache (arg), zb (de)\n");
}

void info_fun(){
    printf("code by yngmr (2024).\nlanguage dictonary (de) from https://codeberg.org/davidak/wortliste/src/branch/master/wortliste.txt\n");
}

int main(int argc, char *argv[]) {
    int sprache = 0;
    bool info = false;
    bool hilfe = false;
    bool assist = false;
    char *sprache_wert = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-sprache") == 0) {
            sprache = 1;
            if (i + 1 < argc) {
                if(argv[i+1][0] != '-'){
                    i++;
                    sprache_wert = argv[i];
                } else{
                    sprache_wert = "de"; //default wert für die sprache, wenn keine sprache angegeben wird aber der command angegeben
                }
            }
        } else if (strcmp(argv[i], "-hilfe") == 0) {
            hilfe = true;
        } else if (strcmp(argv[i], "-info") == 0){
            info = true;
        } else if (strcmp(argv[i], "-assist") == 0) {
            assist = true;
        } else if (strcmp(argv[i], "-sprache") != 0 
        && strcmp(argv[i], "-hilfe") != 0 
        && strcmp(argv[i], "-assist") != 0 
        && strcmp(argv[i], "-info") != 0) {
            printf("Unbekannter Parameter: %s\n", argv[i]);
        }
    }
    if(sprache == 0) sprache_wert = "de"; //setze sprache auf default wenn keine sprache angegeben wird
    if(hilfe){ help(); return 0;} //comamnds , nachdenen der code beendet wird und das spiel nicht gestartet
    if(info){ info_fun(); return 0;}

    srand(time(NULL)); //random time funktion initialisieren, noch nicht wirklich random

    int word_count = 0;
    char* lng = sprache_wert;
        
    char** db_new = read_db(&word_count, lng); 

    if (word_count > 0) {
        game_starter(db_new, word_count, lng, assist); 
    } else {
        printf("\033[31mDie Wortliste ist leer!\033[0m\n");
    }

    free(db_new[0]); 
    free(db_new); 

    return 0;
}