#include "pebble.h"
#define PEBLIFE_SIZE 45
#define P_CARRAY &(calcArray[0][0])
#define P_DARRAY &(dispArray[0][0])
#define ARRAYSIZE sizeof(bool)*(PEBLIFE_SIZE*PEBLIFE_SIZE)
	
static Window *window;

static Layer *layer;


static bool calcArray[PEBLIFE_SIZE][PEBLIFE_SIZE];
static bool dispArray[PEBLIFE_SIZE][PEBLIFE_SIZE];

static void layer_update_callback(Layer *me, GContext* ctx) {
  // We make sure the dimensions of the GRect to draw into
  // are equal to the size of the bitmap--otherwise the image
  // will automatically tile. Which might be what *you* want.

  GRect bounds = image->bounds;

  graphics_draw_bitmap_in_rect(ctx, image, (GRect) { .origin = { 5, 5 }, .size = bounds.size });

  graphics_draw_bitmap_in_rect(ctx, image, (GRect) { .origin = { 80, 60 }, .size = bounds.size });
}

static void peblife_run_iteration(){//Run one iteration of the Conway's game of life simulation. Note:
	for(int i=1;i<(PEBLIFE_SIZE-1);i++){// This implementation has very basic boundary condition handling. Any 
		for(int j=1;j<(PEBLIFE_SIZE-1);j++){//cells before the second array element are simply not calculated.
					
			//Retrieve the collective statuses of the surrounding neighbors
			int neighborCount = dispArray[i-1][j+1]+dispArray[i][j+1]+dispArray[i+1][j+1]+dispArray[i-1][j]+dispArray[i+1][j]+dispArray[i-1][j-1]+dispArray[i][j-1]+dispArray[i+1][j-1];
			
			if(dispArray[i][j])//If the cell is alive
			{
				if(neighborCount < 2){ calcArray[i][j] = 0; }//Death: Undercrowding
				if(neighborCount == 2 || neighborCount == 3){ calcArray[i][j] = 1; } //Survival
				if(neighborCount > 3){ calcArray[i][j] = 0; }//Death: Overcrowding
			}
			else{ //He's dead, Jim.
				if(neighborCount == 3){ calcArray[i][j] = 1; }//Birth
			}
			//Now calcArray contains the generation to push to the screen.
			memcpy(P_CARRAY,P_DARRAY,ARRAYSIZE); //Copy calcArray to dispArray
			
		}
	}
}
static void peblife_display_array(){
	
	app_event_loop();
}
static void peblife_initialize(){
	
	memset(P_CARRAY,0,ARRAYSIZE);//Set the calcArray to all zero's
	memset(P_DARRAY,0,ARRAYSIZE);//Set the dispArray to all zero's
	
	
	
	
}
int main(void) {
    
  peblife_initialize();
	
  window = window_create();
  window_stack_push(window, true /* Animated */);

  // Init the layer for display the image
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, layer_update_callback);
  layer_add_child(window_layer, layer);
	
  //Begin the loop
  while(1)
  {
	  peblife_run_iteration();//Update the grid
	  peblife_display_array();
  }	
	
  

  //image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NO_LITTER);

  

  //gbitmap_destroy(image);

  window_destroy(window);
  layer_destroy(layer);
}
