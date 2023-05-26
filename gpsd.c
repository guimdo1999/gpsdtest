#include <stdio.h>
#include <gps.h>

int main() {
    struct gps_data_t gpsdata;
    
    // Inicializa a conexão com o GPS
    if (gps_open("localhost", "2947", &gpsdata) < 0) {
        fprintf(stderr, "Falha ao abrir a conexão com o GPS.\n");
        return 1;
    }
    
    // Configura o modo de operação do GPS
    gps_stream(&gpsdata, WATCH_ENABLE | WATCH_JSON, NULL);
    
    // Loop principal
    while (1) {
        // Aguarda os dados do GPS
        if (gps_waiting(&gpsdata, 5000000)) {
            // Lê os dados do GPS
            if (gps_read(&gpsdata, NULL, 0) == -1) {
                fprintf(stderr, "Falha ao ler dados do GPS.\n");
                break;
            }
            
            printf("Socket selecionado corretamente.\n");
            break;
        } else {
            fprintf(stderr, "Socket não está pronto, aguardando...\n");
        }
    }
    
    // Fecha a conexão com o GPS
    gps_stream(&gpsdata, WATCH_DISABLE, NULL);
    gps_close(&gpsdata);
    
    return 0;
}
