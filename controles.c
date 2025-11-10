#include "raylib.h"
#include "controles.h"
#include "config.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void actualizar_controles(Controles *ctrl) {
    ctrl->izquierda = IsKeyDown(KEY_LEFT);
    ctrl->derecha = IsKeyDown(KEY_RIGHT);
    ctrl->arriba = IsKeyDown(KEY_UP);
    ctrl->abajo = IsKeyDown(KEY_DOWN);
    ctrl->espacio = IsKeyDown(KEY_SPACE);
}

void aplicar_movimiento(EstadoJuego *estado, Controles *ctrl) {
    Jugador *j = &estado->jugador;
    
    // Resetear estado de suelo al inicio del frame
    j->en_suelo = false;
    
    // Aplicar gravedad si no está en suelo o agarrado de liana
    if (j->estado != ESTADO_AGARRADO_LIANA) {
        j->velocidad_y += GRAVEDAD;
    }
    
    switch (j->estado) {
        case ESTADO_SUELO:
            j->puede_saltar = true;
            j->tiempo_en_aire = 0;
            
            // Movimiento horizontal en suelo
            j->velocidad_x = 0;
            if (ctrl->izquierda) {
                j->velocidad_x = -VELOCIDAD_HORIZONTAL_SUELO;
            }
            if (ctrl->derecha) {
                j->velocidad_x = VELOCIDAD_HORIZONTAL_SUELO;
            }
            
            // Iniciar salto
            if (ctrl->espacio && j->puede_saltar) {
                j->estado = ESTADO_SALTANDO;
                j->velocidad_y = FUERZA_SALTO;
                j->puede_saltar = false;
                j->tiempo_en_aire = 0;
                
                // Dirección inicial del salto
                if (ctrl->izquierda) j->velocidad_x = -VELOCIDAD_HORIZONTAL_Aire;
                else if (ctrl->derecha) j->velocidad_x = VELOCIDAD_HORIZONTAL_Aire;
            }
            
            // Agarre a liana
            if (ctrl->arriba) {
                int liana_cercana = liana_mas_cercana(j, estado->lianas, estado->num_lianas);
                if (liana_cercana != -1) {
                    j->estado = ESTADO_AGARRADO_LIANA;
                    j->liana_actual = liana_cercana;
                    j->x = estado->lianas[liana_cercana].x;
                    j->velocidad_y = 0;
                    j->velocidad_x = 0;
                }
            }
            break;
            
        case ESTADO_SALTANDO:
        case ESTADO_CAYENDO:
            j->tiempo_en_aire++;
            
            // Control en el aire (más limitado)
            if (ctrl->izquierda) {
                j->velocidad_x = -VELOCIDAD_HORIZONTAL_Aire;
            } else if (ctrl->derecha) {
                j->velocidad_x = VELOCIDAD_HORIZONTAL_Aire;
            } else {
                // Frenado gradual en el aire
                j->velocidad_x *= FRENADO_Aire;
                if (fabs(j->velocidad_x) < 0.1f) j->velocidad_x = 0;
            }
            
            // Transición de salto a caída
            if (j->velocidad_y > 0 && j->estado == ESTADO_SALTANDO) {
                j->estado = ESTADO_CAYENDO;
            }
            
            // Agarre a liana durante el salto/caída
            if (ctrl->arriba && j->velocidad_y >= 0) { // Solo agarrar cuando está cayendo
                int liana_cercana = liana_mas_cercana(j, estado->lianas, estado->num_lianas);
                if (liana_cercana != -1) {
                    j->estado = ESTADO_AGARRADO_LIANA;
                    j->liana_actual = liana_cercana;
                    j->x = estado->lianas[liana_cercana].x;
                    j->velocidad_y = 0;
                    j->velocidad_x = 0;
                    break;
                }
            }
            break;
            
        case ESTADO_AGARRADO_LIANA:
            j->velocidad_x = 0;
            j->velocidad_y = 0;
            
            // Movimiento vertical en liana
            if (ctrl->arriba && j->y > estado->lianas[j->liana_actual].y_inicio) {
                j->y -= VELOCIDAD_TREPADO;
            }
            if (ctrl->abajo && j->y < estado->lianas[j->liana_actual].y_fin) {
                j->y += VELOCIDAD_TREPADO;
            }
            
            // Salto desde liana
            if (ctrl->espacio) {
                j->estado = ESTADO_SALTANDO;
                j->velocidad_y = FUERZA_SALTO * 0.7f; // Salto más débil desde liana
                j->velocidad_x = (ctrl->derecha ? VELOCIDAD_HORIZONTAL_Aire : 
                                 (ctrl->izquierda ? -VELOCIDAD_HORIZONTAL_Aire : 0));
                j->liana_actual = -1;
                j->puede_saltar = false;
            }
            
            // Cambio entre lianas
            if (ctrl->derecha || ctrl->izquierda) {
                int nueva_liana = -1;
                float direccion = ctrl->derecha ? 1.0f : -1.0f;
                
                for (int i = 0; i < estado->num_lianas; i++) {
                    if (fabs(estado->lianas[i].x - estado->lianas[j->liana_actual].x - direccion * 80) < 25) {
                        nueva_liana = i;
                        break;
                    }
                }
                
                if (nueva_liana != -1) {
                    j->liana_actual = nueva_liana;
                    j->x = estado->lianas[nueva_liana].x;
                } else {
                    // Soltarse si no hay liana adyacente
                    j->estado = ESTADO_CAYENDO;
                    j->velocidad_y = 0;
                    j->velocidad_x = direccion * 2.0f;
                    j->liana_actual = -1;
                }
            }
            
            // Soltarse al fondo de la liana
            if (ctrl->abajo && j->y >= estado->lianas[j->liana_actual].y_fin - 10) {
                j->estado = ESTADO_CAYENDO;
                j->velocidad_y = 0;
                j->liana_actual = -1;
            }
            break;
    }
    
    // Aplicar velocidades
    j->x += j->velocidad_x;
    j->y += j->velocidad_y;
    
    // Limites horizontales
    if (j->x < JUGADOR_SIZE/2) {
        j->x = JUGADOR_SIZE/2;
        j->velocidad_x = 0;
    }
    if (j->x > SCREEN_WIDTH - JUGADOR_SIZE/2) {
        j->x = SCREEN_WIDTH - JUGADOR_SIZE/2;
        j->velocidad_x = 0;
    }
    
    // Verificar colisiones DURANTE el movimiento (en el aire)
    verificar_colisiones_en_movimiento(estado);
    
    // Verificar si aterriza en una superficie
    j->en_suelo = esta_en_superficie(j, estado);
    if (j->en_suelo && j->velocidad_y > 0) {
        j->velocidad_y = 0;
        j->estado = ESTADO_SUELO;
        j->puede_saltar = true;
    }
}