#include "game.h"
#include "config.h"
#include <math.h>
#include <stdio.h>

void inicializar_juego(EstadoJuego *estado) {
    // Jugador
    estado->jugador = (Jugador){
        200, 300,  // Empezar en una isla
        0, 0,                 // velocidades
        3, 0, true,           // vidas, puntuación, activo
        ESTADO_SUELO, -1,     // estado, liana_actual
        true, 0               // en_suelo, tiempo_salto
    };
    
    // Padre en plataforma superior
    estado->padre = (Padre){SCREEN_WIDTH/2, 80, true};
    
    // Plataforma superior
    estado->plataforma_superior = (Plataforma){0, 60, SCREEN_WIDTH};
    
    // Lianas
    estado->num_lianas = 4;
    estado->lianas[0] = (Liana){150, 80, SCREEN_HEIGHT - AGUA_HEIGHT - 20};
    estado->lianas[1] = (Liana){300, 80, SCREEN_HEIGHT - AGUA_HEIGHT - 20};
    estado->lianas[2] = (Liana){450, 80, SCREEN_HEIGHT - AGUA_HEIGHT - 20};
    estado->lianas[3] = (Liana){600, 80, SCREEN_HEIGHT - AGUA_HEIGHT - 20};
    
    // Islas
    estado->num_islas = 3;
    estado->islas[0] = (Isla){100, 400, 120, COLOR_ISLA_VERDE, COLOR_ISLA_SUELO};
    estado->islas[1] = (Isla){350, 300, 100, COLOR_ISLA_VERDE, COLOR_ISLA_SUELO};
    estado->islas[2] = (Isla){550, 450, 130, COLOR_ISLA_VERDE, COLOR_ISLA_SUELO};
    
    // Frutas
    estado->num_frutas = 4;
    estado->frutas[0] = (Fruta){200, 250, 100, FRUTA_NARANJA, true};
    estado->frutas[1] = (Fruta){350, 180, 150, FRUTA_CELESTE, true};
    estado->frutas[2] = (Fruta){500, 320, 200, FRUTA_ROJA, true};
    estado->frutas[3] = (Fruta){600, 280, 120, FRUTA_BANANO, true};
    
    // Cocodrilos
    estado->num_cocodrilos = 3;
    estado->cocodrilos[0] = (Cocodrilo){100, 200, 0, true};
    estado->cocodrilos[1] = (Cocodrilo){250, 300, 1, true};
    estado->cocodrilos[2] = (Cocodrilo){400, 150, 0, true};
    
    estado->juego_activo = true;
}

int encontrar_isla_para_respawn(Isla islas[], int num_islas) {
    // Por ahora devuelve una isla aleatoria
    return GetRandomValue(0, num_islas - 1);
}

// NUEVA FUNCIÓN: Verificar si está sobre cualquier superficie
bool esta_en_superficie(Jugador *jugador, EstadoJuego *estado) {
    // Verificar islas
    for (int i = 0; i < estado->num_islas; i++) {
        if (jugador->x >= estado->islas[i].x && 
            jugador->x <= estado->islas[i].x + estado->islas[i].ancho &&
            fabs(jugador->y - estado->islas[i].y) < 15) {
            jugador->y = estado->islas[i].y; // Ajustar posición
            return true;
        }
    }
    
    // Verificar plataforma superior
    if (jugador->x >= estado->plataforma_superior.x && 
        jugador->x <= estado->plataforma_superior.x + estado->plataforma_superior.ancho &&
        fabs(jugador->y - estado->plataforma_superior.y) < 15) {
        jugador->y = estado->plataforma_superior.y;
        return true;
    }
    
    return false;
}

bool esta_en_isla(Jugador *jugador, Isla islas[], int num_islas) {
    for (int i = 0; i < num_islas; i++) {
        if (jugador->x >= islas[i].x && 
            jugador->x <= islas[i].x + islas[i].ancho &&
            fabs(jugador->y - islas[i].y) < 10) {
            return true;
        }
    }
    return false;
}

int liana_mas_cercana(Jugador *jugador, Liana lianas[], int num_lianas) {
    int liana_cercana = -1;
    float distancia_min = DISTANCIA_AGARRE_LIANA;
    
    for (int i = 0; i < num_lianas; i++) {
        float dist = fabs(jugador->x - lianas[i].x);
        if (dist < distancia_min && 
            jugador->y >= lianas[i].y_inicio && 
            jugador->y <= lianas[i].y_fin) {
            distancia_min = dist;
            liana_cercana = i;
        }
    }
    return liana_cercana;
}
// Convertir coordenadas de pantalla a matriz
void coordenadas_a_matriz(float x, float y, int *fila, int *columna) {
    *columna = (int)(x / TAMANIO_CELDA);
    *fila = (int)(y / TAMANIO_CELDA);
}

// Convertir coordenadas de matriz a pantalla
void matriz_a_coordenadas(int fila, int columna, float *x, float *y) {
    *x = columna * TAMANIO_CELDA + TAMANIO_CELDA / 2;
    *y = fila * TAMANIO_CELDA + TAMANIO_CELDA / 2;
}

// Inicializar matriz del juego (versión temporal)
void inicializar_matriz(MatrizJuego *matriz, EstadoJuego *estado) {
    // Limpiar matriz
    for (int f = 0; f < MATRIZ_FILAS; f++) {
        for (int c = 0; c < MATRIZ_COLUMNAS; c++) {
            matriz->celdas[f][c].tipo = 0; // vacío
            matriz->celdas[f][c].datos_extra = 0;
        }
    }
    
    // Agua en la parte inferior
    for (int f = MATRIZ_FILAS - 2; f < MATRIZ_FILAS; f++) {
        for (int c = 0; c < MATRIZ_COLUMNAS; c++) {
            matriz->celdas[f][c].tipo = 3; // agua
        }
    }
    
    // Plataforma superior
    for (int c = 0; c < MATRIZ_COLUMNAS; c++) {
        matriz->celdas[1][c].tipo = 1; // plataforma
    }
    
    // Islas (convertir de coordenadas de mundo a matriz)
    for (int i = 0; i < estado->num_islas; i++) {
        int fila, col_inicio, col_fin;
        coordenadas_a_matriz(estado->islas[i].x, estado->islas[i].y, &fila, &col_inicio);
        coordenadas_a_matriz(estado->islas[i].x + estado->islas[i].ancho, estado->islas[i].y, &fila, &col_fin);
        
        for (int c = col_inicio; c <= col_fin && c < MATRIZ_COLUMNAS; c++) {
            if (c >= 0) matriz->celdas[fila][c].tipo = 1; // plataforma
        }
    }
    
    // Lianas
    for (int i = 0; i < estado->num_lianas; i++) {
        int fila_inicio, col, fila_fin;
        coordenadas_a_matriz(estado->lianas[i].x, estado->lianas[i].y_inicio, &fila_inicio, &col);
        coordenadas_a_matriz(estado->lianas[i].x, estado->lianas[i].y_fin, &fila_fin, &col);
        
        for (int f = fila_inicio; f <= fila_fin && f < MATRIZ_FILAS; f++) {
            if (f >= 0 && col >= 0 && col < MATRIZ_COLUMNAS) {
                matriz->celdas[f][col].tipo = 2; // liana
            }
        }
    }
    
    // Frutas
    for (int i = 0; i < estado->num_frutas; i++) {
        if (estado->frutas[i].activo) {
            int fila, col;
            coordenadas_a_matriz(estado->frutas[i].x, estado->frutas[i].y, &fila, &col);
            if (fila >= 0 && fila < MATRIZ_FILAS && col >= 0 && col < MATRIZ_COLUMNAS) {
                matriz->celdas[fila][col].tipo = 4; // fruta
                matriz->celdas[fila][col].datos_extra = estado->frutas[i].tipo;
            }
        }
    }
    
    // Cocodrilos
    for (int i = 0; i < estado->num_cocodrilos; i++) {
        if (estado->cocodrilos[i].activo) {
            int fila, col;
            coordenadas_a_matriz(estado->cocodrilos[i].x, estado->cocodrilos[i].y, &fila, &col);
            if (fila >= 0 && fila < MATRIZ_FILAS && col >= 0 && col < MATRIZ_COLUMNAS) {
                matriz->celdas[fila][col].tipo = 5; // cocodrilo
                matriz->celdas[fila][col].datos_extra = estado->cocodrilos[i].tipo;
            }
        }
    }
}

// Verificar colisiones usando matriz (versión alternativa)
void verificar_colisiones_matriz(EstadoJuego *estado, MatrizJuego *matriz) {
    Jugador *j = &estado->jugador;
    int fila, col;
    
    coordenadas_a_matriz(j->x, j->y, &fila, &col);
    
    if (fila >= 0 && fila < MATRIZ_FILAS && col >= 0 && col < MATRIZ_COLUMNAS) {
        // Verificar si está en agua
        if (matriz->celdas[fila][col].tipo == 3) { // agua
            estado->juego_activo = false;
            printf("¡Game Over! Caíste al agua\n");
            return;
        }
        
        // Verificar si está en cocodrilo
        if (matriz->celdas[fila][col].tipo == 5) { // cocodrilo
            estado->juego_activo = false;
            printf("¡Game Over! Tocaste un cocodrilo\n");
            return;
        }
        
        // Verificar si está en fruta
        if (matriz->celdas[fila][col].tipo == 4) { // fruta
            // Buscar la fruta en el estado y desactivarla
            for (int i = 0; i < estado->num_frutas; i++) {
                int f_fruta, c_fruta;
                coordenadas_a_matriz(estado->frutas[i].x, estado->frutas[i].y, &f_fruta, &c_fruta);
                if (f_fruta == fila && c_fruta == col && estado->frutas[i].activo) {
                    j->puntuacion += estado->frutas[i].puntos;
                    estado->frutas[i].activo = false;
                    matriz->celdas[fila][col].tipo = 0; // vacío
                    printf("¡Fruta tomada! +%d puntos\n", estado->frutas[i].puntos);
                    break;
                }
            }
        }
    }
}