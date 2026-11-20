#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Bot de Discord sencillo.

Antes de usarlo:
  1) Instala la libreria:   pip install discord.py
  2) Crea el bot y copia su TOKEN (ver README.md).
  3) Pon el token en la variable de entorno DISCORD_TOKEN, o en un archivo .env

Ejecutar:
  python3 bot.py
"""

import os
import random
import discord
from discord.ext import commands

# --- Token: se lee de la variable de entorno (nunca lo escribas aqui) ---
TOKEN = os.environ.get("DISCORD_TOKEN")

# Prefijo de los comandos: se escriben con "!" delante, p. ej. !hola
intents = discord.Intents.default()
intents.message_content = True  # necesario para leer los mensajes
bot = commands.Bot(command_prefix="!", intents=intents)


@bot.event
async def on_ready():
    print(f"Conectado como {bot.user} (id: {bot.user.id})")
    print("El bot esta listo.")


@bot.command(help="Responde con un saludo.")
async def hola(ctx):
    await ctx.send(f"Hola, {ctx.author.mention}!")


@bot.command(help="El bot repite lo que escribas. Ej: !di hola mundo")
async def di(ctx, *, texto: str):
    await ctx.send(texto)


@bot.command(help="Dice cuanto tarda el bot en responder.")
async def ping(ctx):
    ms = round(bot.latency * 1000)
    await ctx.send(f"Pong! {ms} ms")


@bot.command(help="Tira un dado. Ej: !dado  o  !dado 20")
async def dado(ctx, caras: int = 6):
    if caras < 2:
        await ctx.send("El dado necesita al menos 2 caras.")
        return
    await ctx.send(f"Salio un {random.randint(1, caras)} (d{caras}).")


@bot.command(help="Informacion del servidor.")
async def servidor(ctx):
    g = ctx.guild
    if not g:
        await ctx.send("Esto solo funciona dentro de un servidor.")
        return
    await ctx.send(f"Servidor: {g.name} | Miembros: {g.member_count}")


def main():
    if not TOKEN:
        print("ERROR: falta el token.")
        print("Pon tu token asi antes de ejecutar:")
        print('  Linux/Mac:  export DISCORD_TOKEN="tu_token"')
        print('  Windows:    set DISCORD_TOKEN=tu_token')
        return
    bot.run(TOKEN)


if __name__ == "__main__":
    main()
