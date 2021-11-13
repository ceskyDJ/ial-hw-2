/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        result += key[i];
    }

    return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for (int i = 0; i < HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    // Use hash function to find item with the right index
    ht_item_t *found = (*table)[get_hash(key)];

    // Found item hasn't to be the search one, we need equality of search key and key of the found item
    // There could be more items at one index, so we need to iterate over the list of them to find the right one
    while (found != NULL && strcmp(found->key, key) != 0) {
        found = found->next;
    }

    // There will be found item or NULL if the searched item hasn't been found
    return found;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    ht_item_t *item;
    if ((item = ht_search(table, key)) != NULL) {
        // Item is already in the table --> only change its value
        item->value = value;

        return;
    }

    // Item is new --> find the right place in the table for it
    int index = get_hash(key);
    item = (*table)[index];

    // Create an item
    ht_item_t *new_item;
    if ((new_item = malloc(sizeof(ht_item_t))) == NULL) {
        return;
    }

    new_item->key = key;
    new_item->value = value;
    new_item->next = item;

    // Add item to the first place of the selected index
    (*table)[index] = new_item;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t *item;
    if ((item = ht_search(table, key)) != NULL) {
        return &item->value;
    } else {
        return NULL;
    }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    // Use hash function to find item with the right index
    int index = get_hash(key);
    ht_item_t *item = (*table)[index];

    if (strcmp(item->key, key) == 0) {
        // It's the first item at the index
        // Move to the next item (or NULL if item for deletion is the last one)
        (*table)[index] = item->next;

        // Delete item
        free(item);

        return;
    }

    // There is more items at the index or item for deletion isn't in the table at all
    // We need to find item for deletion in the list of synonyms
    ht_item_t *prev_item = item;
    while (item != NULL && strcmp(item->key, key) != 0) {
        prev_item = item;
        item = item->next;
    }

    if (item == NULL) {
        // Item for deletion isn't in the table at all --> do nothing
        return;
    }

    // Item is in the list --> remove it from there
    prev_item->next = item->next;
    free(item);
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    for (int i = 0; i < HT_SIZE; i++) {
        // Delete second and next items from the list of synonyms
        ht_item_t *item = (*table)[i];
        ht_item_t *next = item;
        while (next != NULL) {
            item = next;
            free(item);

            next = next->next;
        }

        // Set list of synonyms as empty
        (*table)[i] = NULL;
    }
}
