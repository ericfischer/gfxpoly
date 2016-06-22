#include "gfxpoly.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    gfxcanvas_t* canvas = gfxcanvas_new(1);
    int sides = 1000;

    if (argc > 1) {
        sides = atoi(argv[1]);
    }

    int i;
    canvas->moveTo(canvas, rand() % 4000, rand() % 4000);
    for (i = 0; i < sides; i++) {
        canvas->lineTo(canvas, rand() % 4000, rand() % 4000);
    }
    canvas->close(canvas);

    gfxpoly_t* poly = (gfxpoly_t*)canvas->result(canvas);

    gfxpoly_t* evenodd = gfxpoly_process(poly, NULL, &windrule_evenodd, &onepolygon, NULL);

    printf("0 setlinewidth\n");
    printf(".5 .setopacityalpha\n");
    printf(".5 setgray\n");

    int within = 0;

    gfxline_t* filled2 = gfxline_from_gfxpoly_with_direction(evenodd);
    gfxline_t* l;
    for (l = filled2; l; l = l->next) {
        if (l->type == gfx_moveTo) {
            if (within) {
                printf("\n closepath fill\n");
                within = 0;
            }
            printf("\nnewpath %f %f moveto", l->x, l->y);
            within = 1;
        } else if (l->type == gfx_lineTo) {
            printf(" %f %f lineto", l->x, l->y);
        }
    }

    if (within) {
        printf("\n closepath fill\n");
        within = 0;
    }

    printf("0 setgray\n");
    printf("1 .setopacityalpha\n");

    for (l = filled2; l; l = l->next) {
        if (l->type == gfx_moveTo) {
            printf("\n%f %f moveto", l->x, l->y);
            within = 1;
        } else if (l->type == gfx_lineTo) {
            printf(" %f %f lineto", l->x, l->y);
        }
    }

    printf("\nstroke\n");
}
