#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define UUID_SIZE 16
#define UUID_FILTER { 0xFA, 0xFF }

int main() {
    int device_id = hci_get_route(NULL);
    int socket = hci_open_dev(device_id);
    if (socket < 0) {
        perror("Failed to open HCI socket");
        exit(1);
    }

    	hci_send_cmd(device_id, OGF_HOST_CTL, OCF_RESET, 0, 0);


    // Configura o filtro para capturar apenas eventos de anúncio
    struct hci_filter filter;
    hci_filter_clear(&filter);
    hci_filter_set_ptype(HCI_EVENT_PKT, &filter);
    hci_filter_set_event(EVT_LE_META_EVENT, &filter);

    if (setsockopt(socket, SOL_HCI, HCI_FILTER, &filter, sizeof(filter)) < 0) {
        perror("Failed to set socket options");
        close(socket);
        exit(1);
    }

    // Loop para receber e tratar os eventos
    while (1) {
        unsigned char buf[HCI_MAX_EVENT_SIZE];
        int len = read(socket, buf, sizeof(buf));
        if (len >= HCI_EVENT_HDR_SIZE) {
            evt_le_meta_event* meta_event = (evt_le_meta_event*)(buf + HCI_EVENT_HDR_SIZE + 1);
            if (meta_event->subevent == EVT_LE_ADVERTISING_REPORT) {
                le_advertising_info* info = (le_advertising_info*)(meta_event->data + 1);
                //if (info->length >= UUID_SIZE && memcmp(info->data + 9, UUID_FILTER, UUID_SIZE) == 0) {
                    // Processa os dados do dispositivo com o UUID 0xFFFA
                    printf("Dispositivo encontrado com UUID 0xFFFA:\n");
                    char addr[18];
                    ba2str(&info->bdaddr, addr);
                    printf("Endereço: %s\n", addr);
                    // Outros dados do dispositivo
                    // ...
                //}
            }
        }
    }

    close(socket);
    return 0;
}
