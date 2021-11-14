/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL) {
        if (tree->key == key) {
            // We found it
            *value = tree->value;

            return true;
        }

        // This key isn't the right one
        if (key < tree->key) {
            // Key is less than current one --> let's look to the left
            tree = tree->left;
        } else {
            // Key is greater than current one --> let's look to the right
            tree = tree->right;
        }
    }

    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    // Search for a place where to insert the new node to
    bool found = false;
    bst_node_t *where = NULL;
    bst_node_t *current_subtree = *tree;
    if (current_subtree != NULL) {
        while (!found && current_subtree != NULL) {
            where = current_subtree;
            if (key < current_subtree->key) {
                // Key is less than current one --> let's look to the left
                current_subtree = current_subtree->left;
            } else if (key > current_subtree->key) {
                // Key is greater than current one --> let's look to the right
                current_subtree = current_subtree->right;
            } else {
                // Keys are the same --> we found it
                found = true;
            }
        }
    }

    if (found) {
        // The key is already in the tree --> just update value
        where->value = value;

        return;
    }

    // Key isn't in the tree --> add it
    // Create a new node
    bst_node_t *new_item;
    if ((new_item = malloc(sizeof(bst_node_t))) == NULL) {
        return;
    }

    // Init new node
    new_item->key = key;
    new_item->value = value;
    new_item->left = NULL;
    new_item->right = NULL;

    // Insert new node into the tree
    if (where == NULL) {
        // The tree is empty --> just set new node as a root
        *tree = new_item;

        return;
    }

    // The new node will be a leaf
    if (key < where->key) {
        // It will be at the left
        where->left = new_item;
    } else {
        // It will be at the right
        where->right = new_item;
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    // Find rightmost node
    bst_node_t **ptr_to_rightmost = tree;
    bst_node_t *rightmost = *tree;
    while (rightmost->right != NULL) {
        ptr_to_rightmost = &(rightmost->right);
        rightmost = rightmost->right;
    }

    target->key = rightmost->key;
    target->value = rightmost->value;

    bst_node_t *left_subtree = rightmost->left;
    free(rightmost);
    *ptr_to_rightmost = left_subtree;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL) {
        // Empty tree --> key can't be contained inside it
        return;
    }

    // Non-empty tree
    // Find node for deletion
    bst_node_t **ptr_to_del_item = tree;
    bst_node_t *deletion_item = *tree;
    while (deletion_item != NULL && deletion_item->key != key) {
        // This key isn't the right one
        if (key < deletion_item->key) {
            // Key is less than current one --> let's look to the left
            ptr_to_del_item = &(deletion_item->left);
            deletion_item = deletion_item->left;
        } else {
            // Key is greater than current one --> let's look to the right
            ptr_to_del_item = &(deletion_item->right);
            deletion_item = deletion_item->right;
        }
    }

    if (deletion_item == NULL) {
        // Item for deletion isn't in the tree --> we're done
        return;
    }

    if (deletion_item->left == NULL && deletion_item->right == NULL) {
        // Item has no child --> just delete it
        free(deletion_item);
        *ptr_to_del_item = NULL;
    } else if (deletion_item->left != NULL && deletion_item->right == NULL) {
        // Item has LEFT child only
        bst_node_t *left_subtree = deletion_item->left;

        // Replace this node with the left one
        **ptr_to_del_item = *left_subtree;

        free(left_subtree);
    } else if (deletion_item->left == NULL && deletion_item->right != NULL) {
        // Item has RIGHT child only
        bst_node_t *right_subtree = deletion_item->right;

        // Replace this node with the right one
        **ptr_to_del_item = *right_subtree;

        free(right_subtree);
    } else {
        // Item has BOTH children
        bst_replace_by_rightmost(deletion_item, &(deletion_item->left));
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    // Prepare help stack
    stack_bst_t help_stack;
    stack_bst_init(&help_stack);

    bst_node_t *current_item = *tree;
    do {
        if (current_item == NULL) {
            if (!stack_bst_empty(&help_stack)) {
                current_item = stack_bst_top(&help_stack);
                stack_bst_pop(&help_stack);
            }
        } else {
            if (current_item->right != NULL) {
                stack_bst_push(&help_stack, current_item->right);
            }

            bst_node_t *help_item = current_item;
            current_item = current_item->left;

            free(help_item);
        }
    } while (current_item != NULL || !stack_bst_empty(&help_stack));

    *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    // Prepare help stack
    stack_bst_t help_stack;
    stack_bst_init(&help_stack);

    bst_leftmost_preorder(tree, &help_stack);
    while (!stack_bst_empty(&help_stack)) {
        tree = stack_bst_top(&help_stack);
        stack_bst_pop(&help_stack);
        bst_leftmost_preorder(tree->right, &help_stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    // Prepare help stack
    stack_bst_t help_stack;
    stack_bst_init(&help_stack);

    bst_leftmost_inorder(tree, &help_stack);
    while (!stack_bst_empty(&help_stack)) {
        tree = stack_bst_top(&help_stack);
        stack_bst_pop(&help_stack);
        bst_print_node(tree);
        bst_leftmost_inorder(tree->right, &help_stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    // Prepare help stacks
    stack_bst_t help_stack_nodes;
    stack_bool_t help_stack_visits;
    stack_bst_init(&help_stack_nodes);
    stack_bool_init(&help_stack_visits);

    bool from_left;
    bst_leftmost_postorder(tree, &help_stack_nodes, &help_stack_visits);
    while (!stack_bst_empty(&help_stack_nodes)) {
        tree = stack_bst_top(&help_stack_nodes);
        from_left = stack_bool_top(&help_stack_visits);
        stack_bool_pop(&help_stack_visits);

        if (from_left) {
            // Returned from left --> continue to the right
            stack_bool_push(&help_stack_visits, false);
            bst_leftmost_postorder(tree->right, &help_stack_nodes, &help_stack_visits);
        } else {
            // Returned from right --> it's parent's turn
            stack_bst_pop(&help_stack_nodes);
            bst_print_node(tree);
        }
    }
}
