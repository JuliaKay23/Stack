#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "da.h"

struct stack {
    DA *ds;
    int debug;
    void (*display)(void *, FILE *);
    int size;
};

STACK *newSTACK(void){
    STACK *items = malloc(sizeof(STACK));
    items->ds = newDA();
    items->debug = 0;
    items->display = NULL;
    items->size = 0;
    assert(items != 0);
    return items;
}

void  setSTACKdisplay(STACK *items,void (*display)(void *,FILE *)){
    items->display = display;
    setDAdisplay(items->ds,display);
}

void  setSTACKfree(STACK *items,void (*free)(void *)) {
    setDAfree(items->ds,free);
}
void  push(STACK *items,void *value){
    insertDA(items->ds,sizeDA(items->ds),value);
    items->size += 1;
}

void *pop(STACK *items){
    assert(sizeDA(items->ds) > 0);
    items->size -= 1;
    return removeDA(items->ds,sizeDA(items->ds) - 1);
}

void *peekSTACK(STACK *items){
    assert(sizeDA(items->ds) > 0);
    return getDA(items->ds,sizeDA(items->ds) - 1);
}

void  displaySTACK(STACK *items,FILE *fp) {
    if (items->debug == 0) {
        if (sizeDA(items->ds) == 0) {
            fprintf(fp, "||");
        }
        else if (sizeDA(items->ds) > 0) {
            fprintf(fp, "|");
            if (items->display == NULL) {
                for (int i = sizeDA(items->ds) - 1; i >= 1; i--) {
                    fprintf(fp, "@%p,", getDA(items->ds, i));
                }
                fprintf(fp, "@%p", getDA(items->ds, 0));
            }
            else if (items->display != NULL) {
                for (int i = sizeDA(items->ds) - 1; i >= 1; i--) {
                    items->display(getDA(items->ds, i), fp);
                    fprintf(fp, ",");
                }
                items->display(getDA(items->ds, 0), fp);
            }
            fprintf(fp, "|");
        }
    }
    else if (items->debug == 1 || items->debug == 2) {
        displayDA(items->ds, fp);
    }
}

int   debugSTACK(STACK *items,int level){
    int old = items->debug;
    if (level == 1){
        debugDA(items->ds,0);
    }
    else if (level == 2){
        debugDA(items->ds,1);
    }
    items->debug = level;
    return old;
}

void  freeSTACK(STACK *items){
    freeDA(items->ds);
    free(items);
}

int   sizeSTACK(STACK *items){
    return sizeDA(items->ds);
}