#ifndef CONEXION_H
#define CONEXION_H

#include "game.h"
#include <stdint.h>
#include "config.h"
#include "controles.h"

// Estructura para enviar inputs al servidor
typedef struct {
    bool izquierda, derecha, arriba, abajo, espacio;
    int jugador_id;
    uint32_t timestamp;
} PaqueteInput;

// Estructura para recibir estado del servidor  
typedef struct {
    int matriz[MATRIZ_FILAS][MATRIZ_COLUMNAS];
    int vidas;
    int puntuacion;
    bool juego_activo;
} PaqueteEstado;

// Placeholders actualizados
bool conectar_servidor(void);
bool enviar_inputs_al_servidor(PaqueteInput *inputs);
bool recibir_estado_del_servidor(PaqueteEstado *estado);
void desconectar_servidor(void);

// Función temporal para simular servidor
void simular_servidor_local(EstadoJuego *estado, Controles *ctrl);

#endif