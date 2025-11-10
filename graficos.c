#include "raylib.h"
#include "graficos.h"
#include "config.h"
#include "game.h"


void inicializar_graficos(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DonCEy Kong Jr");
    SetTargetFPS(FPS);
}

void cerrar_graficos(void) {
    CloseWindow();
}

void dibujar_jugador(Jugador *j) {
    if (j->activo) {
        DrawRectangle(j->x - JUGADOR_SIZE/2, j->y - JUGADOR_SIZE/2, 
                     JUGADOR_SIZE, JUGADOR_SIZE, COLOR_JUGADOR);
    }
}

void dibujar_cocodrilo(Cocodrilo *c) {
    if (c->activo) {
        Color color = (c->tipo == 0) ? COLOR_COCODRILO_ROJO : COLOR_COCODRILO_AZUL;
        DrawRectangle(c->x - COCODRILO_SIZE/2, c->y - COCODRILO_SIZE/2,
                     COCODRILO_SIZE, COCODRILO_SIZE, color);
    }
}

void dibujar_fruta(Fruta *f) {
    if (f->activo) {
        Color color_fruta;
        
        switch (f->tipo) {
            case FRUTA_NARANJA:
                color_fruta = COLOR_NARANJA;
                break;
            case FRUTA_CELESTE:
                color_fruta = COLOR_FRUTA_CELESTE;
                break;
            case FRUTA_ROJA:
                color_fruta = COLOR_FRUTA_ROJA;
                break;
            case FRUTA_BANANO:
                color_fruta = COLOR_BANANO;
                break;
            default:
                color_fruta = GREEN;
        }
        
        DrawRectangle(f->x - FRUTA_SIZE/2, f->y - FRUTA_SIZE/2,
                     FRUTA_SIZE, FRUTA_SIZE, color_fruta);
        
        // Opcional: pequeño detalle para diferenciar
        DrawRectangle(f->x - FRUTA_SIZE/4, f->y - FRUTA_SIZE/4,
                     FRUTA_SIZE/2, FRUTA_SIZE/2, (Color){color_fruta.r/2, color_fruta.g/2, color_fruta.b/2, 255});
    }
}

void dibujar_lianas(Liana lianas[], int count) {
    for (int i = 0; i < count; i++) {
        DrawRectangle(lianas[i].x - LIANA_WIDTH/2, lianas[i].y_inicio,
                     LIANA_WIDTH, lianas[i].y_fin - lianas[i].y_inicio, COLOR_LIANA);
    }
}


void dibujar_ui(EstadoJuego *estado) {
    DrawText(TextFormat("Vidas: %d", estado->jugador.vidas), 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Puntos: %d", estado->jugador.puntuacion), 10, 40, 20, DARKGRAY);
    
    if (!estado->juego_activo) {
        DrawText("GAME OVER", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 20, 40, RED);
    }
}
void dibujar_agua(void) {
    DrawRectangle(0, SCREEN_HEIGHT - AGUA_HEIGHT, SCREEN_WIDTH, AGUA_HEIGHT, COLOR_AGUA);
}

void dibujar_plataforma_superior(Plataforma *p) {
    DrawRectangle(p->x, p->y, p->ancho, PLATAFORMA_HEIGHT, COLOR_PLATAFORMA);
}

void dibujar_padre(Padre *p) {
    if (p->activo) {
        DrawRectangle(p->x - JUGADOR_SIZE/2, p->y - JUGADOR_SIZE/2, 
                     JUGADOR_SIZE, JUGADOR_SIZE, COLOR_PADRE);
    }
}

void dibujar_isla(Isla *isla) {
    // Parte superior verde
    DrawRectangle(isla->x, isla->y - 10, isla->ancho, 10, isla->color_superior);
    // Parte inferior café
    DrawRectangle(isla->x, isla->y, isla->ancho, 10, isla->color_inferior);
}

void dibujar_mario(void) {
    // Mario temporal - azul con detalle rojo
    DrawRectangle(SCREEN_WIDTH/2 - 15, 40, 30, 20, COLOR_MARIO_BASE);
    DrawRectangle(SCREEN_WIDTH/2 - 5, 45, 10, 10, COLOR_MARIO_DETALLE);
}

void dibujar_escena(EstadoJuego *estado) {
    BeginDrawing();
    ClearBackground(COLOR_FONDO);
    
    // Dibujar elementos en orden
    dibujar_agua();
    
    // Islas
    for (int i = 0; i < estado->num_islas; i++) {
        dibujar_isla(&estado->islas[i]);
    }
    
    // Lianas
    for (int i = 0; i < estado->num_lianas; i++) {
        DrawRectangle(estado->lianas[i].x - LIANA_WIDTH/2, estado->lianas[i].y_inicio,
                     LIANA_WIDTH, estado->lianas[i].y_fin - estado->lianas[i].y_inicio, COLOR_LIANA);
    }
    
    // Plataforma superior y personajes
    dibujar_plataforma_superior(&estado->plataforma_superior);
    dibujar_mario();
    dibujar_padre(&estado->padre);
    
    // Elementos del juego
    for (int i = 0; i < estado->num_frutas; i++) {
        dibujar_fruta(&estado->frutas[i]);
    }
    
    for (int i = 0; i < estado->num_cocodrilos; i++) {
        dibujar_cocodrilo(&estado->cocodrilos[i]);
    }
    
    dibujar_jugador(&estado->jugador);
    dibujar_ui(estado);
    
    EndDrawing();
}