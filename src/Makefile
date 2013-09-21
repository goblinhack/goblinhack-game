#
#   ghack.
#   Copyright (C) 1999-2010 Neil McGill
#
#   This game is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Library General Public
#   License as published by the Free Software Foundation; either
#   version 2 of the License, or (at your option) any later version.
#
#   This game is distributed in the hope that it will be fun,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Library General Public License for more details.
#
#   You should have received a copy of the GNU Library General Public
#   License along with this game; if not, write to the Free
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#   Neil McGill
#
#   $Id: Makefile.in 1297 2009-08-24 13:50:50Z Goblinhack $
#

TOP=..

include $(TOP)/build/mk/config

SUBDIRS = levels rooms main ui debug io game sdl map math misc things player term unix resources

TARGET = ghack

LIBS += $(SYSLIBS) $(OBJS_SUBDIRS)

OBJS=

OBJS_SUBDIRS=\
	./debug/backtrace.o \
	./debug/debugit.o \
	./debug/debugit_instrumented_list.o \
	./debug/dmalloc.o \
	./game/game.o \
	./game/hiscore.o \
	./game/load.o \
	./game/loop.o \
	./game/save.o \
	./io/dir.o \
	./io/io.o \
	./io/zip.o \
	./levels/level.o \
	./levels/level_map.o \
	./main/global.o \
	./main/main.o \
	./main/updater.o \
	./main/subsys.o \
	./map/find.o \
	./map/fractal.o \
	./map/gfx.o \
	./map/map.o \
	./map/map_manip.o \
	./map/maze.o \
	./map/cave.o \
	./map/forest.o \
	./map/landscape.o \
	./map/hell.o \
	./map/void.o \
	./map/bonus.o \
	./map/shop.o \
	./math/line.o \
	./math/rand.o \
	./math/sine.o \
	./misc/gzequals.o \
	./misc/hacklib.o \
	./misc/ptr.o \
	./misc/string.o \
	./player/help.o \
	./player/quit.o \
	./player/info.o \
	./player/item.o \
	./player/keys.o \
	./player/loop.o \
	./player/editor.o \
	./player/pause.o \
	./player/player.o \
	./player/save.o \
	./player/score.o \
	./player/tick.o \
	./player/use.o \
	./resources/things/room/doors/broken_door.o \
	./resources/things/room/doors/cell_door.o \
	./resources/things/room/doors/portcullis.o \
	./resources/things/room/doors/fancy.o \
	./resources/things/room/doors/door.o \
	./resources/things/room/doors/electro_door.o \
	./resources/things/room/exit.o \
	./rooms/log.o \
	./rooms/parse.o \
	./rooms/room.o \
	./sdl/collage.o \
	./sdl/getpixel.o \
	./sdl/glshape.o \
	./sdl/key.o \
	./sdl/log.o \
	./sdl/loop.o \
	./sdl/thread.o \
	./sdl/pixel.o \
	./sdl/putpixel.o \
	./sdl/screen.o \
	./sdl/screendump.o \
	./sdl/surface.o \
	./sdl/audio.o \
	./sdl/music.o \
	./sdl/time.o \
	./sdl/window.o \
	./term/console.o \
	./term/console_screen.o \
	./term/figlet.o \
	./term/term.o \
	./things/audio.o \
	./things/actions.o \
	./things/anger.o \
	./things/possess.o \
	./things/anim.o \
	./things/armour.o \
	./things/ability.o \
	./things/water_proof.o \
	./things/fire_proof.o \
	./things/acid_proof.o \
	./things/cold_proof.o \
	./things/suffocation_proof.o \
	./things/teleport_proof.o \
	./things/poison_proof.o \
	./things/stone_proof.o \
	./things/attack.o \
	./things/polymorph.o \
	./things/bite.o \
	./things/can_see_distance.o \
	./things/chasing.o \
	./things/collect.o \
	./things/collision.o \
	./things/dead.o \
	./things/debug.o \
	./things/default.o \
	./things/drop.o \
	./things/bravery.o \
	./things/find.o \
	./things/followers.o \
	./things/gfx.o \
	./things/hit.o \
	./things/item.o \
	./things/insult.o \
	./things/util.o \
	./things/lifespan.o \
	./things/light_strength.o \
	./things/log.o \
	./things/meet.o \
	./things/exit.o \
	./things/metabolism.o \
	./things/monst.o \
	./things/move.o \
	./things/open.o \
	./things/parse.o \
	./things/ref.o \
	./things/ptr.o \
	./things/registration.o \
	./things/room.o \
	./things/scared.o \
	./things/shop.o \
	./things/sleeping.o \
	./things/shielded.o \
	./things/invisible.o \
	./things/poisoned.o \
	./things/stone.o \
	./things/spawn.o \
	./things/summon.o \
	./things/spell.o \
	./things/strength.o \
	./things/dex.o \
	./things/stamina.o \
	./things/teleport.o \
	./things/rage.o \
	./things/thing.o \
	./things/unique_things.o \
	./things/throwing.o \
	./things/tick.o \
	./things/transmog.o \
	./things/use.o \
	./things/vision.o \
	./things/weapon.o \
	./things/wear.o \
	./ui/hiscore.o \
	./ui/read_text.o \
	./ui/read_small_text.o \
	./ui/help_txt.o \
	./ui/intro.o \
	./ui/level_chooser.o \
	./ui/config.o \
	./ui/level_end.o \
	./ui/level_start.o \
	./ui/messages.o \
	./ui/player_chooser.o \
	./ui/player_class_chooser.o \
	./ui/level_type_chooser.o \
	./ui/player_powerup_chooser.o \
	./ui/player_auto_save.o \
	./ui/player_croaked_it.o \
	./ui/player_dead.o \

$(TARGET) : $(OBJS_SUBDIRS) 

include $(TOP)/build/mk/program

depend:
	../build/sh/makedep.sh

