#include "raylib.h"
#include "graficos.h"
#include "config.h"
#include "game.h"
#include <stdio.h>
#include <string.h>

SistemaSprites sprites_global;

void inicializar_graficos(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Donkey Kong Jr");
    SetTargetFPS(FPS);
    cargar_sprites(&sprites_global);
}

void cargar_sprites(SistemaSprites *sprites) {
    memset(sprites, 0, sizeof(SistemaSprites));
    
    printf("=== CARGANDO SPRITES ===\n");
    
    // FONDO - escalar al tamaño de la ventana
    /*
    if (FileExists(SPRITE_FONDO)) {
        Image img_fondo = LoadImage(SPRITE_FONDO);
        // Escalar imagen al tamaño de la ventana
        ImageResize(&img_fondo, SCREEN_WIDTH, SCREEN_HEIGHT);
        sprites->fondo.textura = LoadTextureFromImage(img_fondo);
        UnloadImage(img_fondo);
        sprites->fondo.cargado = true;
        printf("✓ Fondo cargado y escalado: %s -> %dx%d\n", SPRITE_FONDO, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    */
    
    // JUGADOR - DK Jr (personaje principal)
    if (FileExists(SPRITE_DKJ)) {
        sprites->jugador.textura = LoadTexture(SPRITE_DKJ);
        sprites->jugador.cargado = true;
        printf("✓ Jugador (DK Jr) cargado: %s\n", SPRITE_DKJ);
    }
    
    // PADRE - Donkey Kong
    if (FileExists(SPRITE_DK)) {
        sprites->padre.textura = LoadTexture(SPRITE_DK);
        sprites->padre.cargado = true;
        printf("✓ Padre (DK) cargado: %s\n", SPRITE_DK);
    }
    
    // COCODRILOS
    if (FileExists(SPRITE_COCODRILO_ROJO)) {
        sprites->cocodrilo_rojo.textura = LoadTexture(SPRITE_COCODRILO_ROJO);
        sprites->cocodrilo_rojo.cargado = true;
        printf("✓ Cocodrilo rojo cargado: %s\n", SPRITE_COCODRILO_ROJO);
    }
    
    if (FileExists(SPRITE_COCODRILO_AZUL)) {
        sprites->cocodrilo_azul.textura = LoadTexture(SPRITE_COCODRILO_AZUL);
        sprites->cocodrilo_azul.cargado = true;
        printf("✓ Cocodrilo azul cargado: %s\n", SPRITE_COCODRILO_AZUL);
    }
    
    // FRUTAS
    if (FileExists(SPRITE_FRUTA_MANZANA)) {
        sprites->fruta_naranja.textura = LoadTexture(SPRITE_FRUTA_MANZANA);
        sprites->fruta_naranja.cargado = true;
        printf("✓ Fruta naranja cargada: %s\n", SPRITE_FRUTA_MANZANA);
    }
    
    if (FileExists(SPRITE_FRUTA_PLATANO)) {
        sprites->fruta_banano.textura = LoadTexture(SPRITE_FRUTA_PLATANO);
        sprites->fruta_banano.cargado = true;
        printf("✓ Fruta plátano cargada: %s\n", SPRITE_FRUTA_PLATANO);
    }
    
    if (FileExists(SPRITE_FRUTA_PERAS)) {
        sprites->fruta_celeste.textura = LoadTexture(SPRITE_FRUTA_PERAS);
        sprites->fruta_celeste.cargado = true;
        printf("✓ Fruta celeste cargada: %s\n", SPRITE_FRUTA_PERAS);
    }
    
    // Fruta roja usa manzana también
    if (FileExists(SPRITE_FRUTA_MANZANA)) {
        sprites->fruta_roja.textura = LoadTexture(SPRITE_FRUTA_MANZANA);
        sprites->fruta_roja.cargado = true;
    }
}

// ==================== FUNCIONES DE DIBUJO CON ESCALADO ====================

void dibujar_jugador_con_sprite(Jugador *j, SistemaSprites *sprites) {
    if (j->activo) {
        if (sprites->jugador.cargado) {
            // DK Jr - tamaño mediano
            Rectangle dest = {
                j->x - SPRITE_JUGADOR_SIZE/2, 
                j->y - SPRITE_JUGADOR_SIZE/2,
                SPRITE_JUGADOR_SIZE, 
                SPRITE_JUGADOR_SIZE
            };
            DrawTexturePro(
                sprites->jugador.textura,
                (Rectangle){0, 0, sprites->jugador.textura.width, sprites->jugador.textura.height},
                dest,
                (Vector2){0, 0}, 0, WHITE
            );
        } else {
            // Fallback
            DrawRectangle(j->x - JUGADOR_HITBOX/2, j->y - JUGADOR_HITBOX/2, 
                         JUGADOR_HITBOX, JUGADOR_HITBOX, COLOR_JUGADOR);
        }
    }
}

void dibujar_padre_con_sprite(Padre *p, SistemaSprites *sprites) {
    if (p->activo) {
        if (sprites->padre.cargado) {
            // DK padre - mas grande
            Rectangle dest = {
                p->x - SPRITE_PADRE_SIZE/2, 
                p->y - SPRITE_PADRE_SIZE/2,
                SPRITE_PADRE_SIZE, 
                SPRITE_PADRE_SIZE
            };
            DrawTexturePro(
                sprites->padre.textura,
                (Rectangle){0, 0, sprites->padre.textura.width, sprites->padre.textura.height},
                dest,
                (Vector2){0, 0}, 0, WHITE
            );
        } else {
            DrawRectangle(p->x - JUGADOR_SIZE/2, p->y - JUGADOR_SIZE/2, 
                         JUGADOR_SIZE, JUGADOR_SIZE, COLOR_PADRE);
        }
    }
}

void dibujar_cocodrilo_con_sprite(Cocodrilo *c, SistemaSprites *sprites) {
    if (c->activo) {
        Texture2D *textura = NULL;
        
        if (c->tipo == 0 && sprites->cocodrilo_rojo.cargado) {
            textura = &sprites->cocodrilo_rojo.textura;
        } else if (c->tipo == 1 && sprites->cocodrilo_azul.cargado) {
            textura = &sprites->cocodrilo_azul.textura;
        }
        
        if (textura) {
            Rectangle dest = {
                c->x - SPRITE_COCODRILO_SIZE/2, 
                c->y - SPRITE_COCODRILO_SIZE/2,
                SPRITE_COCODRILO_SIZE, 
                SPRITE_COCODRILO_SIZE
            };
            DrawTexturePro(
                *textura,
                (Rectangle){0, 0, textura->width, textura->height},
                dest,
                (Vector2){0, 0}, 0, WHITE
            );
        } else {
            Color color = (c->tipo == 0) ? COLOR_COCODRILO_ROJO : COLOR_COCODRILO_AZUL;
            DrawRectangle(c->x - COCODRILO_HITBOX/2, c->y - COCODRILO_HITBOX/2,
                         COCODRILO_HITBOX, COCODRILO_HITBOX, color);
        }
    }
}

void dibujar_fruta_con_sprite(Fruta *f, SistemaSprites *sprites) {
    if (f->activo) {
        Texture2D *textura = NULL;
        
        switch (f->tipo) {
            case FRUTA_NARANJA: 
                if (sprites->fruta_naranja.cargado) textura = &sprites->fruta_naranja.textura;
                break;
            case FRUTA_ROJA:
                if (sprites->fruta_roja.cargado) textura = &sprites->fruta_roja.textura;
                break;
            case FRUTA_CELESTE:
                if (sprites->fruta_celeste.cargado) textura = &sprites->fruta_celeste.textura;
                break;
            case FRUTA_BANANO:
                if (sprites->fruta_banano.cargado) textura = &sprites->fruta_banano.textura;
                break;
        }
        
        if (textura) {
            Rectangle dest = {
                f->x - SPRITE_FRUTA_SIZE/2, 
                f->y - SPRITE_FRUTA_SIZE/2,
                SPRITE_FRUTA_SIZE, 
                SPRITE_FRUTA_SIZE
            };
            DrawTexturePro(
                *textura,
                (Rectangle){0, 0, textura->width, textura->height},
                dest,
                (Vector2){0, 0}, 0, WHITE
            );
        } else {
            Color color_fruta;
            switch (f->tipo) {
                case FRUTA_NARANJA: color_fruta = COLOR_NARANJA; break;
                case FRUTA_CELESTE: color_fruta = COLOR_FRUTA_CELESTE; break;
                case FRUTA_ROJA: color_fruta = COLOR_FRUTA_ROJA; break;
                case FRUTA_BANANO: color_fruta = COLOR_BANANO; break;
                default: color_fruta = GREEN;
            }
            DrawRectangle(f->x - FRUTA_HITBOX/2, f->y - FRUTA_HITBOX/2,
                         FRUTA_HITBOX, FRUTA_HITBOX, color_fruta);
        }
    }
}

// ==================== FUNCION PRINCIPAL ====================

void dibujar_escena_completa(EstadoJuego *estado, Mapa *mapa, SistemaSprites *sprites) {
    BeginDrawing();
    
    // 1. Fondo del mapa (ya escalado al tamaño de ventana)
    dibujar_mapa(mapa);
    
    // 2. Superficies debug
    if (DEBUG_COLISIONES) {
        dibujar_superficies_debug(mapa);
    }
    
    // 3. Elementos del juego
    dibujar_padre_con_sprite(&estado->padre, sprites);
    
    for (int i = 0; i < estado->num_frutas; i++) {
        dibujar_fruta_con_sprite(&estado->frutas[i], sprites);
    }
    
    for (int i = 0; i < estado->num_cocodrilos; i++) {
        dibujar_cocodrilo_con_sprite(&estado->cocodrilos[i], sprites);
    }
    
    // Jugador último para que esté encima
    dibujar_jugador_con_sprite(&estado->jugador, sprites);
    
    // 4. UI
    dibujar_ui(estado);
    
    // 5. Debug info
    if (DEBUG_COLISIONES) {
        DrawText("DEBUG: ON (Cambia DEBUG_COLISIONES en config.h)", 10, SCREEN_HEIGHT - 30, 20, RED);
        DrawText(TextFormat("DK Jr: (%.1f, %.1f)", estado->jugador.x, estado->jugador.y), 
                10, SCREEN_HEIGHT - 60, 20, DARKGRAY);
    }
    
    EndDrawing();
}

// ==================== FUNCIONES RESTANTES ====================

