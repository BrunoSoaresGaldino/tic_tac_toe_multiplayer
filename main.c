#include <allegro.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 300
#define CELL_SIZE 100

typedef struct
{
    int x;
    
    int y;
    
    int width;
    
    int height;
    
    bool selected;
    
    int owner;
    
} Cell;

typedef struct
{
    
    Cell cells[9];
    
} Tabuleiro;

int UserInput( Tabuleiro *tab , int mouse_x, int mouse_y, int player )
{
    int i = 0;
    
    for( i = 0 ; i < 9 ; i++ )
    {
        
        if( mouse_x >= tab->cells[i].x && 
            mouse_x <= ( tab->cells[i].x + tab->cells[i].width ) && 
            mouse_y >= tab->cells[i].y && mouse_y <= ( tab->cells[i].y + tab->cells[i].height) &&
            !tab->cells[i].selected
          )
        {
            tab->cells[i].selected = true;
            
            tab->cells[i].owner = player;
          
            return i;
        }
        
        
    }
    
    return -1;
    
}

int CheckWin( Tabuleiro *tabuleiro )
{
    int i;
    
    for( i = 0 ; i < 9; i += 3 )
    {
        if( tabuleiro->cells[i].owner != -1 && tabuleiro->cells[i].owner == tabuleiro->cells[i+1].owner && tabuleiro->cells[i].owner == tabuleiro->cells[i+2].owner)  
        {
            return tabuleiro->cells[i].owner;
        }
        
    }
    
    for( i = 0 ; i < 3; i++ )
    {
        if( tabuleiro->cells[i].owner != -1 && tabuleiro->cells[i].owner == tabuleiro->cells[i+3].owner && tabuleiro->cells[i].owner == tabuleiro->cells[i+6].owner)  
        {
            return tabuleiro->cells[i].owner;
        }
        
    }
    
    if( tabuleiro->cells[0].owner != -1 && tabuleiro->cells[0].owner == tabuleiro->cells[4].owner && tabuleiro->cells[0].owner == tabuleiro->cells[8].owner)
    {
        return tabuleiro->cells[0].owner;
    }
    
    if( tabuleiro->cells[2].owner != -1 && tabuleiro->cells[2].owner == tabuleiro->cells[4].owner && tabuleiro->cells[2].owner == tabuleiro->cells[6].owner)
    {
        return tabuleiro->cells[2].owner;
    }
    
    for( i = 0 ; i < 9; i++ )
    {
        if( tabuleiro->cells[i].owner == -1 )
        {
            return -1;//ninguém venceu
        }
    }
    
    if( i == 9)
    {
        return 2;// empate
    }
    
}

int main( void )
{
    int i,j;
    
    int cell = -1;
    
    int player = 0;
    
    int winner;

    Tabuleiro tab;
    
    bool exit_program = false;
    
    BITMAP *buffer;
    BITMAP *tabuleiro;
    BITMAP *arrow[2];
   
    allegro_init( );
    
    install_timer( );
    install_mouse( );
    install_keyboard( );
    
    

    set_window_title("Jogo da velha multiplayer");
    
    set_color_depth(24);
    
    if( set_gfx_mode( GFX_AUTODETECT_WINDOWED , SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 ) )
    {
        fputs("Erro ao inicializar a allegro",stderr);
        exit(EXIT_FAILURE);
    }
    
    buffer = create_bitmap( SCREEN_WIDTH , SCREEN_HEIGHT );
    
    if( !buffer )
    {
        allegro_message("Nao foi possivel criar o buffer de tela");
        
        exit_program = true;
    }
    
    tabuleiro = load_bitmap("tabuleiro.bmp",NULL);
    
    if( !tabuleiro )
    {
        allegro_message("Nao foi possivel carregar o tabuleiro");
        
        exit_program = true;
    
    }
    
    arrow[0] = load_bitmap("red_arrow.bmp",NULL);
    
    if( !arrow[0] )
    {
        allegro_message("Nao foi possivel carregar o ponteiro do mouse");
    
        exit_program = true;
    }
    
    arrow[1] = load_bitmap("green_arrow.bmp",NULL);
    
    if( !arrow[1] )
    {
        allegro_message("Nao foi possivel carregar o ponteiro do mouse");
       
        exit_program = true;
    }
    
    
    for( i = 0, j = 0; i < 9; i++ )
    {
        
        if( i && !( i%3) )
        {
            j += CELL_SIZE;
            
        }
        
        tab.cells[i].x = (i%3) * CELL_SIZE;
        tab.cells[i].y = j;
        
        tab.cells[i].width = CELL_SIZE;
        tab.cells[i].height = CELL_SIZE;
        
        tab.cells[i].owner = -1;
        tab.cells[i].selected = false;
        
       
    }
    
    
    while( !exit_program )
    {
        
        if(key[KEY_ESC])
        {
            exit_program = true;
        }
        
        winner = CheckWin(&tab);
        
        if( winner == 0 )
        {
            allegro_message("Player 1 ganhou");
            
            exit_program = true;
            
        }

        else if( winner == 1 )
        {
            allegro_message("Player 2 ganhou");
            
            exit_program= true;
            
        }
        else if( winner == 2 )
        {
            allegro_message("Empate");
            
            exit_program = true;
            
        }
            
        
        clear_to_color(buffer, makecol(255,255,255) );
        
        if( mouse_b )
        {   
            cell =  UserInput(&tab,mouse_x,mouse_y,player) ;
            
            if( cell != -1)
            {
                player = !player;
            
            }
        }
        
        for( i = 0 ; i < 9; i++ )
        {
           
            if( tab.cells[i].selected )
            {
                rectfill
                ( 
                    buffer,
                    tab.cells[i].x,
                    tab.cells[i].y,
                    tab.cells[i].x + tab.cells[i].width,
                    tab.cells[i].y + tab.cells[i].height,
                    tab.cells[i].owner ? makecol(0, 128, 0 ):makecol(128, 0 ,0 )
                );
            }
            
        }
        
        draw_sprite( buffer, tabuleiro , 0 , 0 );
        
        draw_sprite( buffer, arrow[player], mouse_x , mouse_y );
        
        draw_sprite( screen,buffer, 0, 0 ); 
        
       
        
    }
    
    destroy_bitmap(buffer);
    destroy_bitmap(tabuleiro);
    destroy_bitmap(arrow[0]);
    destroy_bitmap(arrow[1]);
    
    return 0;
}
END_OF_MAIN( )