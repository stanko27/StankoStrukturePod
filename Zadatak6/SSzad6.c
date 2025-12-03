/*6. Napisati program koji čita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini račun.Na početku svake datoteke je zapisan datum u kojem vremenu je
račun izdat u formatu YYYY - MM - DD.Svaki sljedeći red u datoteci predstavlja artikl u formatu
naziv, količina, cijena.Potrebno je formirati vezanu listu računa sortiranu po datumu.Svaki čvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla.Nakon toga potrebno je
omogućiti upit kojim će korisnik saznati koliko je novaca sveukupno potrošeno na specifični
artikl u određenom vremenskom razdoblju i u kojoj je količini isti kupljen. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF 256

/* struktura za artikl */
typedef struct Item {
    char name[100];
    int qty;
    float price;
    struct Item* next;
} Item;

/* struktura za račun */
typedef struct Receipt {
    char date[11];  /* yyyy-mm-dd */
    Item* items;    /* lista artikala */
    struct Receipt* next;
} Receipt;

/* početak liste računa */
Receipt* head = NULL;

/* trim - uklanjanje razmaka i \n, vraća pokazivač na početak stringa */
char* trim(char* s) {
    if (!s) return NULL;

    char* start = s;
    while (*start == ' ' || *start == '\t') start++;

    char* end = start + strlen(start);
    while (end > start && (end[-1]==' ' || end[-1]=='\t' || end[-1]=='\n' || end[-1]=='\r')) end--;

    *end = '\0';
    return start;
}

/* dodavanje artikla sortirano po nazivu, vraća 0 ako uspješno */
int add_item_sorted(Item** item_head, const char* name_in, int qty, float price) {
    if (!item_head || !name_in) return -1;

    Item* prev = NULL;
    Item* cur = *item_head;

    while (cur && strcmp(cur->name, name_in) < 0) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && strcmp(cur->name, name_in) == 0) {
        cur->qty += qty;
        return 0;
    }

    Item* it = malloc(sizeof(Item));
    if (!it) return -1;

    strcpy(it->name, name_in);
    it->qty = qty;
    it->price = price;
    it->next = cur;

    if (prev) prev->next = it;
    else *item_head = it;

    return 0;
}

/* dodavanje računa sortirano po datumu, vraća 0 ako uspješno */
int add_receipt_sorted(Receipt* r) {
    if (!r) return -1;

    if (!head || strcmp(head->date, r->date) > 0) {
        r->next = head;
        head = r;
        return 0;
    }

    Receipt* cur = head;
    while (cur->next && strcmp(cur->next->date, r->date) <= 0)
        cur = cur->next;

    r->next = cur->next;
    cur->next = r;
    return 0;
}

/* čitanje jednog računa iz datoteke, vraća pointer na Receipt ili NULL */
Receipt* parse_receipt_file(const char* filename) {
    if (!filename) return NULL;

    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    char line[LINE_BUF];
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return NULL;
    }

    char* date_line = trim(line);

    Receipt* r = malloc(sizeof(Receipt));
    if (!r) { fclose(f); return NULL; }

    strncpy(r->date, date_line, 10);
    r->date[10] = '\0';
    r->items = NULL;
    r->next = NULL;

    while (fgets(line, sizeof(line), f)) {
        char* ln = trim(line);
        if (strlen(ln) == 0) continue;

        char* name = strtok(ln, ",");
        char* qtys = strtok(NULL, ",");
        char* prices = strtok(NULL, ",");

        if (!name || !qtys || !prices) continue;

        name = trim(name);
        qtys = trim(qtys);
        prices = trim(prices);

        int qty = atoi(qtys);
        float price = atof(prices);

        add_item_sorted(&r->items, name, qty, price);
    }

    fclose(f);
    return r;
}

/* učitava sve račune iz racuni.txt, vraća 0 ako uspješno */
int load_receipts(const char* list_filename) {
    if (!list_filename) return -1;

    FILE* f = fopen(list_filename, "r");
    if (!f) return -1;

    char line[LINE_BUF];
    while (fgets(line, sizeof(line), f)) {
        char* ln = trim(line);
        if (strlen(ln) == 0) continue;

        Receipt* r = parse_receipt_file(ln);
        if (r) add_receipt_sorted(r);
    }

    fclose(f);
    return 0;
}

/* izračun potrošnje za artikl u intervalu datuma, vraća ukupnu cijenu */
float total_spent(const char* item_name, const char* date_from, const char* date_to, int* out_qty) {
    if (!item_name || !date_from || !date_to || !out_qty) return 0;

    float total = 0;
    int qtysum = 0;

    Receipt* r = head;
    while (r) {
        if (strcmp(r->date, date_from) >= 0 && strcmp(r->date, date_to) <= 0) {
            Item* it = r->items;
            while (it) {
                if (strcmp(it->name, item_name) == 0) {
                    qtysum += it->qty;
                    total += it->qty * it->price;
                }
                it = it->next;
            }
        }
        r = r->next;
    }

    *out_qty = qtysum;
    return total;
}

/* oslobađanje memorije, vraća 0 ako uspješno */
int free_all() {
    Receipt* r = head;
    while (r) {
        Item* it = r->items;
        while (it) {
            Item* nit = it->next;
            free(it);
            it = nit;
        }
        Receipt* nr = r->next;
        free(r);
        r = nr;
    }
    head = NULL;
    return 0;
}

/* glavni program */
int main() {
    if (load_receipts("racuni.txt") != 0) {
        printf("greska: ne mogu otvoriti racuni.txt\n");
        return 1;
    }

    char item[100];
    char from[11];
    char to[11];

    printf("unesi naziv artikla: ");
    if (!fgets(item, sizeof(item), stdin)) { free_all(); return 0; }
    trim(item);

    printf("unesi datum od (yyyy-mm-dd): ");
    if (!fgets(from, sizeof(from), stdin)) { free_all(); return 0; }
    trim(from);

    printf("unesi datum do (yyyy-mm-dd): ");
    if (!fgets(to, sizeof(to), stdin)) { free_all(); return 0; }
    trim(to);

    int qty = 0;
    float total = total_spent(item, from, to, &qty);

    printf("ukupno kupljeno %d kom, ukupno potroseno %.2f\n", qty, total);

    free_all();
    return 0;
}