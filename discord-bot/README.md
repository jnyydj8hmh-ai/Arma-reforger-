# Bot de Discord

Bot sencillo con comandos: `!hola`, `!di`, `!ping`, `!dado`, `!servidor`.

## 1. Instalar

Necesitas Python 3.10 o superior.

```
pip install -r requirements.txt
```

## 2. Crear el bot en Discord

1. Entra en https://discord.com/developers/applications y pulsa **New Application**.
2. En el menu **Bot**, pulsa **Add Bot**.
3. Activa **MESSAGE CONTENT INTENT** (en la seccion *Privileged Gateway Intents*).
4. Pulsa **Reset Token** y copia el token. **No lo compartas con nadie.**

## 3. Poner el token

No escribas el token dentro del codigo. Pasalo como variable de entorno:

```
# Linux / Mac
export DISCORD_TOKEN="pega_aqui_tu_token"

# Windows (CMD)
set DISCORD_TOKEN=pega_aqui_tu_token
```

## 4. Invitar el bot a tu servidor

En el portal, menu **OAuth2 > URL Generator**:
- Scopes: marca `bot`.
- Bot Permissions: marca `Send Messages` y `Read Message History`.
- Copia el enlace de abajo, abrelo y elige tu servidor.

## 5. Ejecutar

```
python3 bot.py
```

Si sale "Conectado como ...", funciona. Escribe `!hola` en tu servidor.
