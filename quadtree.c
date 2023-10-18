/*CONSTANTIN Gabriel - 312CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// implementare structura pentru culori rgb
typedef struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Color;

// implementare structura pentru arbore cuaternar
typedef struct QuadTreeNode
{
    unsigned char is_leaf;
    int size;
    Color color;
    // am denumit cei 4 fii ai unui nod dupa cele 4 puncte cardinale combinate
    struct QuadTreeNode *ne;
    struct QuadTreeNode *nv;
    struct QuadTreeNode *se;
    struct QuadTreeNode *sv;

} QuadTree, *TQuadTree;

// functie pentru crearea unui nod in arbore
QuadTree *create_node()
{
    // se initializeaza un nod
    QuadTree *node = (QuadTree *)calloc(1, sizeof(QuadTree));
    // datele acestuia sunt nule
    node->is_leaf = 0;
    node->size = 0;
    node->color.red = 0;
    node->color.green = 0;
    node->color.blue = 0;
    // fii acestuia sunt nuli
    node->ne = NULL;
    node->nv = NULL;
    node->se = NULL;
    node->sv = NULL;
    // returnam nodul
    return node;
}

// functie pentru crearea unei frunze
QuadTree *create_leaf(Color pixel, int size)
{
    // initializam un nod
    QuadTree *node = (QuadTree *)calloc(1, sizeof(QuadTree));
    // scriem despre aceasta ca este o frunza
    node->is_leaf = 1;
    // adaugam marimea pe care o are in imaginea compresata
    node->size = size;
    // trecem culorile rgb
    node->color.red = pixel.red;
    node->color.green = pixel.green;
    node->color.blue = pixel.blue;
    // fii acestui nod sunt nuli
    node->ne = NULL;
    node->nv = NULL;
    node->se = NULL;
    node->sv = NULL;
    // returnam frunza
    return node;
}

// functie pentru a calcula cate frunze avem in arbore
void leaves_tree(QuadTree *node, int *leaves)
{
    if (node->is_leaf) {
        // daca avem o frunza vom creste variabila pentru frunze
        (*leaves) = (*leaves) + 1;
        // ne oprim daca am ajuns la o frunza
        return;
    }
    // daca nu am ajuns la un nod terminal vom vizita tot fii acestuia
    leaves_tree(node->ne, &(*leaves));
    leaves_tree(node->nv, &(*leaves));
    leaves_tree(node->sv, &(*leaves));
    leaves_tree(node->se, &(*leaves));
}

// functie pentru a determina zona cea mai mare din imagine
void size_tree(QuadTree *node, int *size)
{
    if (node->is_leaf) { /*daca avem o frunza vom verifica daca zona acesteia e
       * mai mare decat zona deja gasita anterior*/
        if ((*size) < node->size)
            (*size) = node->size;
        // ne oprim daca am ajuns la o frunza
        return;
    }
    // daca nu am ajuns la o frunza vom merge recursiv pe fii nodului
    size_tree(node->ne, &(*size));
    size_tree(node->nv, &(*size));
    size_tree(node->sv, &(*size));
    size_tree(node->se, &(*size));
}

// functie pentru a determina numarul nivelurilor in arbore
int height_tree(QuadTree *node)
{
    // daca nodul nu exista returnam 0
    if (node == NULL)
        return 0;
    // daca nodul este o frunza returnam 1
    if (node->is_leaf == 1)
        return 1;

    /*vom merge recursiv pe cei 4 fii ai nodului si vom
     * memora inaltimile pe subarbori*/
    int height_nv = height_tree(node->nv);
    int height_ne = height_tree(node->ne);
    int height_se = height_tree(node->se);
    int height_sv = height_tree(node->sv);

    // comparam cele 4 inaltimi gasite pentru a o determina pe cea mai mare
    int max_height = height_nv;
    if (height_ne > max_height)
        max_height = height_ne;
    if (height_se > max_height)
        max_height = height_se;
    if (height_sv > max_height)
        max_height = height_sv;

    // returnam numarul maxim gasit plus o unitate pentru radacina
    return max_height + 1;
}

/*functie pentru a crea un arbore cuaternar plecand de la o matrice de culori
 * functia primeste ca parametri zona de start (n,m) si zona de terminare (h,w)
 * grid reprezinta matricea de culori, root arborele, iar factor eroarea*/
void make_quadtree(int n, int m, int h, int w, Color **grid,
                   TQuadTree *root, int factor)
{
    // variabilele x,y vor fi folosite pentru a parcurge matricea
    int x, y, r, g, b;
    unsigned long long mean;
    // initial verificam daca toata imagina poate fi comprimata
    mean = 0;
    r = g = b = 0;
    // calculam cele 3 culori r,g,b
    for (x = n; x < h; x++)
        for (y = m; y < w; y++) {
            r = r + grid[x][y].red;
            g = g + grid[x][y].green;
            b = b + grid[x][y].blue;
        }
    r = r / ((h - n) * (w - m));
    g = g / ((h - n) * (w - m));
    b = b / ((h - n) * (w - m));
    // calculam factorul mean
    for (x = n; x < h; x++)
        for (y = m; y < w; y++) {
            mean = mean + (r - grid[x][y].red) * (r - grid[x][y].red) +
                   (g - grid[x][y].green) * (g - grid[x][y].green) +
                   (b - grid[x][y].blue) * (b - grid[x][y].blue);
        }
    mean = mean / (3 * (h - n) * (w - m));
    // daca mean e mai mic decat factor vom crea o frunza
    if (mean <= factor) {
        // cum nodul a fost deja alocat, doar vom modifica datele din el
        Color pixel;
        pixel.red = r;
        pixel.green = g;
        pixel.blue = b;
        // nodul devine radacina, va avea dimensiune maxima si culori gasite
        (*root)->is_leaf = 1;
        (*root)->color.red = pixel.red;
        (*root)->color.green = pixel.green;
        (*root)->color.blue = pixel.blue;
        (*root)->size = h;
    }

    /* folosim acelasi algoritm pentru celelalte zone
     * dimensiunea unei zone este data de jumatatea distantei dintre
     * inceput si sfarsit */

    // zona nv
    mean = 0;
    r = g = b = 0;
    /* am adjustat cele 2 foruri pentru ca variabilele sa se afle
     * in zona matricei de culori */
    for (x = n; x < (h + n) / 2; x++)
        for (y = m; y < (w + m) / 2; y++) {
            r = r + grid[x][y].red;
            g = g + grid[x][y].green;
            b = b + grid[x][y].blue;
        }
    r = r / ((h / 2 - n / 2) * (w / 2 - m / 2));
    g = g / ((h / 2 - n / 2) * (w / 2 - m / 2));
    b = b / ((h / 2 - n / 2) * (w / 2 - m / 2));
    for (x = n; x < (h + n) / 2; x++)
        for (y = m; y < (w + m) / 2; y++) {
            mean = mean + (r - grid[x][y].red) * (r - grid[x][y].red) +
                   (g - grid[x][y].green) * (g - grid[x][y].green) +
                   (b - grid[x][y].blue) * (b - grid[x][y].blue);
        }
    mean = mean / (3 * (h / 2 - n / 2) * (w / 2 - m / 2));
    // daca mean este mai mic decat factorul, atunci vom initializa o frunza
    if (mean <= factor) {
        Color pixel;
        pixel.red = r;
        pixel.green = g;
        pixel.blue = b;
        (*root)->nv = create_leaf(pixel, (h - n) / 2);
    } else {
        /* daca nu am gasit o frunza, atunci vom merge in interiorul zonei si
         * vom reaplica algoritmul pe un nod nou */
        (*root)->nv = create_node();
        make_quadtree(n, m, (h + n) / 2, (w + m) / 2, grid,
                      &(*root)->nv, factor);
    }

    // zona ne
    mean = 0;
    r = g = b = 0;
    for (x = n; x < (h + n) / 2; x++)
        for (y = (w + m) / 2; y < w; y++) {
            r = r + grid[x][y].red;
            g = g + grid[x][y].green;
            b = b + grid[x][y].blue;
        }
    r = r / ((h / 2 - n / 2) * (w / 2 - m / 2));
    g = g / ((h / 2 - n / 2) * (w / 2 - m / 2));
    b = b / ((h / 2 - n / 2) * (w / 2 - m / 2));
    for (x = n; x < (h + n) / 2; x++)
        for (y = w / 2 + m / 2; y < w; y++) {
            mean = mean + (r - grid[x][y].red) * (r - grid[x][y].red) +
                   (g - grid[x][y].green) * (g - grid[x][y].green) +
                   (b - grid[x][y].blue) * (b - grid[x][y].blue);
        }
    mean = mean / (3 * (h / 2 - n / 2) * (w / 2 - m / 2));
    if (mean <= factor) {

        Color pixel;
        pixel.red = r;
        pixel.green = g;
        pixel.blue = b;
        (*root)->ne = create_leaf(pixel, (h - n) / 2);
    } else {
        (*root)->ne = create_node();
        make_quadtree(n, (w + m) / 2, (h + n) / 2, w, grid,
                      &(*root)->ne, factor);
    }

    // zona se
    mean = 0;
    r = g = b = 0;
    for (x = (h + n) / 2; x < h; x++)
        for (y = (w + m) / 2; y < w; y++) {
            r = r + grid[x][y].red;
            g = g + grid[x][y].green;
            b = b + grid[x][y].blue;
        }
    r = r / ((h / 2 - n / 2) * (w / 2 - m / 2));
    g = g / ((h / 2 - n / 2) * (w / 2 - m / 2));
    b = b / ((h / 2 - n / 2) * (w / 2 - m / 2));
    for (x = (h + n) / 2; x < h; x++)
        for (y = (w + m) / 2; y < w; y++) {
            mean = mean + (r - grid[x][y].red) * (r - grid[x][y].red) +
                   (g - grid[x][y].green) * (g - grid[x][y].green) +
                   (b - grid[x][y].blue) * (b - grid[x][y].blue);
        }
    mean = mean / (3 * (h / 2 - n / 2) * (w / 2 - m / 2));
    if (mean <= factor) {
        Color pixel;
        pixel.red = r;
        pixel.green = g;
        pixel.blue = b;
        (*root)->se = create_leaf(pixel, (h - n) / 2);
    } else {
        (*root)->se = create_node();
        make_quadtree((h + n) / 2, (w + m) / 2, h, w, grid,
                      &(*root)->se, factor);
    }

    // zona sv
    mean = 0;
    r = g = b = 0;
    for (x = (h + n) / 2; x < h; x++)
        for (y = m; y < (w + m) / 2; y++) {
            r = r + grid[x][y].red;
            g = g + grid[x][y].green;
            b = b + grid[x][y].blue;
        }
    r = r / ((h / 2 - n / 2) * (w / 2 - m / 2));
    g = g / ((h / 2 - n / 2) * (w / 2 - m / 2));
    b = b / ((h / 2 - n / 2) * (w / 2 - m / 2));
    for (x = (h + n) / 2; x < h; x++)
        for (y = m; y < (w + m) / 2; y++) {
            mean = mean + (r - grid[x][y].red) * (r - grid[x][y].red) +
                   (g - grid[x][y].green) * (g - grid[x][y].green) +
                   (b - grid[x][y].blue) * (b - grid[x][y].blue);
        }
    mean = mean / (3 * (h / 2 - n / 2) * (w / 2 - m / 2));
    if (mean <= factor) {
        Color pixel;
        pixel.red = r;
        pixel.green = g;
        pixel.blue = b;
        (*root)->sv = create_leaf(pixel, (h - n) / 2);
    } else {
        (*root)->sv = create_node();
        make_quadtree((h + n) / 2, m, h, (w + m) / 2, grid,
                      &(*root)->sv, factor);
    }
}

// strucuta pentru o coada de noduri din arborele cuaternar
typedef struct QueueNode
{
    QuadTree *node;
    struct QueueNode *next;
} QueueNode;

// structura pentru coada
typedef struct Queue
{
    QueueNode *head;
    QueueNode *tail;
} Queue;

// functie pentru adaugarea unui nod in coada
void enqueue(Queue *q, QuadTree *node)
{
    // cream un nod nou care va pointa la nodul din coada
    QueueNode *new_node = (QueueNode *)calloc(1, sizeof(QueueNode));
    new_node->node = node;
    new_node->next = NULL;

    // daca coada e goala, atunci inceputul si finalul vor pointa catre nod
    if (q->tail == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        // daca coada nu e goala, vom adauga nodul la coada
        q->tail->next = new_node;
        q->tail = new_node;
    }
}

// functie pentru a extrage din coada
QuadTree *dequeue(Queue *q)
{
    // daca coada e goala vom returna nul
    if (q->head == NULL) {
        return NULL;
    } else {
        // altfel vom crea un nod nou si un pointer catre inceputul cozii
        QuadTree *node = q->head->node;
        QueueNode *tmp = q->head;
        // mutam cursorul cozii
        q->head = q->head->next;
        // daca am ajuns la finalul cozii, atunci si capatul este nul
        if (q->head == NULL) {
            q->tail = NULL;
        }
        // eliberam elementul din coada si vom returna nodul
        free(tmp);
        return node;
    }
}

// parcurgerea arborelui in bfs si scrierea in fisier a acestuia
void bfs(QuadTree *root, FILE *fout)
{
    // cream o coada goala
    Queue *q = (Queue *)calloc(1, sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;

    // adaugam radacina in coada
    enqueue(q, root);
    QuadTree *node;
    // cat timp coada nu e goala vom repeta algoritmul
    while (q->head != NULL) {
        // scoatem capul din coada
        node = dequeue(q);
        // verificam daca avem o frunza
        if (node->is_leaf == 0) {
            // daca nu este frunza, atunci vom scrie in fisier valoarea 0
            fwrite(&node->is_leaf, sizeof(unsigned char), 1, fout);
        } else {
            // daca avem frunza, atunci vom scrie 1 si culorile rgb ale nodului
            fwrite(&node->is_leaf, sizeof(unsigned char), 1, fout);
            fwrite(&node->color.red, sizeof(unsigned char), 1, fout);
            fwrite(&node->color.green, sizeof(unsigned char), 1, fout);
            fwrite(&node->color.blue, sizeof(unsigned char), 1, fout);
        }
        if (node->is_leaf == 0) {
            // daca totodata nu avem frunza, atunci vom adauga in coada fii
            enqueue(q, node->nv);
            enqueue(q, node->ne);
            enqueue(q, node->se);
            enqueue(q, node->sv);
        }
    }
    // eliberam coada la final
    free(q);
}

// functie pentru decompresia unei imagini dintr-un fisier binar intr-un arbore
QuadTree *decompresie(FILE *fin, int height)
{

    unsigned char is_leaf;
    /*size1 si size2 reprezinta numarul de noduri de pe nivelul curent in
     * arbore si numarul de noduri de pe nivelul urmator*/
    int size1, size2;
    size1 = 1;
    size2 = 0;

    // cream radacina
    QuadTree *root = create_node();

    // cream o coada in care vom adauga nodurile
    Queue q;
    q.head = q.tail = NULL;
    enqueue(&q, root);

    // cat timp coada nu e goala vom repeta algoritmul
    while (q.head != NULL) {
        // micsoram size1 pentru fiecare nod pe nivel
        size1--;
        // extragem nodul din coada
        QuadTree *node = dequeue(&q);
        // citim daca acesta este o frunza sau nu
        fread(&is_leaf, sizeof(unsigned char), 1, fin);
        // daca nu e frunza, vom crea nodurile fii si le vom adauga in coada
        if (is_leaf == 0) {
            node->nv = create_node();
            enqueue(&q, node->nv);
            node->ne = create_node();
            enqueue(&q, node->ne);
            node->se = create_node();
            enqueue(&q, node->se);
            node->sv = create_node();
            enqueue(&q, node->sv);
            /*marim dimensiunea lui size2 cu 4 pentru ca vom avea inca 4
             * noduri noi pe nivelul urmator*/
            size2 += 4;
        } else {
            /*daca avem o frunza, atunci vom citi informatii despre culoarea
             *frunzei si le vom adauga la nod*/
            Color pixel;
            fread(&pixel.red, sizeof(unsigned char), 1, fin);
            fread(&pixel.green, sizeof(unsigned char), 1, fin);
            fread(&pixel.blue, sizeof(unsigned char), 1, fin);
            node->is_leaf = 1;
            node->color.red = pixel.red;
            node->color.green = pixel.green;
            node->color.blue = pixel.blue;
            node->size = height;
        }
        if (size1 == 0) {
            /*daca size1 a devenit 0 vom trece pe urmatorul nivel si vom
             * injumatati dimensiunea pe care o poate avea o frunza*/
            height = height / 2;
            size1 = size2;
            size2 = 0;
        }
    }
    // returnam arborele creat
    return root;
}

/*functie pentru a crea o imagine din arborele cuaternar
 * n,m reprezinta punctele de plecare, size dimensiunea unei zone*/
void make_picture(int n, int m, int size, Color **grid, TQuadTree root)
{
    // variabilele x si y vor fi folosite pentru parcurgerea matricei
    int x, y;
    // daca radacina e o frunza, atunci vom crea matricea imaginii
    if (root->is_leaf == 1) {
        for (x = n; x < root->size; x++)
            for (y = m; y < root->size; y++) {
                grid[x][y].red = root->color.red;
                grid[x][y].green = root->color.green;
                grid[x][y].blue = root->color.blue;
            }
        return;
    }
    // am aplicat acelasi algoritm pentru cele 4 zone din arbore
    // zona NV
    // daca nodul din stanga sus nu e frunza, vom cobori pe el
    if (root->nv->is_leaf == 0)
        make_picture(n, m, size / 2, grid, root->nv);
    // daca am ajuns aici, atunci avem o frunza si adaugam culoarea in matrice
    for (x = n; x < root->nv->size + n; x++)
        for (y = m; y < root->nv->size + m; y++) {
            grid[x][y].red = root->nv->color.red;
            grid[x][y].green = root->nv->color.green;
            grid[x][y].blue = root->nv->color.blue;
        }

    // zona NE
    // daca nodul din dreapta sus nu e frunza, vom cobori pe el
    if (root->ne->is_leaf == 0)
        make_picture(n, m + size / 2, size / 2, grid, root->ne);
    // daca am ajuns aici, atunci avem o frunza si adaugam culoarea in matrice
    for (x = n; x < root->ne->size + n; x++)
        for (y = m + root->ne->size; y < 2 * root->ne->size + m; y++) {
            grid[x][y].red = root->ne->color.red;
            grid[x][y].green = root->ne->color.green;
            grid[x][y].blue = root->ne->color.blue;
        }

    // zona SE
    // daca nodul din dreapta jos nu e frunza, vom cobori pe el
    if (root->se->is_leaf == 0)
        make_picture(n + size / 2, m + size / 2, size / 2, grid, root->se);
    // daca am ajuns aici, atunci avem o frunza si adaugam culoarea in matrice
    for (x = n + root->se->size; x < 2 * root->se->size + n; x++)
        for (y = m + root->se->size; y < 2 * root->se->size + m; y++) {
            grid[x][y].red = root->se->color.red;
            grid[x][y].green = root->se->color.green;
            grid[x][y].blue = root->se->color.blue;
        }

    // zona SV
    // dadca nodul din stanga jos nu e frunza, vom cobori pe el
    if (root->sv->is_leaf == 0)
        make_picture(n + size / 2, m, size / 2, grid, root->sv);
    // daca am ajuns aici, atunci avem o frunza si adaugam culoarea in matrice
    for (x = n + root->sv->size; x < 2 * root->sv->size + n; x++)
        for (y = m; y < root->sv->size + m; y++) {
            grid[x][y].red = root->sv->color.red;
            grid[x][y].green = root->sv->color.green;
            grid[x][y].blue = root->sv->color.blue;
        }
}

// functie pentru eliberarea nodurilor din arbore
void free_tree(QuadTree *node)
{
    // daca nodul este o frunza verificam daca nu cumva fii sunt initializati
    if (node->is_leaf) {
        if (node->nv != NULL)
            free(node->nv);
        if (node->ne != NULL)
            free(node->ne);
        if (node->se != NULL)
            free(node->se);
        if (node->sv != NULL)
            free(node->sv);
        // eliberam frunza
        free(node);
        return;
    }
    // mergem recursiv pe arbore pentru a elibera de cat mai jos memoria
    if (node->ne != NULL)
        free_tree(node->ne);
    if (node->nv != NULL)
        free_tree(node->nv);
    if (node->sv != NULL)
        free_tree(node->sv);
    if (node->se != NULL)
        free_tree(node->se);
    free(node);
}

int main(int argc, char *argv[])
{
    // fisiere de citire si scriere
    FILE *fin;
    FILE *fout;
    // variabile si factor
    unsigned int i, j, factor;
    // arborele
    TQuadTree root;
    // matricea de culori
    Color **grid;
    if (strstr(argv[1], "c1")) {
        // daca cerinta e de tipul c1 vom deschide fisierele si factorul
        fin = fopen(argv[3], "rb");
        fout = fopen(argv[4], "w");
        factor = atoi(argv[2]);
    }
    if (strstr(argv[1], "c2")) {
        // daca cerinta e de tipul c2 vom deschide fisierele si factorul
        fin = fopen(argv[3], "rb");
        fout = fopen(argv[4], "wb");
        factor = atoi(argv[2]);
    }
    if (strchr(argv[1], 'd')) {
        // daca cerinta e de tipul d vom deschide fisierele
        fin = fopen(argv[2], "rb");
        fout = fopen(argv[3], "wb");
    }
    // daca fisierele nu merg corect vom iesi din program
    if (!fin) {
        exit(1);
    }
    if (fout == NULL) {
        exit(1);
    }

    // verificam daca cerinta e de tipul c sau d
    if (strchr(argv[1], 'c')) {
        // initializam arborele
        root = create_node();
        TQuadTree varf = root;
        unsigned int width, height, val_max;
        char magic[3];
        // citim valorile din fisierul ppm
        fscanf(fin, "%2s\n%d %d\n%d", magic, &width, &height, &val_max);
        // initializam matricea de culori
        grid = calloc(width, sizeof(struct Color *));
        for (i = 0; i < width; i++)
            grid[i] = calloc(height, sizeof(struct Color));
        fseek(fin, 1, SEEK_CUR);
        // citim pe rand datele despre culori
        for (i = 0; i < width; i++)
            for (j = 0; j < height; j++) {
                Color pixel;
                fread(&pixel, sizeof(struct Color), 1, fin);
                grid[i][j] = pixel;
            }
        // cream arborele cuaternar
        make_quadtree(0, 0, height, width, grid, &varf, factor);
        // eliberam matricea de culori
        for (i = 0; i < width; i++)
            free(grid[i]);
        free(grid);
        // daca avem cerinta c1 vom afla datele necesare
        if (strchr(argv[1], '1')) {
            // aflam inaltimea, numarul de frunze si dimensiunea maxima
            int h = height_tree(root);
            int leaves = 0;
            leaves_tree(root, &leaves);
            int dim = 0;
            size_tree(root, &dim);
            fprintf(fout, "%d\n%d\n%d\n", h, leaves, dim);
        }
        if (strchr(argv[1], '2')) {
            // daca avem cerinta 2, vom scrie datele in fisierul binar
            fwrite(&width, sizeof(unsigned int), 1, fout);
            bfs(root, fout);
        }
    }
    // daca avem cerinta d vom citi inaltimea si vom crea matricea
    if (strchr(argv[1], 'd')) {    
        unsigned int height;
        fread(&height, sizeof(unsigned int), 1, fin);

        // alocam spatiu pentru matrice
        grid = calloc(height, sizeof(struct Color *));
        for (i = 0; i < height; i++)
            grid[i] = calloc(height, sizeof(struct Color));

        // aplicam algoritmul de decompresie
        root = decompresie(fin, height);

        // cream matricea
        make_picture(0, 0, height, grid, root);
        
        // scriem datele in fisierul ppm
        fprintf(fout, "P6\n%d %d\n255\n", height, height);
        for (i = 0; i < height; i++)
            for (j = 0; j < height; j++)
                fwrite(&grid[i][j], sizeof(struct Color), 1, fout);
        // eliberam matricea
        for (i = 0; i < height; i++)
            free(grid[i]);
        free(grid);
    }
    // eliberam spatiul pentru arbore si inchidem fisierele
    free_tree(root);
    fclose(fin);
    fclose(fout);
    return 0;
}
