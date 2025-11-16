#include "graficos.h"
#include "mapa.h"
#include "controles.h"
#include "game.h"

int main(void) {
    EstadoJuego estado;
    Controles ctrl = {0};
    
    inicializar_graficos();
    cargar_mapa(&mapa_global);
    inicializar_juego(&estado);
    configurar_mapa_completo(&mapa_global, &estado); // Sincronizar debug
    
    while (!WindowShouldClose()) {
        actualizar_controles(&ctrl);
        aplicar_movimiento(&estado, &ctrl);
        verificar_colisiones_matriz(&estado);
        actualizar_matriz_desde_estado(&estado);
        
        // Dibujar con sprites y debug
        dibujar_escena_completa(&estado, &mapa_global, &sprites_global);
    }
    
    descargar_mapa(&mapa_global);
    cerrar_graficos();
    return 0;
}