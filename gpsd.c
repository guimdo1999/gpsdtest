#include <stdio.h>
#include <gps.h>
#include <unistd.h>

#define GPSD_BUFFER_SIZE 512

int main() {
    struct gps_data_t gpsdata;
    char gpsd_message[GPSD_BUFFER_SIZE];
    
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
            if (gps_read(&gpsdata, gpsd_message, sizeof(gpsd_message)) == -1) {
                fprintf(stderr, "Falha ao ler dados do GPS.\n");
                break;
            }
            printf("Socket selecionado corretamente.\n");
            
            // Imprime os dados de latitude e longitude
            if (gpsdata.fix.mode >= MODE_2D) {
                printf("Latitude: %.6f\n", gpsdata.fix.latitude);
                printf("Longitude: %.6f\n", gpsdata.fix.longitude);
            }
        } else {
            fprintf(stderr, "Socket não está pronto, aguardando...\n");
        }
        
        // Aguarda 1 segundo
        sleep(1);
    }
    
    // Fecha a conexão com o GPS
    gps_stream(&gpsdata, WATCH_DISABLE, NULL);
    gps_close(&gpsdata);
    
    return 0;
}
