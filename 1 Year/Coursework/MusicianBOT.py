import youtube_dl
import discord
from async_timeout import timeout
from discord.ext import commands
import asyncio
import aiohttp

import config


class VoiceError(Exception):
    pass


# Класс, представляющий аудио-ресурс через библиотеку youtube_dl
# Хранит различную информацию о треке, в т.ч. длительность, ссылку на аудио, название, автора и т.д.
class YTDLSource:
    YTDL_OPTIONS = {
        'format': 'bestaudio/best',
        'extractaudio': True,
        'audioformat': 'mp3',
        'outtmpl': '%(extractor)s-%(id)s-%(title)s.%(ext)s',
        'restrictfilenames': True,
        'noplaylist': True,
        'nocheckcertificate': True,
        'ignoreerrors': False,
        'logtostderr': False,
        'quiet': True,
        'no_warnings': True,
        'default_search': 'auto',
        'source_address': '0.0.0.0',
        'skip_download': True,
        'simulate': True,
    }

    ytdl = youtube_dl.YoutubeDL(YTDL_OPTIONS)

    FFMPEG_OPTIONS = {
        'before_options': '-reconnect 1 -reconnect_streamed 1 -reconnect_delay_max 5',
        'options': '-vn',
    }

    def __init__(self, ctx: commands.Context, url: str):
        self.requester = ctx.author
        self.youtube_url = url

        self._data = self.ytdl.extract_info(url)
        self.source = discord.FFmpegPCMAudio(self._data.get('url'), **self.FFMPEG_OPTIONS)
        self.title = self._data.get('title')
        self.thumbnail = self._data.get('thumbnail')
        self.author = self._data.get('creator')
        self.duration = self._parse_duration(int(self._data.get('duration')))
        self.embed = self._create_embed()

    @staticmethod
    def _parse_duration(duration: int):
        minutes, seconds = divmod(duration, 60)
        hours, minutes = divmod(minutes, 60)
        days, hours = divmod(hours, 24)

        duration = []
        if days > 0:
            duration.append('{} days'.format(days))
        if hours > 0:
            duration.append('{} hours'.format(hours))
        if minutes > 0:
            duration.append('{} minutes'.format(minutes))
        if seconds > 0:
            duration.append('{} seconds'.format(seconds))

        return ', '.join(duration)

    def _create_embed(self):
        embed = discord.Embed(title='Now playing:',
                                color=self.requester.guild.get_member(bot.user.id).top_role.color)
        if self.author is not None:
            embed.add_field(name='Author', value=self.author)

        (embed.add_field(name='Name', value=self.title)
              .add_field(name='YouTube', value=self.youtube_url)
              .add_field(name='Requester', value=self.requester)
              .add_field(name='Duration', value=self.duration)
              .set_image(url=self.thumbnail))
        return embed


# Класс, представляющий голосовую связь бота.
# Содержит очередь и сопрограмму, реализующую аудио-плеер
class Voice:
    def __init__(self, mybot: commands.Bot, ctx: commands.Context):
        self.bot = mybot
        self.voice_client = ctx.guild.voice_client
        self.voice_channel = ctx.author.voice.channel
        self.songs = asyncio.Queue()
        self.next = asyncio.Event()
        self.audio_player = self.bot.loop.create_task(self.audio_player_task())
        self.current_song = None
        self.queue_mes = None
        self.song_mes = None
        self.music_text_channel = None

    async def audio_player_task(self):
        while True:
            self.next.clear()

            try:
                async with timeout(120):
                    self.current_song = await self.songs.get()
            except asyncio.TimeoutError:
                await self.music_text_channel.delete()
                await self.voice_client.disconnect()
                return

            self.voice_client.play(self.current_song.source, after=self.play_next_song)
            await self.song_mes.edit(embed=self.current_song.embed)
            await self.next.wait()
            content = str(self.queue_mes.content)
            content = content.replace(content[6:content.find(';') + 1], '')
            await self.queue_mes.edit(content=content)
            await self.song_mes.edit(embed=None)

    def play_next_song(self, error=None):
        if error:
            raise VoiceError(str(error))
        self.next.set()

    def skip(self):
        self.voice_client.stop()

    async def stop(self):
        self.songs = asyncio.Queue()

        if self.voice_client:
            await self.voice_client.disconnect()
            self.voice_client = None


# Класс, собирающий воедино все команды бота
class Music(commands.Cog):
    def __init__(self, mybot: commands.Bot):
        self.voicestates = {}
        self.tracks = {}
        self.bot = mybot

    @commands.command(aliases=['p'])
    async def play(self, ctx: commands.Context, *, search: str):
        """Play music from youtube.
        You can use url or any search request"""

        if ctx.guild.voice_client is None:
            await ctx.invoke(self.join)

        if 'www.youtube.com' in search:
            yt_source = YTDLSource(ctx, search)
        elif search.isdigit():
            if not self.tracks[ctx.guild.id]:
                await ctx.send('You must have selection to choose!')
                return
            if int(search) > 4:
                await ctx.send('Write a correct number!')
                return

            url = 'https://www.youtube.com/watch?v=' + self.tracks[ctx.guild.id][int(search) - 1]
            yt_source = YTDLSource(ctx, url)
            del self.tracks[ctx.guild.id]
            await ctx.invoke(clean, amount=5)
        else:
            async with aiohttp.ClientSession() as session:
                async with session.get('https://www.googleapis.com/youtube/v3/search?part=snippet&q='
                                       + search
                                       + '&key=' + config.GOOGLE_API_YT_KEY) as resp:
                    i = 1
                    trackslist = []
                    json_resp = await resp.json()
                    mes = 'Choose track from the given selection (send \"{}play <number>\" ):\n\n'.format(
                        bot.command_prefix)
                    for item in json_resp['items']:
                        if item['id']['kind'] == "youtube#video":
                            trackslist.append(item['id']['videoId'])
                            mes += '{}: {}\n'.format(i, item['snippet']['title'])
                            i += 1
                    await ctx.send(mes)
                    self.tracks[ctx.guild.id] = trackslist
                    return

        state = self.voicestates[ctx.guild.id]

        await state.songs.put(yt_source)
        await state.queue_mes.edit(content='{0.content}'.format(state.queue_mes) + '\n' + '  ' + yt_source.title + ';')

    @commands.command()
    async def pause(self, ctx: commands.Context):
        """Pauses current song"""

        state = self.voicestates[ctx.guild.id]
        state.voice_client.pause()

    @commands.command(aliases=['res'])
    async def resume(self, ctx: commands.Context):
        """Continues current song"""

        if not ctx.guild.voice_client:
            await ctx.send('I\'m not in voice channel currently!')
            raise commands.CommandInvokeError('Not in voice channel')
        ctx.guild.voice_client.resume()

    @commands.command(aliases=['s'])
    async def skip(self, ctx: commands.Context):
        """Skip current song"""

        self.voicestates[ctx.guild.id].skip()

    @commands.command()
    async def join(self, ctx: commands.Context):
        """Joins to your current voice channel"""

        try:
            await ctx.author.voice.channel.connect()
        except discord.errors.ClientException:
            await ctx.send('Already in voice channel!')
            return

        state = Voice(bot, ctx)
        self.voicestates[ctx.guild.id] = state
        print('Joined \'{0.name}\' voice channel'.format(state.voice_channel))

        if 'music_player' not in str(ctx.guild.channels):
            overwrites = {ctx.guild.default_role: discord.PermissionOverwrite(send_messages=False,
                                                                              add_reactions=False)}
            state.music_text_channel = await ctx.guild.create_text_channel('music_player', overwrites=overwrites)

        state.queue_mes = await state.music_text_channel.send('Queue:\n\n')
        state.song_mes = await state.music_text_channel.send('> \n> \n Current song:', embed=None)

    @commands.command()
    async def quit(self, ctx: commands.Context):
        """Stop and disconnect.
        Deletes the 'Music_player' text channel'"""

        if not ctx.guild.voice_client:
            await ctx.send('I\'m not in voice channel currently!')
            raise commands.CommandInvokeError('Not in voice channel')
        state = self.voicestates[ctx.guild.id]
        await state.music_text_channel.delete()
        await state.voice_client.disconnect()

    @play.before_invoke
    @join.before_invoke
    async def ensure_voice_state(self, ctx: commands.Context):
        if not ctx.author.voice or not ctx.author.voice.channel:
            await ctx.send('You are not connected to any voice channel.')
            raise commands.CommandError('You are not connected to any voice channel.')

    @skip.before_invoke
    @pause.before_invoke
    async def is_song_playing(self, ctx: commands.Context):
        if not ctx.guild.voice_client:
            await ctx.send('I\'m not in voice channel currently!')
            raise commands.CommandInvokeError('Not in voice channel')
        if not ctx.guild.voice_client.is_playing():
            await ctx.send('Nothing is playing currently!')
            raise commands.CommandInvokeError('Nothing is playing currently!')


# Точка входа
# Создание объекта бота, "подключение" музыкальных команд
# Запуск бота и Event loop через bot.run()
if __name__ == '__main__':
    bot = commands.Bot(command_prefix='>')
    bot.add_cog(Music(bot))

    @bot.command(aliases=['cls', 'clear'])
    async def clean(ctx: commands.Context, *, amount: int = 100):
        """Deletes messages to bot and from bot.
        You can optionally specify the number of messages to delete"""

        async for message in ctx.channel.history(limit=amount):
            if bot.command_prefix in message.content[0:3] or message.author == bot.user:
                await message.delete()

    @bot.event
    async def on_ready():
        print('Online as: {}'.format(bot.user))


    bot.run(config.BOT_TOKEN)
# Воспроизведение музыки из разных источников, включая YouTube.