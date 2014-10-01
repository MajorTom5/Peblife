#include "pebble.h"
#define PEBLIFE_SIZE 10
	
//Some simple helper defines
#define P_CARRAY &(calcArray[0][0])
#define P_DARRAY &(dispArray[0][0])
#define ARRAYSIZE sizeof(bool)*(PEBLIFE_SIZE*PEBLIFE_SIZE)
	
static Window *window;

static Layer *layer;


static bool calcArray[PEBLIFE_SIZE][PEBLIFE_SIZE];
static bool dispArray[PEBLIFE_SIZE][PEBLIFE_SIZE];

static void layer_update_callback(Layer *me, GContext* ctx){
	
    GRect lBounds = layer_get_bounds(me);
    float rectWidth = lBounds.size.w/PEBLIFE_SIZE;
    float rectHeight = lBounds.size.h/PEBLIFE_SIZE;

	for(int i=0;i<PEBLIFE_SIZE;i++){
		for(int j=0;j<PEBLIFE_SIZE;j++){
                      
				if(dispArray[i][j]){
 					graphics_fill_rect(ctx,GRect(i*rectWidth,j*rectHeight,rectWidth,rectHeight),0,GCornerNone);
                }
        }
			
   }
  
				
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

static void peblife_initialize(){
	
	memset(P_CARRAY,0,ARRAYSIZE);//Set the calcArray to all zero's
	memset(P_DARRAY,0,ARRAYSIZE);//Set the dispArray to all zero's

    for(int i=1;i<(PEBLIFE_SIZE-1);i++){
		for(int j=1;j<(PEBLIFE_SIZE-1);j++){
        		dispArray[i][j] = (rand() % 2);//Generate a random number of 0 or 1
        }
}
	
	
	
}
int main(void) {
	
  
  //peblife_initialize();
	
  window = window_create();
  window_stack_push(window, true /* Animated */);

  // Init the layer for display the image
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, layer_update_callback);
  layer_add_child(window_layer, layer);
	
  //Begin the loop
  //while(1)
  //{
	app_event_loop();  
	
	for(int i = 0; i<10;i++){//DBG: Only do this 10 times in case soemthing happens.
	  //peblife_run_iteration();//Update the grid
	  peblife_initialize();//DBG: Simple hack to test display updates. The final version will call peblife_run_iteration()
	  
	  layer_mark_dirty(layer);
      
	}
	//}
	
  window_destroy(window);
  layer_destroy(layer);
}
