#include <pebble.h>

Window *my_window;
TextLayer *text_layer;

void gett(char *out,int state){
  time_t temp = time(NULL); 
  struct tm *t = localtime(&temp);
  char ti[2];
  ti[0]='0';
  ti[1]='0';
  int n,r[2],i,d=0,e=1;
  if (!state)n=t->tm_hour; else n=t->tm_min;
    for(i=0;i<2;i++)
    {
        r[i]=n%16;
        n=n/16;
        d=d+(r[i]*e);
        e=e*10;
    }
    for(i=1;i>=0;i--)
    {
        if(r[i]==10)
            ti[i]='A';
        else if(r[i]==11)
            ti[i]='B';
        else if(r[i]==12)
            ti[i]='C';
        else if(r[i]==13)
            ti[i]='D';
        else if(r[i]==14)
            ti[i]='E';
        else if(r[i]==15)
            ti[i]='F';
      else ti[i]=r[i]+(int)'0';
    }
  //place the variables in their spot
  if(!state){
    out[0]=ti[1];
    out[1]=ti[0];
  }else{
    out[3]=ti[1];
    out[4]=ti[0];
  }
}

char * convertToHexTime(struct tm * currtime){
    char* time=malloc(sizeof(char)*5);
    for(int i=0;i<5;i++){time[i]=0;}
  //just a placeholder will have first digit
    gett(time,0);
    time[2]=':'; 
    gett(time,1);
  return time;
}
static void handler(struct tm *tick_time, TimeUnits units_changed) {
    char* time=convertToHexTime(tick_time);
    text_layer_set_text(text_layer, time);
    free(time);
}
//does first two digit converstion

void handle_init(void) {
  my_window = window_create();
  char initial[5]="00:00";
  text_layer = text_layer_create(GRect(0, 50, 144, 80));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer,GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_text(text_layer,initial);
  text_layer_set_font(text_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONTMAIN_44)));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_layer));
  tick_timer_service_subscribe(MINUTE_UNIT, handler);
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}
int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
