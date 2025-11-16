#include "conexion.h"
#include <stdio.h>

//Version temporal para simular comunicacion con servidor
bool conectar_servidor(void) {
    printf("Conectando al servidor... (SIMULADO)\n");
    return true;
}

void enviar_estado(EstadoJuego *estado) {
    // Enviara controles y recibira estado actual del juego
}

void recibir_actualizacion(EstadoJuego *estado) {
    // Recibira estado actualizado del servidor
    // Por ahora solo mantiene el estado local
}

void desconectar_servidor(void) {
    printf("Desconectando del servidor... (SIMULADO)\n");
}