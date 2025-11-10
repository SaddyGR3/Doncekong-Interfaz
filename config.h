#ifndef CONFIG_H
#define CONFIG_H

// Configuración de pantalla
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

// Configuración de matriz de juego
#define MATRIZ_FILAS 15
#define MATRIZ_COLUMNAS 20
#define TAMANIO_CELDA 40  // Cada celda de matriz = 40x40 píxeles

// Tamaños
#define JUGADOR_SIZE 20
#define COCODRILO_SIZE 18
#define FRUTA_SIZE 15
#define LIANA_WIDTH 5
#define PLATAFORMA_HEIGHT 20
#define AGUA_HEIGHT 80

// Nuevos colores según tu descripción
#define COLOR_FONDO BLACK
#define COLOR_JUGADOR (Color){184, 134, 72, 255}       // Café claro
#define COLOR_PADRE (Color){101, 67, 33, 255}          // Café oscuro
#define COLOR_AGUA (Color){100, 149, 237, 255}         // Celeste
#define COLOR_LIANA YELLOW
#define COLOR_PLATAFORMA YELLOW
#define COLOR_MARIO_BASE BLUE
#define COLOR_MARIO_DETALLE RED
#define COLOR_COCODRILO_ROJO RED
#define COLOR_COCODRILO_AZUL (Color){0, 191, 255, 255} // Azul brillante
#define COLOR_ISLA_VERDE (Color){34, 139, 34, 255}     // Verde
#define COLOR_ISLA_SUELO (Color){139, 69, 19, 255}     // Café suelo
#define COLOR_NARANJA (Color){255, 165, 0, 255}        // Naranja
#define COLOR_FRUTA_CELESTE (Color){135, 206, 250, 255} // Celeste
#define COLOR_FRUTA_ROJA (Color){220, 20, 60, 255}      // Rojo intenso
#define COLOR_BANANO (Color){255, 225, 0, 255}          // Amarillo banano

// Tipos de frutas
#define FRUTA_NARANJA 0
#define FRUTA_CELESTE 1
#define FRUTA_ROJA 2
#define FRUTA_BANANO 3

// Configuración de juego
#define VELOCIDAD_JUGADOR 3.0f
#define VELOCIDAD_TREPADO 2.5f
#define DISTANCIA_SALTO 60.0f
#define DISTANCIA_AGARRE_LIANA 15.0f

// Física del salto
#define GRAVEDAD 0.8f
#define FUERZA_SALTO -15.0f
#define VELOCIDAD_HORIZONTAL_Aire 3.0f
#define VELOCIDAD_HORIZONTAL_SUELO 4.0f
#define FRENADO_Aire 0.9f
#define ALTURA_MINIMA_SALTO 50.0f


// Configuración de red (placeholder para integración)
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

#endif