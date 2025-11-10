#include "conexion.h"
#include <stdio.h>

// Versión temporal - simula comunicación
bool conectar_servidor(void) {
    printf("Conectando al servidor... (SIMULADO)\n");
    return true;
}

void enviar_estado(EstadoJuego *estado) {
    // Enviará controles y recibirá estado actual del juego
    // Por ahora solo simulación
}

void recibir_actualizacion(EstadoJuego *estado) {
    // Recibirá estado actualizado del servidor
    // Por ahora solo mantiene el estado local
}

void desconectar_servidor(void) {
    printf("Desconectando del servidor... (SIMULADO)\n");
}