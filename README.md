# Arma Reforger – ServerTools

Pequeño mod en Enforce Script + configuración de servidor dedicado.

## Contenido

| Archivo | Qué hace |
|---|---|
| `Scripts/Game/ServerTools/ST_ServerTools.c` | Registra en el log del servidor las conexiones y reapariciones de cada jugador, y cada 5 minutos imprime la lista de jugadores conectados. |
| `Scripts/Game/ServerTools/ST_NameTags.c` | Muestra el nombre (y la distancia) de los jugadores encima de su cabeza. Por defecto solo aliados, hasta 60 m y sin ver a través de paredes. Los ajustes están al principio del archivo. |
| `server/config.json` | Configuración de ejemplo para un servidor dedicado (Conflict en Everon, 32 jugadores). |

## Usar el mod

1. Abre **Arma Reforger Tools** → Workbench → *Create New Project* (dependencia: `ArmaReforger`).
2. Copia la carpeta `Scripts/` dentro de la carpeta del proyecto.
3. En el Script Editor pulsa **Compile and Reload** (o *Build → Compile*). Comprueba que no hay errores.
4. Prueba en el World Editor con cualquier escenario; verás en la consola líneas `[ServerTools] ...`.
5. Para publicarlo: *Workshop → Publish*. Después añade su ID al array `mods` del `config.json`:
   ```json
   "mods": [ { "modId": "TU_ID_DEL_MOD", "name": "ServerTools" } ]
   ```

## Lanzar el servidor

```bash
ArmaReforgerServer -config ./server/config.json -profile ./profile -maxFPS 60
```

Cambia `passwordAdmin` antes de usarlo. Ajusta el intervalo del informe con `ST_STATUS_INTERVAL_MS`.
