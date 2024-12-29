        }
    }

    bytemaps.bytemapInodos[inodo_libre] = 1;
    strcpy(directorio[entrada_libre].archivo, destino);
    directorio[entrada_libre].inodo = inodo_libre;

    printf("Fichero copiado\n");
}

void comando_rename(char *antiguo, char *nuevo) {
    for (int i = 0; i < 20; i++) {		// Es hasta 20 porque en la partición hay hasta 20 archivos
        if (strcmp(directorio[i].archivo, nuevo) == 0) {
            printf("Error: Ese nombre ya existe\n");
            return;
        }
    }

    for (int i = 0; i < 20; i++) {
        if (strcmp(directorio[i].archivo, antiguo) == 0) {
            strcpy(directorio[i].archivo, nuevo);
            printf("Fichero renombrado\n");
            return;
        }
    }

    printf("Error: El fichero origen no existe\n");
}
