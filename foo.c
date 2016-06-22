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

    int n = 0;
    for (l = filled2; l; l = l->next) {
        n++;
    }

    double xs[n], ys[n];
    n = 0;

    for (l = filled2; l; l = l->next) {
        if (l->type == gfx_moveTo || l->type == gfx_lineTo) {
            xs[n] = l->x;
            ys[n] = l->y;
            n++;
        }

        if (l->next == NULL || l->next->type != gfx_lineTo) {
            if (n > 2) {
                printf("newpath ");

                int i;
                for (i = 0; i < n; i++) {
                    printf("%f %f %s ", xs[i], ys[i], i == 0 ? "moveto" : "lineto");
                }
                n = 0;

                printf("closepath fill\n");
            }
        }
    }

    printf("0 setgray\n");
    printf("1 .setopacityalpha\n");

    for (l = filled2; l; l = l->next) {
        if (l->type == gfx_moveTo || l->type == gfx_lineTo) {
            xs[n] = l->x;
            ys[n] = l->y;
            n++;
        }

        if (l->next == NULL || l->next->type != gfx_lineTo) {
            if (n > 2) {
                printf("newpath ");

                int i;
                for (i = 0; i < n; i++) {
                    printf("%f %f %s ", xs[i], ys[i], i == 0 ? "moveto" : "lineto");
                }
                n = 0;

                printf("closepath stroke\n");
            }
        }
    }
}
