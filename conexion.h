#ifndef CONEXION_H
#define CONEXION_H

#include "game.h"
// Funciones placeholder - se implementarán para integración
bool conectar_servidor(void);
void enviar_estado(EstadoJuego *estado);
void recibir_actualizacion(EstadoJuego *estado);
void desconectar_servidor(void);

#endif