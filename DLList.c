#include "DLList.h"

void DLL_error()
{
    fprintf(stderr, "*ERROR* The program has performed an illegal operation.\n");
}

void DLL_init(DLList *list)
{
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->length = 0;
}

void DLL_dispose(DLList *list)
{
    
    if(list == NULL)
    {
        return;
    }

    DLLnodePtr tmp = list->firstElement;


    while(tmp != NULL)
    {
        list->length--;
        list->firstElement = tmp->next;
        free(tmp);
        tmp = list->firstElement;
    }

    list->lastElement = NULL;
    list->firstElement = NULL;
}

void DLL_insertLast(DLList *list, char insertMe)
{
    DLLnodePtr tmp = malloc(sizeof(struct DLLnode));

    // if(tmp == NULL) ;        DODELAT ERROROVE HLASENI

    //Prepisu data noveho elementu a ukazatele noveho elementu
    tmp->data = insertMe;

    //Pro neprazdny seznam
    if(list->firstElement != NULL)
    {
        //elementu lastElement dame ukazatel na nas novy posledni element seznamu
        list->lastElement->next = tmp;
        //Novemu elementu dame ukazatel nextElement na NULL, protoze je posledni v listu
        tmp->next = NULL;
        //Previous ukazatel mu dame na lastElement
        tmp->prev = list->lastElement;
        //Prenastaveni ukazatele na posledni element
        list->lastElement = tmp;
    }
    //Pro prazdny seznam
    else
    {
        //Nastavim novy element, jako prvni a posledni v seznamu, protoze jich vice seznam nema
        list->firstElement = tmp;
        list->lastElement = tmp;
    }
}

void DLL_deleteFirst(DLList *list)
{
    //Osetreni neprazdnosti
    if(list->firstElement != NULL)
    {
        //Ziskam si pomocnym pointerem prvni element v listu 
        DLLnodePtr tmp = list->firstElement;

        //Pro jedno prvkovy list
        if(list->firstElement->next == NULL)
        {
            //V tomto pripade je jeden prvek prvni i posledni, takze chceme smazat oba
            list->lastElement = NULL;
            list->firstElement = NULL;
            free(tmp);
        }
        else
        {
            //Mazeme prvni, takze druhy musi mit previous ukazatel na NULL
            tmp->next->prev = NULL;
            //Nastavim novy prvni prvek, coz je prvek za mym novym prvkem
            list->firstElement = tmp->next;
            //uvolnim pamet
            free(tmp);
        }
    }
}

void DLL_deleteLast(DLList *list)
{
    //Osetreni neprazdnosti
    if(list->firstElement != NULL)
    {
        //Ziskam si pomocnym pointerem posledni element v listu 
        DLLnodePtr tmp = list->lastElement;

        //pro jedno prvkovy list
        if(list->firstElement->next == NULL)
        {
            //V tomto pripade je jeden prvek prvni i posledni, takze chceme smazat oba
            list->firstElement = NULL;
            free(tmp);
        }
        else
        {
            //Mazeme posledni, takze predposledni musi mit next ukazatel na NULL
            tmp->prev->next = NULL;
            //Nastavim novy posledni prvek listu
            list->lastElement = tmp->prev;
            //Uvolnim pamet
            free(tmp);
        }
    }
}

void DLL_getFirst(DLList *list, char *data)
{
    if(list->firstElement == NULL)
    {
        DLL_error();
        return;
    }
    *data = list->firstElement->data;
}

