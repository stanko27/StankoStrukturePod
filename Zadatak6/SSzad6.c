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


/* strukture za artikle i racune */
typedef struct Item {
    char name[100];
    int qty;
    float price;
    struct Item* next;
} Item;

typedef struct Receipt {
    char date[11]; /* yyyy-mm-dd */
    Item* items;
    struct Receipt* next;
} Receipt;

/* pokazivac na pocetak liste racuna */
Receipt* head = NULL;

/* pomocna funkcija za dupliciranje stringa */
char* my_strdup(const char* s) {
    size_t n = strlen(s) + 1;
    char* p = (char*)malloc(n);
    if (p) strcpy(p, s);
    return p;
}

/* uklanjanje razmaka na pocetku i kraju */
int trim(char* s) {
    if (!s) return 0;
    char* start = s;
    while (*start == ' ' || *start == '\t') start++;
    char* end = start + strlen(start);
    while (end > start && (*(end - 1) == ' ' || *(end - 1) == '\t' || *(end - 1) == '\n' || *(end - 1) == '\r'))
        end--;
    size_t len = (size_t)(end - start);
    memmove(s, start, len);
    s[len] = '\0';
    return 0;
}

/* dodavanje artikla sortirano po nazivu; ako postoji isti, povecava kolicinu */
int add_item_sorted(Item** item_head, const char* name_in, int qty, float price) {
    char namebuf[100];
    strncpy(namebuf, name_in, sizeof(namebuf) - 1);
    namebuf[sizeof(namebuf) - 1] = '\0';
    trim(namebuf);

    Item* prev = NULL;
    Item* cur = *item_head;

    while (cur && strcmp(cur->name, namebuf) < 0) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && strcmp(cur->name, namebuf) == 0) {
        cur->qty += qty;
        return 0;
    }

    Item* it = (Item*)malloc(sizeof(Item));
    strncpy(it->name, namebuf, sizeof(it->name) - 1);
    it->name[sizeof(it->name) - 1] = '\0';
    it->qty = qty;
    it->price = price;
    it->next = cur;

    if (prev) prev->next = it;
    else *item_head = it;

    return 0;
}

/* dodavanje racuna sortirano po datumu */
int add_receipt_sorted(Receipt* r) {
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

/* citanje jednog racuna iz datoteke */
Receipt* parse_receipt_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    char line[LINE_BUF];
    if (!fgets(line, sizeof(line), f)) { fclose(f); return NULL; }
    trim(line);

    Receipt* r = (Receipt*)malloc(sizeof(Receipt));
    strncpy(r->date, line, 10);
    r->date[10] = '\0';
    r->items = NULL;
    r->next = NULL;

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strlen(line) == 0) continue;

        char buf[LINE_BUF];
        strncpy(buf, line, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';

        char* token = strtok(buf, ",");
        if (!token) continue;
        char name[100];
        strncpy(name, token, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        trim(name);

        token = strtok(NULL, ",");
        if (!token) continue;
        trim(token);
        int qty = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        trim(token);
        float price = (float)atof(token);

        add_item_sorted(&r->items, name, qty, price);
    }

    fclose(f);
    return r;
}

/* ucitavanje svih racuna navedenih u racuni.txt */
int load_receipts(const char* list_filename) {
    FILE* f = fopen(list_filename, "r");
    if (!f) return -1;

    char line[LINE_BUF];
    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strlen(line) == 0) continue;
        Receipt* r = parse_receipt_file(line);
        if (r) add_receipt_sorted(r);
    }

    fclose(f);
    return 0;
}

/* racunanje ukupne kolicine i cijene unutar intervala datuma */
float total_spent(const char* item_name_in, const char* date_from, const char* date_to, int* out_qty) {
    char target[100];
    strncpy(target, item_name_in, sizeof(target) - 1);
    target[sizeof(target) - 1] = '\0';
    trim(target);

    float total = 0;
    int qtysum = 0;

    Receipt* r = head;
    while (r) {
        if (strcmp(r->date, date_from) >= 0 && strcmp(r->date, date_to) <= 0) {
            Item* it = r->items;
            while (it) {
                if (strcmp(it->name, target) == 0) {
                    qtysum += it->qty;
                    total += it->qty * it->price;
                }
                it = it->next;
            }
        }
        if (strcmp(r->date, date_to) > 0) break;
        r = r->next;
    }

    *out_qty = qtysum;
    return total;
}

/* oslobadanje memorije */
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

    printf("unesi naziv artikla (enter za kraj): ");
    if (!fgets(item, sizeof(item), stdin)) { free_all(); return 0; }
    trim(item);
    if (strlen(item) == 0) { free_all(); return 0; }

    printf("unesi datum od (yyyy-mm-dd): ");
    fgets(from, sizeof(from), stdin);
    trim(from);

    printf("unesi datum do (yyyy-mm-dd): ");
    fgets(to, sizeof(to), stdin);
    trim(to);

    int qty = 0;
    float total = total_spent(item, from, to, &qty);

    printf("ukupno kupljeno %d kom, ukupno potroseno %.2f\n", qty, total);

    free_all();
    return 0;
}


