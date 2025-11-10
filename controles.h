#ifndef CONTROLES_H
#define CONTROLES_H

#include "game.h"

typedef struct {
    bool izquierda, derecha, arriba, abajo, espacio;
} Controles;

void actualizar_controles(Controles *ctrl);
void aplicar_movimiento(EstadoJuego *estado, Controles *ctrl);


#endif