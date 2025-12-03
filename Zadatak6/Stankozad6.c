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

/* trim - uklanjanje razmaka i \n */
void trim(char* s) {
    if (!s) return;
    while (*s == ' ' || *s == '\t') s++;

    char* end = s + strlen(s);
    while (end > s && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\n' || end[-1] == '\r'))
        end--;

    *end = '\0';
}

/* dodavanje artikla sortirano po nazivu */
void add_item_sorted(Item** item_head, const char* name_in, int qty, float price) {
    Item* prev = NULL;
    Item* cur = *item_head;

    while (cur && strcmp(cur->name, name_in) < 0) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && strcmp(cur->name, name_in) == 0) {
        cur->qty += qty;  /* već postoji — povećaj količinu */
        return;
    }

    Item* it = malloc(sizeof(Item));
    strcpy(it->name, name_in);
    it->qty = qty;
    it->price = price;
    it->next = cur;

    if (prev) prev->next = it;
    else *item_head = it;
}

/* dodavanje računa sortirano po datumu */
void add_receipt_sorted(Receipt* r) {
    if (!head || strcmp(head->date, r->date) > 0) {
        r->next = head;
        head = r;
        return;
    }
    Receipt* cur = head;
    while (cur->next && strcmp(cur->next->date, r->date) <= 0)
        cur = cur->next;

    r->next = cur->next;
    cur->next = r;
}

/* čitanje jednog računa */
Receipt* parse_receipt_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    char line[LINE_BUF];

    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return NULL;
    }
    trim(line);

    Receipt* r = malloc(sizeof(Receipt));
    strncpy(r->date, line, 10);
    r->date[10] = '\0';
    r->items = NULL;
    r->next = NULL;

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strlen(line) == 0) continue;

        char* name = strtok(line, ",");
        char* qtys = strtok(NULL, ",");
        char* prices = strtok(NULL, ",");

        if (!name || !qtys || !prices) continue;

        trim(name);
        trim(qtys);
        trim(prices);

        int qty = atoi(qtys);
        float price = atof(prices);

        add_item_sorted(&r->items, name, qty, price);
    }

    fclose(f);
    return r;
}

/* učitava sve račune iz racuni.txt */
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

/* izračun potrošnje za artikl u intervalu */
float total_spent(const char* item_name, const char* date_from, const char* date_to, int* out_qty) {
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

/* oslobađanje memorije */
void free_all() {
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
    fgets(item, sizeof(item), stdin);
    trim(item);

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
