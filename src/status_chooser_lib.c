
#include "status_chooser_lib.h"

char *status_chooser_transfer(char *status_chooser_name, const char *interface_name, const char *mode, const int current_round)  
{  
    int i;  
    int found = 0;  
    for (i = 0; i < status_chooser_num; i++) {  
        if (strcmp(status_chooser_name, status_chooser_name_array[i]) == 0) {  
            found = 1;  
            break;  
        }  
    }  
    if (!found) {  
        return "closed";  
    }  
  
    if (strcmp(status_chooser_name, "work_at_each_round") == 0) {  
        return work_at_each_round(interface_name, mode, current_round);  
    }  
    if (strcmp(status_chooser_name, "only_work_at_odd_round") == 0) {  
        return only_work_at_odd_round(interface_name, mode, current_round);  
    }  
    if (strcmp(status_chooser_name, "only_work_at_even_round") == 0) {  
        return only_work_at_even_round(interface_name, mode, current_round);  
    }  
  
    return "closed"; // This should never be reached, but kept for safety.  
}  
  
char* work_at_each_round(const char *interface_name, const char *mode, const int current_round) {    
    if (strcmp(mode, "test") == 0) {    
        // Decide based on half-duplex configuration and current round    
        return "sending"; // If odd rounds and configured for odd, send    
    }   
    if (strcmp(mode, "listen") == 0) {    
        return "receiving"; // If odd rounds and configured for odd, send    
    }    
  
    return "closed"; // Default status if interface type is not recognized    
}    
    
char* only_work_at_odd_round(const char *interface_name, const char *mode, const int current_round) {    
    if (strcmp(mode, "test") == 0) {    
        // Decide based on half-duplex configuration and current round    
        if (current_round % 2 == 1) {    
            return "sending"; // If odd rounds and configured for odd, send    
        }    
        return "receiving"; // If none of the above, receive    
    }   
    if (strcmp(mode, "listen") == 0) {    
        // Decide based on half-duplex configuration and current round    
        if (current_round % 2 == 1) {    
            return "receiving"; // If odd rounds and configured for odd, send    
        }    
        return "sending"; // If none of the above, receive    
    }    
  
    return "closed"; // Default status if interface type is not recognized    
}    
    
char* only_work_at_even_round(const char *interface_name, const char *mode, const int current_round) {    
    if (strcmp(mode, "test") == 0) {	  
        // Decide based on half-duplex configuration and current round    
        if (current_round % 2 == 0) {    
            return "sending"; // If even rounds and configured for even, send    
        }    
        return "receiving"; // If none of the above, receive	  
    }   
    if (strcmp(mode, "listen") == 0) {    
        // Decide based on half-duplex configuration and current round    
        if (current_round % 2 == 0) {    
            return "receiving"; // If even rounds and configured for even, listen    
        }    
        return "sending"; // If none of the above, receive    
    }    
    
    return "closed"; // Default status if interface type is not recognized    
}



