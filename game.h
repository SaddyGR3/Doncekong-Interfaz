#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "config.h"

typedef enum {
    ESTADO_SUELO,
    ESTADO_AGARRADO_LIANA,
    ESTADO_SALTANDO,
    ESTADO_CAYENDO
} EstadoJugador;

typedef struct {
    float x, y;
    float velocidad_x, velocidad_y;
    int vidas;
    int puntuacion;
    bool activo;
    EstadoJugador estado;
    int liana_actual;
    bool en_suelo;
    bool puede_saltar;
    float tiempo_en_aire;
} Jugador;

typedef struct {
    float x, y;
    bool activo;
} Padre;

typedef struct {
    float x, y;
    int tipo;  // 0: rojo, 1: azul
    bool activo;
} Cocodrilo;

typedef struct {
    float x, y;
    int puntos;
    int tipo;  // 0: naranja, 1: celeste, 2: roja, 3: banano
    bool activo;
} Fruta;

typedef struct {
    float x, y_inicio, y_fin;
} Liana;

typedef struct {
    float x, y, ancho;
} Plataforma;

typedef struct {
    float x, y, ancho;
    Color color_superior, color_inferior;
} Isla;


typedef struct {
    int tipo;  // 0: vacío, 1: plataforma, 2: liana, 3: agua, 4: fruta, 5: cocodrilo, 6: jugador
    int datos_extra; // para tipo de fruta, tipo de cocodrilo, etc.
} CeldaMatriz;

typedef struct {
    CeldaMatriz celdas[MATRIZ_FILAS][MATRIZ_COLUMNAS];
} MatrizJuego;

typedef struct {
    Jugador jugador;
    Padre padre;
    Cocodrilo cocodrilos[50];
    Fruta frutas[30];
    Liana lianas[20];
    Plataforma plataforma_superior;
    Isla islas[10];
    MatrizJuego matriz; // ← AGREGAR ESTA LÍNEA
    int num_cocodrilos;
    int num_frutas;
    int num_lianas;
    int num_islas;
    bool juego_activo;
} EstadoJuego;


void inicializar_juego(EstadoJuego *estado);
bool esta_en_isla(Jugador *jugador, Isla islas[], int num_islas);
int liana_mas_cercana(Jugador *jugador, Liana lianas[], int num_lianas);
void verificar_colisiones(EstadoJuego *estado);
bool esta_en_superficie(Jugador *jugador, EstadoJuego *estado);
void inicializar_matriz(MatrizJuego *matriz, EstadoJuego *estado);
void verificar_colisiones_matriz(EstadoJuego *estado, MatrizJuego *matriz);

#endif