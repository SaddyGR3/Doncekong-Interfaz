#include "C:/raylib/raylib/src/raylib.h"
#include "raylib.h" 
#include "game.h"
#include "config.h"
#include "graficos.h"
#include "controles.h"
#include "conexion.h"

int main(void) {
    EstadoJuego estado;
    Controles controles = {0};
    
    inicializar_graficos();
    inicializar_juego(&estado);
    
    // Para integración: conectar_servidor();
    
    while (!WindowShouldClose() && estado.juego_activo) {
        // Actualizar controles
        actualizar_controles(&controles);
        
        // Aplicar movimiento - CORREGIDO: ahora pasa el estado completo
        aplicar_movimiento(&estado, &controles);
        
        // Verificar colisiones
        verificar_colisiones_matriz(&estado); // Solo el estado, la matriz está dentro
        
        // Para integración:
        // enviar_estado(&estado);
        // recibir_actualizacion(&estado);
        
        // Dibujar
        dibujar_escena(&estado);
    }
    
    // Para integración: desconectar_servidor();
    cerrar_graficos();
    
    return 0;
}