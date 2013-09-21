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

# DO NOT DELETE

debug/backtrace.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
debug/backtrace.o: ./include/debugit_instrumented_list.h
debug/backtrace.o: ./include/gh_version.h ./include/gh_main.h
debug/backtrace.o: ./include/gh_sys.h ./include/gh_macro.h
debug/backtrace.o: ./include/gh_hash.h ./include/gh_mathdef.h
debug/backtrace.o: ./include/gh_backtrace.h ./include/gh_debug.h
debug/backtrace.o: ./include/gh_io.h ./include/gh_global.h
debug/backtrace.o: ./include/gh_stream.h ./include/gh_templates.h
debug/backtrace.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
debug/backtrace.o: ./include/gh_string.h ./include/gh_point.h
debug/debugit.o: ./include/debugit.h ./include/debugit_instrumented_list.h
game/game.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
game/game.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
game/game.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
game/game.o: ./include/gh_hash.h ./include/gh_mathdef.h
game/game.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
game/game.o: ./include/gh_global.h ./include/gh_stream.h
game/game.o: ./include/gh_templates.h ./include/gh_size.h
game/game.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
game/game.o: ./include/gh_point.h ./include/gh_game.h ./include/gh_messages.h
game/game.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
game/game.o: ./include/gh_model.h ./include/gh_thing_ptr.h
game/game.o: ./include/gh_player.h ./include/gh_subsys.h
game/game.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_screen.h
game/game.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
game/game.o: ./include/gh_hiscore.h ./include/gh_level_map.h
game/hiscore.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
game/hiscore.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
game/hiscore.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
game/hiscore.o: ./include/gh_hash.h ./include/gh_mathdef.h
game/hiscore.o: ./include/gh_backtrace.h ./include/gh_debug.h
game/hiscore.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
game/hiscore.o: ./include/gh_templates.h ./include/gh_size.h
game/hiscore.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
game/hiscore.o: ./include/gh_point.h ./include/gh_thing.h
game/hiscore.o: ./include/gh_model.h ./include/gh_thing_ptr.h
game/hiscore.o: ./include/gh_player.h ./include/gh_subsys.h
game/hiscore.o: ./include/gh_ui_event.h ./include/gh_sdl.h
game/hiscore.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
game/hiscore.o: ./include/gh_surface.h ./include/gh_hiscore.h
game/hiscore.o: ./include/gh_zip.h
game/load.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
game/load.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
game/load.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
game/load.o: ./include/gh_hash.h ./include/gh_mathdef.h
game/load.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
game/load.o: ./include/gh_global.h ./include/gh_stream.h
game/load.o: ./include/gh_templates.h ./include/gh_size.h
game/load.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
game/load.o: ./include/gh_point.h ./include/gh_game.h ./include/gh_messages.h
game/load.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
game/load.o: ./include/gh_model.h ./include/gh_thing_ptr.h
game/load.o: ./include/gh_player.h ./include/gh_subsys.h
game/load.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_screen.h
game/load.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
game/load.o: ./include/gh_ptr.h ./include/gh_room.h
game/load.o: ./include/gh_thing_registration.h
game/loop.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
game/loop.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
game/loop.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
game/loop.o: ./include/gh_hash.h ./include/gh_mathdef.h
game/loop.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
game/loop.o: ./include/gh_global.h ./include/gh_stream.h
game/loop.o: ./include/gh_templates.h ./include/gh_size.h
game/loop.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
game/loop.o: ./include/gh_point.h ./include/gh_game.h ./include/gh_messages.h
game/loop.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
game/loop.o: ./include/gh_model.h ./include/gh_thing_ptr.h
game/loop.o: ./include/gh_player.h ./include/gh_subsys.h
game/loop.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_screen.h
game/loop.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
game/loop.o: ./include/gh_hiscore.h ./include/gh_audio.h ./include/gh_music.h
game/save.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
game/save.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
game/save.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
game/save.o: ./include/gh_hash.h ./include/gh_mathdef.h
game/save.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
game/save.o: ./include/gh_global.h ./include/gh_stream.h
game/save.o: ./include/gh_templates.h ./include/gh_size.h
game/save.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
game/save.o: ./include/gh_point.h ./include/gh_game.h ./include/gh_messages.h
game/save.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
game/save.o: ./include/gh_model.h ./include/gh_thing_ptr.h
game/save.o: ./include/gh_player.h ./include/gh_subsys.h
game/save.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_screen.h
game/save.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
game/save.o: ./include/gh_math.h
io/dir.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
io/dir.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
io/dir.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
io/dir.o: ./include/gh_hash.h ./include/gh_mathdef.h ./include/gh_backtrace.h
io/dir.o: ./include/gh_debug.h ./include/gh_io.h ./include/gh_global.h
io/dir.o: ./include/gh_stream.h ./include/gh_templates.h ./include/gh_size.h
io/dir.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
io/dir.o: ./include/gh_point.h
io/io.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
io/io.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
io/io.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
io/io.o: ./include/gh_hash.h ./include/gh_mathdef.h ./include/gh_backtrace.h
io/io.o: ./include/gh_debug.h ./include/gh_io.h ./include/gh_global.h
io/io.o: ./include/gh_stream.h ./include/gh_templates.h ./include/gh_size.h
io/io.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
io/io.o: ./include/gh_point.h ./include/gh_zip.h ./include/gh_lexical_cast.h
io/zip.o: io/zip.h ./../build/config.h ./include/debugit.h
io/zip.o: ./include/debugit_instrumented_list.h ./include/gh_sys.h
io/zip.o: ./include/gh_macro.h ./include/gh_hash.h ./include/gh_mathdef.h
io/zip.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_string.h
levels/level.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
levels/level.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
levels/level.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
levels/level.o: ./include/gh_hash.h ./include/gh_mathdef.h
levels/level.o: ./include/gh_backtrace.h ./include/gh_debug.h
levels/level.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
levels/level.o: ./include/gh_templates.h ./include/gh_size.h
levels/level.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
levels/level.o: ./include/gh_point.h ./include/gh_thing.h
levels/level.o: ./include/gh_model.h ./include/gh_thing_ptr.h
levels/level.o: ./include/gh_player.h ./include/gh_subsys.h
levels/level.o: ./include/gh_ui_event.h ./include/gh_sdl.h
levels/level.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
levels/level.o: ./include/gh_surface.h ./include/gh_game.h
levels/level.o: ./include/gh_messages.h ./include/gh_level.h
levels/level.o: ./include/gh_map.h ./include/gh_maze.h ./include/gh_room.h
levels/level.o: ./include/gh_zip.h ./include/gh_math.h ./include/gh_fractal.h
levels/level.o: ./include/gh_array2d.h ./include/gh_term.h ./include/gh_ptr.h
levels/level.o: ./include/gh_level_map.h ./include/gh_thing_registration.h
levels/level_map.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
levels/level_map.o: ./include/debugit_instrumented_list.h
levels/level_map.o: ./include/gh_version.h ./include/gh_main.h
levels/level_map.o: ./include/gh_sys.h ./include/gh_macro.h
levels/level_map.o: ./include/gh_hash.h ./include/gh_mathdef.h
levels/level_map.o: ./include/gh_backtrace.h ./include/gh_debug.h
levels/level_map.o: ./include/gh_io.h ./include/gh_global.h
levels/level_map.o: ./include/gh_stream.h ./include/gh_templates.h
levels/level_map.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
levels/level_map.o: ./include/gh_string.h ./include/gh_point.h
levels/level_map.o: ./include/gh_player.h ./include/gh_subsys.h
levels/level_map.o: ./include/gh_ui_event.h ./include/gh_sdl.h
levels/level_map.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
levels/level_map.o: ./include/gh_surface.h ./include/gh_level_map.h
levels/level_map.o: ./include/gh_zip.h
main/global.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
main/global.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
main/global.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
main/global.o: ./include/gh_hash.h ./include/gh_mathdef.h
main/global.o: ./include/gh_backtrace.h ./include/gh_debug.h
main/global.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
main/global.o: ./include/gh_templates.h ./include/gh_size.h
main/global.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
main/global.o: ./include/gh_point.h ./include/gh_screen.h
main/global.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
main/global.o: ./include/gh_math.h
main/main.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
main/main.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
main/main.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
main/main.o: ./include/gh_hash.h ./include/gh_mathdef.h
main/main.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
main/main.o: ./include/gh_global.h ./include/gh_stream.h
main/main.o: ./include/gh_templates.h ./include/gh_size.h
main/main.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
main/main.o: ./include/gh_point.h ./include/gh_game.h ./include/gh_messages.h
main/main.o: ./include/gh_thing.h ./include/gh_model.h
main/main.o: ./include/gh_thing_ptr.h ./include/gh_room.h ./include/gh_map.h
main/main.o: ./include/gh_zip.h ./include/gh_surface.h
main/main.o: ./include/gh_getput_pixel.h ./include/gh_audio.h
main/main.o: ./include/gh_music.h ./include/gh_sdl.h
main/subsys.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
main/subsys.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
main/subsys.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
main/subsys.o: ./include/gh_hash.h ./include/gh_mathdef.h
main/subsys.o: ./include/gh_backtrace.h ./include/gh_debug.h
main/subsys.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
main/subsys.o: ./include/gh_templates.h ./include/gh_size.h
main/subsys.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
main/subsys.o: ./include/gh_point.h ./include/gh_subsys.h
main/subsys.o: ./include/gh_ui_event.h ./include/gh_sdl.h
main/updater.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
main/updater.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
main/updater.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
main/updater.o: ./include/gh_hash.h ./include/gh_mathdef.h
main/updater.o: ./include/gh_backtrace.h ./include/gh_debug.h
main/updater.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
main/updater.o: ./include/gh_templates.h ./include/gh_size.h
main/updater.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
main/updater.o: ./include/gh_point.h ./include/gh_game.h
main/updater.o: ./include/gh_messages.h ./include/gh_thing.h
main/updater.o: ./include/gh_model.h ./include/gh_thing_ptr.h
main/updater.o: ./include/gh_room.h ./include/gh_map.h ./include/gh_zip.h
main/updater.o: ./include/gh_surface.h ./include/gh_getput_pixel.h
main/updater.o: ./include/hacklib.h ./include/gh_audio.h ./include/gh_music.h
main/updater.o: ./include/gh_sdl.h
map/bonus.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/bonus.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/bonus.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/bonus.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/bonus.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/bonus.o: ./include/gh_global.h ./include/gh_stream.h
map/bonus.o: ./include/gh_templates.h ./include/gh_size.h
map/bonus.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
map/bonus.o: ./include/gh_point.h ./include/gh_maze.h ./include/gh_room.h
map/bonus.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
map/bonus.o: ./include/gh_thing_ptr.h
map/cave.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/cave.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/cave.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/cave.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/cave.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/cave.o: ./include/gh_global.h ./include/gh_stream.h
map/cave.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/cave.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/cave.o: ./include/gh_maze.h ./include/gh_room.h ./include/gh_map.h
map/cave.o: ./include/gh_thing.h ./include/gh_model.h
map/cave.o: ./include/gh_thing_ptr.h
map/find.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/find.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/find.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/find.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/find.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/find.o: ./include/gh_global.h ./include/gh_stream.h
map/find.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/find.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/find.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
map/find.o: ./include/gh_thing_ptr.h
map/forest.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/forest.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/forest.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/forest.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/forest.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/forest.o: ./include/gh_global.h ./include/gh_stream.h
map/forest.o: ./include/gh_templates.h ./include/gh_size.h
map/forest.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
map/forest.o: ./include/gh_point.h ./include/gh_maze.h ./include/gh_room.h
map/forest.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
map/forest.o: ./include/gh_thing_ptr.h
map/fractal.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/fractal.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/fractal.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/fractal.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/fractal.o: ./include/gh_backtrace.h ./include/gh_debug.h
map/fractal.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
map/fractal.o: ./include/gh_templates.h ./include/gh_size.h
map/fractal.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
map/fractal.o: ./include/gh_point.h ./include/gh_fractal.h
map/fractal.o: ./include/gh_array2d.h ./include/gh_math.h
map/gfx.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/gfx.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/gfx.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/gfx.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/gfx.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/gfx.o: ./include/gh_global.h ./include/gh_stream.h
map/gfx.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/gfx.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/gfx.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
map/gfx.o: ./include/gh_model.h ./include/gh_thing_ptr.h ./include/gh_game.h
map/gfx.o: ./include/gh_messages.h ./include/gh_room.h ./include/gh_term.h
map/gfx.o: ./include/gh_player.h ./include/gh_subsys.h
map/gfx.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_screen.h
map/gfx.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
map/gfx.o: ./include/gh_ui_thing.h ./include/gh_math.h ./include/gh_console.h
map/gfx.o: ./include/gh_collage.h ./include/gh_console_screen.h
map/hell.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/hell.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/hell.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/hell.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/hell.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/hell.o: ./include/gh_global.h ./include/gh_stream.h
map/hell.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/hell.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/hell.o: ./include/gh_maze.h ./include/gh_room.h ./include/gh_map.h
map/hell.o: ./include/gh_thing.h ./include/gh_model.h
map/hell.o: ./include/gh_thing_ptr.h
map/landscape.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/landscape.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/landscape.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/landscape.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/landscape.o: ./include/gh_backtrace.h ./include/gh_debug.h
map/landscape.o: ./include/gh_io.h ./include/gh_global.h
map/landscape.o: ./include/gh_stream.h ./include/gh_templates.h
map/landscape.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
map/landscape.o: ./include/gh_string.h ./include/gh_point.h
map/landscape.o: ./include/gh_maze.h ./include/gh_room.h ./include/gh_map.h
map/landscape.o: ./include/gh_thing.h ./include/gh_model.h
map/landscape.o: ./include/gh_thing_ptr.h ./include/gh_fractal.h
map/landscape.o: ./include/gh_array2d.h
map/map.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/map.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/map.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/map.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/map.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/map.o: ./include/gh_global.h ./include/gh_stream.h
map/map.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/map.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/map.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
map/map.o: ./include/gh_model.h ./include/gh_thing_ptr.h ./include/gh_maze.h
map/map.o: ./include/gh_room.h
map/map_manip.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/map_manip.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/map_manip.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/map_manip.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/map_manip.o: ./include/gh_backtrace.h ./include/gh_debug.h
map/map_manip.o: ./include/gh_io.h ./include/gh_global.h
map/map_manip.o: ./include/gh_stream.h ./include/gh_templates.h
map/map_manip.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
map/map_manip.o: ./include/gh_string.h ./include/gh_point.h
map/map_manip.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
map/map_manip.o: ./include/gh_thing_ptr.h ./include/gh_level.h
map/maze.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/maze.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/maze.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/maze.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/maze.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/maze.o: ./include/gh_global.h ./include/gh_stream.h
map/maze.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/maze.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/maze.o: ./include/gh_maze.h ./include/gh_room.h ./include/gh_map.h
map/maze.o: ./include/gh_thing.h ./include/gh_model.h
map/maze.o: ./include/gh_thing_ptr.h ./include/gh_math.h ./include/hacklib.h
map/shop.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/shop.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/shop.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/shop.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/shop.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/shop.o: ./include/gh_global.h ./include/gh_stream.h
map/shop.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/shop.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/shop.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
map/shop.o: ./include/gh_model.h ./include/gh_thing_ptr.h ./include/gh_room.h
map/void.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
map/void.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
map/void.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
map/void.o: ./include/gh_hash.h ./include/gh_mathdef.h
map/void.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
map/void.o: ./include/gh_global.h ./include/gh_stream.h
map/void.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
map/void.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
map/void.o: ./include/gh_maze.h ./include/gh_room.h ./include/gh_map.h
map/void.o: ./include/gh_thing.h ./include/gh_model.h
map/void.o: ./include/gh_thing_ptr.h
math/line.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
math/line.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
math/line.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
math/line.o: ./include/gh_hash.h ./include/gh_mathdef.h
math/line.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
math/line.o: ./include/gh_global.h ./include/gh_stream.h
math/line.o: ./include/gh_templates.h ./include/gh_size.h
math/line.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
math/line.o: ./include/gh_point.h ./include/gh_math.h
math/rand.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
math/rand.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
math/rand.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
math/rand.o: ./include/gh_hash.h ./include/gh_mathdef.h
math/rand.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
math/rand.o: ./include/gh_global.h ./include/gh_stream.h
math/rand.o: ./include/gh_templates.h ./include/gh_size.h
math/rand.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
math/rand.o: ./include/gh_point.h ./include/gh_math.h
math/sine.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
math/sine.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
math/sine.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
math/sine.o: ./include/gh_hash.h ./include/gh_mathdef.h
math/sine.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
math/sine.o: ./include/gh_global.h ./include/gh_stream.h
math/sine.o: ./include/gh_templates.h ./include/gh_size.h
math/sine.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
math/sine.o: ./include/gh_point.h ./include/gh_math.h
misc/gzequals.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
misc/gzequals.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
misc/gzequals.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
misc/gzequals.o: ./include/gh_hash.h ./include/gh_mathdef.h
misc/gzequals.o: ./include/gh_backtrace.h ./include/gh_debug.h
misc/gzequals.o: ./include/gh_io.h ./include/gh_global.h
misc/gzequals.o: ./include/gh_stream.h ./include/gh_templates.h
misc/gzequals.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
misc/gzequals.o: ./include/gh_string.h ./include/gh_point.h
misc/hacklib.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
misc/hacklib.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
misc/hacklib.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
misc/hacklib.o: ./include/gh_hash.h ./include/gh_mathdef.h
misc/hacklib.o: ./include/gh_backtrace.h ./include/gh_debug.h
misc/hacklib.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
misc/hacklib.o: ./include/gh_templates.h ./include/gh_size.h
misc/hacklib.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
misc/hacklib.o: ./include/gh_point.h ./include/hacklib.h
misc/ptr.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
misc/ptr.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
misc/ptr.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
misc/ptr.o: ./include/gh_hash.h ./include/gh_mathdef.h
misc/ptr.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
misc/ptr.o: ./include/gh_global.h ./include/gh_stream.h
misc/ptr.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
misc/ptr.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
misc/ptr.o: ./include/gh_ptr.h
misc/string.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
misc/string.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
misc/string.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
misc/string.o: ./include/gh_hash.h ./include/gh_mathdef.h
misc/string.o: ./include/gh_backtrace.h ./include/gh_debug.h
misc/string.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
misc/string.o: ./include/gh_templates.h ./include/gh_size.h
misc/string.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
misc/string.o: ./include/gh_point.h
player/editor.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/editor.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/editor.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/editor.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/editor.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/editor.o: ./include/gh_io.h ./include/gh_global.h
player/editor.o: ./include/gh_stream.h ./include/gh_templates.h
player/editor.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
player/editor.o: ./include/gh_string.h ./include/gh_point.h
player/editor.o: ./include/hacklib.h ./include/gh_player.h
player/editor.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/editor.o: ./include/gh_sdl.h ./include/gh_screen.h
player/editor.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/editor.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
player/editor.o: ./include/gh_model.h ./include/gh_thing_ptr.h
player/editor.o: ./include/gh_game.h ./include/gh_messages.h
player/editor.o: ./include/gh_term.h ./include/gh_audio.h ./include/gh_math.h
player/help.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/help.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/help.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/help.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/help.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/help.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/help.o: ./include/gh_templates.h ./include/gh_size.h
player/help.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/help.o: ./include/gh_point.h ./include/gh_player.h
player/help.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/help.o: ./include/gh_sdl.h ./include/gh_screen.h
player/help.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/help.o: ./include/gh_game.h ./include/gh_messages.h
player/help.o: ./include/gh_term.h
player/info.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/info.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/info.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/info.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/info.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/info.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/info.o: ./include/gh_templates.h ./include/gh_size.h
player/info.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/info.o: ./include/gh_point.h ./include/gh_player.h
player/info.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/info.o: ./include/gh_sdl.h ./include/gh_screen.h
player/info.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/info.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
player/info.o: ./include/gh_model.h ./include/gh_thing_ptr.h
player/info.o: ./include/gh_game.h ./include/gh_messages.h
player/info.o: ./include/gh_term.h
player/item.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/item.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/item.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/item.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/item.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/item.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/item.o: ./include/gh_templates.h ./include/gh_size.h
player/item.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/item.o: ./include/gh_point.h ./include/gh_player.h
player/item.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/item.o: ./include/gh_sdl.h ./include/gh_screen.h
player/item.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/item.o: ./include/gh_term.h ./include/gh_thing.h ./include/gh_model.h
player/item.o: ./include/gh_thing_ptr.h ./include/gh_game.h
player/item.o: ./include/gh_messages.h
player/keys.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/keys.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/keys.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/keys.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/keys.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/keys.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/keys.o: ./include/gh_templates.h ./include/gh_size.h
player/keys.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/keys.o: ./include/gh_point.h ./include/gh_thing.h ./include/gh_model.h
player/keys.o: ./include/gh_thing_ptr.h ./include/gh_level.h
player/keys.o: ./include/gh_map.h ./include/gh_subsys.h
player/keys.o: ./include/gh_ui_event.h ./include/gh_sdl.h
player/keys.o: ./include/gh_player.h ./include/gh_screen.h
player/keys.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/keys.o: ./include/gh_game.h ./include/gh_messages.h
player/keys.o: ./include/gh_term.h ./include/gh_audio.h
player/keys.o: ./include/gh_sdl_event.h ./include/gh_music.h
player/loop.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/loop.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/loop.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/loop.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/loop.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/loop.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/loop.o: ./include/gh_templates.h ./include/gh_size.h
player/loop.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/loop.o: ./include/gh_point.h ./include/hacklib.h ./include/gh_player.h
player/loop.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/loop.o: ./include/gh_sdl.h ./include/gh_screen.h
player/loop.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/loop.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
player/loop.o: ./include/gh_model.h ./include/gh_thing_ptr.h
player/loop.o: ./include/gh_game.h ./include/gh_messages.h
player/loop.o: ./include/gh_term.h ./include/gh_audio.h ./include/gh_math.h
player/pause.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/pause.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/pause.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/pause.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/pause.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/pause.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/pause.o: ./include/gh_templates.h ./include/gh_size.h
player/pause.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/pause.o: ./include/gh_point.h ./include/gh_player.h
player/pause.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/pause.o: ./include/gh_sdl.h ./include/gh_screen.h
player/pause.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/pause.o: ./include/gh_game.h ./include/gh_messages.h
player/pause.o: ./include/gh_term.h
player/player.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/player.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/player.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/player.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/player.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/player.o: ./include/gh_io.h ./include/gh_global.h
player/player.o: ./include/gh_stream.h ./include/gh_templates.h
player/player.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
player/player.o: ./include/gh_string.h ./include/gh_point.h
player/player.o: ./include/gh_player.h ./include/gh_subsys.h
player/player.o: ./include/gh_ui_event.h ./include/gh_sdl.h
player/player.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
player/player.o: ./include/gh_surface.h ./include/gh_level.h
player/player.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
player/player.o: ./include/gh_thing_ptr.h ./include/gh_game.h
player/player.o: ./include/gh_messages.h
player/quit.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/quit.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/quit.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/quit.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/quit.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/quit.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/quit.o: ./include/gh_templates.h ./include/gh_size.h
player/quit.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/quit.o: ./include/gh_point.h ./include/gh_player.h
player/quit.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/quit.o: ./include/gh_sdl.h ./include/gh_screen.h
player/quit.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/quit.o: ./include/gh_game.h ./include/gh_messages.h
player/quit.o: ./include/gh_term.h
player/save.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/save.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/save.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/save.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/save.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/save.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/save.o: ./include/gh_templates.h ./include/gh_size.h
player/save.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/save.o: ./include/gh_point.h ./include/gh_player.h
player/save.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/save.o: ./include/gh_sdl.h ./include/gh_screen.h
player/save.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/save.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
player/save.o: ./include/gh_model.h ./include/gh_thing_ptr.h
player/save.o: ./include/gh_game.h ./include/gh_messages.h
player/score.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/score.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/score.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/score.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/score.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/score.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/score.o: ./include/gh_templates.h ./include/gh_size.h
player/score.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/score.o: ./include/gh_point.h ./include/gh_player.h
player/score.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/score.o: ./include/gh_sdl.h ./include/gh_screen.h
player/score.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/score.o: ./include/gh_term.h ./include/gh_thing.h ./include/gh_model.h
player/score.o: ./include/gh_thing_ptr.h ./include/gh_level.h
player/score.o: ./include/gh_map.h ./include/gh_figlet.h
player/tick.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/tick.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/tick.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/tick.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/tick.o: ./include/gh_backtrace.h ./include/gh_debug.h
player/tick.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
player/tick.o: ./include/gh_templates.h ./include/gh_size.h
player/tick.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/tick.o: ./include/gh_point.h ./include/gh_player.h
player/tick.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/tick.o: ./include/gh_sdl.h ./include/gh_screen.h
player/tick.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/tick.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
player/tick.o: ./include/gh_model.h ./include/gh_thing_ptr.h
player/tick.o: ./include/gh_game.h ./include/gh_messages.h
player/use.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
player/use.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
player/use.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
player/use.o: ./include/gh_hash.h ./include/gh_mathdef.h
player/use.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
player/use.o: ./include/gh_global.h ./include/gh_stream.h
player/use.o: ./include/gh_templates.h ./include/gh_size.h
player/use.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
player/use.o: ./include/gh_point.h ./include/gh_player.h
player/use.o: ./include/gh_subsys.h ./include/gh_ui_event.h
player/use.o: ./include/gh_sdl.h ./include/gh_screen.h
player/use.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
player/use.o: ./include/gh_term.h ./include/gh_thing.h ./include/gh_model.h
player/use.o: ./include/gh_thing_ptr.h ./include/gh_game.h
player/use.o: ./include/gh_messages.h
rooms/log.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
rooms/log.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
rooms/log.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
rooms/log.o: ./include/gh_hash.h ./include/gh_mathdef.h
rooms/log.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
rooms/log.o: ./include/gh_global.h ./include/gh_stream.h
rooms/log.o: ./include/gh_templates.h ./include/gh_size.h
rooms/log.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
rooms/log.o: ./include/gh_point.h ./include/gh_sdl.h ./include/gh_room.h
rooms/log.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
rooms/log.o: ./include/gh_thing_ptr.h
rooms/parse.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
rooms/parse.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
rooms/parse.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
rooms/parse.o: ./include/gh_hash.h ./include/gh_mathdef.h
rooms/parse.o: ./include/gh_backtrace.h ./include/gh_debug.h
rooms/parse.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
rooms/parse.o: ./include/gh_templates.h ./include/gh_size.h
rooms/parse.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
rooms/parse.o: ./include/gh_point.h ./include/gh_term.h ./include/gh_room.h
rooms/parse.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
rooms/parse.o: ./include/gh_thing_ptr.h ./include/gh_console.h
rooms/parse.o: ./include/gh_collage.h ./include/gh_surface.h
rooms/parse.o: ./include/gh_getput_pixel.h ./include/gh_math.h
rooms/room.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
rooms/room.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
rooms/room.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
rooms/room.o: ./include/gh_hash.h ./include/gh_mathdef.h
rooms/room.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
rooms/room.o: ./include/gh_global.h ./include/gh_stream.h
rooms/room.o: ./include/gh_templates.h ./include/gh_size.h
rooms/room.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
rooms/room.o: ./include/gh_point.h ./include/gh_room.h ./include/gh_map.h
rooms/room.o: ./include/gh_thing.h ./include/gh_model.h
rooms/room.o: ./include/gh_thing_ptr.h ./include/gh_surface.h
rooms/room.o: ./include/gh_getput_pixel.h ./include/gh_collage.h
rooms/room.o: ./include/gh_math.h ./include/gh_sdl.h
sdl/audio.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/audio.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/audio.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/audio.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/audio.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/audio.o: ./include/gh_global.h ./include/gh_stream.h
sdl/audio.o: ./include/gh_templates.h ./include/gh_size.h
sdl/audio.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/audio.o: ./include/gh_point.h ./include/gh_audio.h
sdl/collage.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/collage.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/collage.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/collage.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/collage.o: ./include/gh_backtrace.h ./include/gh_debug.h
sdl/collage.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
sdl/collage.o: ./include/gh_templates.h ./include/gh_size.h
sdl/collage.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/collage.o: ./include/gh_point.h ./include/gh_collage.h
sdl/collage.o: ./include/gh_surface.h ./include/gh_getput_pixel.h
sdl/getpixel.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/getpixel.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/getpixel.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/getpixel.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/getpixel.o: ./include/gh_backtrace.h ./include/gh_debug.h
sdl/getpixel.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
sdl/getpixel.o: ./include/gh_templates.h ./include/gh_size.h
sdl/getpixel.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/getpixel.o: ./include/gh_point.h ./include/gh_surface.h
sdl/getpixel.o: ./include/gh_getput_pixel.h
sdl/getpixel.o: ./include/gh_getput_pixel_macros.h
sdl/glshape.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/glshape.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/glshape.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/glshape.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/glshape.o: ./include/gh_backtrace.h ./include/gh_debug.h
sdl/glshape.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
sdl/glshape.o: ./include/gh_templates.h ./include/gh_size.h
sdl/glshape.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/glshape.o: ./include/gh_point.h ./include/gh_model.h
sdl/key.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/key.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/key.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/key.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/key.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/key.o: ./include/gh_global.h ./include/gh_stream.h
sdl/key.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
sdl/key.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
sdl/key.o: ./include/gh_sdl_event.h
sdl/log.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/log.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/log.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/log.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/log.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/log.o: ./include/gh_global.h ./include/gh_stream.h
sdl/log.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
sdl/log.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
sdl/log.o: ./include/gh_sdl.h
sdl/loop.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/loop.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/loop.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/loop.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/loop.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/loop.o: ./include/gh_global.h ./include/gh_stream.h
sdl/loop.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
sdl/loop.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
sdl/loop.o: ./include/gh_term.h ./include/gh_subsys.h ./include/gh_ui_event.h
sdl/loop.o: ./include/gh_sdl.h ./include/gh_console_screen.h
sdl/loop.o: ./include/gh_collage.h ./include/gh_surface.h
sdl/loop.o: ./include/gh_getput_pixel.h ./include/gh_screen.h
sdl/loop.o: ./include/gh_math.h ./include/gh_audio.h ./include/gh_music.h
sdl/music.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/music.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/music.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/music.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/music.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/music.o: ./include/gh_global.h ./include/gh_stream.h
sdl/music.o: ./include/gh_templates.h ./include/gh_size.h
sdl/music.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/music.o: ./include/gh_point.h ./include/gh_music.h
sdl/pixel.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/pixel.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/pixel.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/pixel.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/pixel.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/pixel.o: ./include/gh_global.h ./include/gh_stream.h
sdl/pixel.o: ./include/gh_templates.h ./include/gh_size.h
sdl/pixel.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/pixel.o: ./include/gh_point.h ./include/gh_surface.h
sdl/pixel.o: ./include/gh_getput_pixel.h ./include/gh_getput_pixel_macros.h
sdl/putpixel.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/putpixel.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/putpixel.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/putpixel.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/putpixel.o: ./include/gh_backtrace.h ./include/gh_debug.h
sdl/putpixel.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
sdl/putpixel.o: ./include/gh_templates.h ./include/gh_size.h
sdl/putpixel.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/putpixel.o: ./include/gh_point.h ./include/gh_surface.h
sdl/putpixel.o: ./include/gh_getput_pixel.h
sdl/putpixel.o: ./include/gh_getput_pixel_macros.h
sdl/screen.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/screen.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/screen.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/screen.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/screen.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/screen.o: ./include/gh_global.h ./include/gh_stream.h
sdl/screen.o: ./include/gh_templates.h ./include/gh_size.h
sdl/screen.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/screen.o: ./include/gh_point.h ./include/gh_screen.h
sdl/screen.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
sdl/screendump.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/screendump.o: ./include/debugit_instrumented_list.h
sdl/screendump.o: ./include/gh_version.h ./include/gh_main.h
sdl/screendump.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
sdl/screendump.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
sdl/screendump.o: ./include/gh_debug.h ./include/gh_io.h
sdl/screendump.o: ./include/gh_global.h ./include/gh_stream.h
sdl/screendump.o: ./include/gh_templates.h ./include/gh_size.h
sdl/screendump.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/screendump.o: ./include/gh_point.h ./include/gh_surface.h
sdl/screendump.o: ./include/gh_getput_pixel.h ./include/gh_game.h
sdl/screendump.o: ./include/gh_messages.h
sdl/surface.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/surface.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/surface.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/surface.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/surface.o: ./include/gh_backtrace.h ./include/gh_debug.h
sdl/surface.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
sdl/surface.o: ./include/gh_templates.h ./include/gh_size.h
sdl/surface.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/surface.o: ./include/gh_point.h ./include/gh_surface.h
sdl/surface.o: ./include/gh_getput_pixel.h
sdl/thread.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/thread.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/thread.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/thread.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/thread.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/thread.o: ./include/gh_global.h ./include/gh_stream.h
sdl/thread.o: ./include/gh_templates.h ./include/gh_size.h
sdl/thread.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/thread.o: ./include/gh_point.h
sdl/time.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/time.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/time.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/time.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/time.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/time.o: ./include/gh_global.h ./include/gh_stream.h
sdl/time.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
sdl/time.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
sdl/time.o: ./include/gh_sdl.h
sdl/window.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
sdl/window.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
sdl/window.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
sdl/window.o: ./include/gh_hash.h ./include/gh_mathdef.h
sdl/window.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
sdl/window.o: ./include/gh_global.h ./include/gh_stream.h
sdl/window.o: ./include/gh_templates.h ./include/gh_size.h
sdl/window.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
sdl/window.o: ./include/gh_point.h ./include/gh_surface.h
sdl/window.o: ./include/gh_getput_pixel.h
term/console.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
term/console.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
term/console.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
term/console.o: ./include/gh_hash.h ./include/gh_mathdef.h
term/console.o: ./include/gh_backtrace.h ./include/gh_debug.h
term/console.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
term/console.o: ./include/gh_templates.h ./include/gh_size.h
term/console.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
term/console.o: ./include/gh_point.h ./include/gh_surface.h
term/console.o: ./include/gh_getput_pixel.h ./include/gh_console.h
term/console.o: ./include/gh_collage.h ./include/gh_term.h
term/console.o: ./include/gh_subsys.h ./include/gh_ui_event.h
term/console.o: ./include/gh_sdl.h
term/console_screen.o: ./include/ghack.h ./../build/config.h
term/console_screen.o: ./include/debugit.h
term/console_screen.o: ./include/debugit_instrumented_list.h
term/console_screen.o: ./include/gh_version.h ./include/gh_main.h
term/console_screen.o: ./include/gh_sys.h ./include/gh_macro.h
term/console_screen.o: ./include/gh_hash.h ./include/gh_mathdef.h
term/console_screen.o: ./include/gh_backtrace.h ./include/gh_debug.h
term/console_screen.o: ./include/gh_io.h ./include/gh_global.h
term/console_screen.o: ./include/gh_stream.h ./include/gh_templates.h
term/console_screen.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
term/console_screen.o: ./include/gh_string.h ./include/gh_point.h
term/console_screen.o: ./include/gh_term.h ./include/gh_console.h
term/console_screen.o: ./include/gh_collage.h ./include/gh_surface.h
term/console_screen.o: ./include/gh_getput_pixel.h
term/console_screen.o: ./include/gh_console_screen.h ./include/gh_screen.h
term/console_screen.o: ./include/gh_sdl.h
term/figlet.o: ./include/gh_sys.h ./include/gh_figlet.h ./include/gh_term.h
term/figlet.o: ./include/gh_macro.h
term/term.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
term/term.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
term/term.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
term/term.o: ./include/gh_hash.h ./include/gh_mathdef.h
term/term.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
term/term.o: ./include/gh_global.h ./include/gh_stream.h
term/term.o: ./include/gh_templates.h ./include/gh_size.h
term/term.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
term/term.o: ./include/gh_point.h ./include/gh_console.h
term/term.o: ./include/gh_collage.h ./include/gh_surface.h
term/term.o: ./include/gh_getput_pixel.h ./include/gh_term.h
term/term.o: ./include/gh_figlet.h ./include/gh_subsys.h
term/term.o: ./include/gh_ui_event.h ./include/gh_sdl.h
test/fractal.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
test/fractal.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
test/fractal.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
test/fractal.o: ./include/gh_hash.h ./include/gh_mathdef.h
test/fractal.o: ./include/gh_backtrace.h ./include/gh_debug.h
test/fractal.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
test/fractal.o: ./include/gh_templates.h ./include/gh_size.h
test/fractal.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
test/fractal.o: ./include/gh_point.h ./include/gh_sdl.h
things/ability.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/ability.o: ./include/debugit_instrumented_list.h
things/ability.o: ./include/gh_version.h ./include/gh_main.h
things/ability.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/ability.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/ability.o: ./include/gh_debug.h ./include/gh_io.h
things/ability.o: ./include/gh_global.h ./include/gh_stream.h
things/ability.o: ./include/gh_templates.h ./include/gh_size.h
things/ability.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/ability.o: ./include/gh_point.h ./include/gh_player.h
things/ability.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/ability.o: ./include/gh_sdl.h ./include/gh_screen.h
things/ability.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/ability.o: ./include/gh_thing.h ./include/gh_model.h
things/ability.o: ./include/gh_thing_ptr.h ./include/gh_game.h
things/ability.o: ./include/gh_messages.h ./include/gh_level.h
things/ability.o: ./include/gh_map.h
things/acid_proof.o: ./include/ghack.h ./../build/config.h
things/acid_proof.o: ./include/debugit.h
things/acid_proof.o: ./include/debugit_instrumented_list.h
things/acid_proof.o: ./include/gh_version.h ./include/gh_main.h
things/acid_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/acid_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/acid_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/acid_proof.o: ./include/gh_io.h ./include/gh_global.h
things/acid_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/acid_proof.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/acid_proof.o: ./include/gh_string.h ./include/gh_point.h
things/acid_proof.o: ./include/gh_player.h ./include/gh_subsys.h
things/acid_proof.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/acid_proof.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/acid_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/acid_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/acid_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/acid_proof.o: ./include/gh_level.h ./include/gh_map.h
things/acid_proof.o: ./include/gh_math.h
things/actions.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/actions.o: ./include/debugit_instrumented_list.h
things/actions.o: ./include/gh_version.h ./include/gh_main.h
things/actions.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/actions.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/actions.o: ./include/gh_debug.h ./include/gh_io.h
things/actions.o: ./include/gh_global.h ./include/gh_stream.h
things/actions.o: ./include/gh_templates.h ./include/gh_size.h
things/actions.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/actions.o: ./include/gh_point.h ./include/gh_level.h
things/actions.o: ./include/gh_map.h ./include/gh_thing.h
things/actions.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/actions.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/actions.o: ./include/gh_sdl.h ./include/gh_game.h
things/actions.o: ./include/gh_messages.h ./include/gh_player.h
things/actions.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/actions.o: ./include/gh_surface.h ./include/gh_math.h
things/anger.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/anger.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/anger.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/anger.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/anger.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/anger.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/anger.o: ./include/gh_templates.h ./include/gh_size.h
things/anger.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/anger.o: ./include/gh_point.h ./include/gh_thing.h
things/anger.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/anger.o: ./include/gh_game.h ./include/gh_messages.h
things/anger.o: ./include/gh_level.h ./include/gh_map.h
things/anim.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/anim.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/anim.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/anim.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/anim.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/anim.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/anim.o: ./include/gh_templates.h ./include/gh_size.h
things/anim.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/anim.o: ./include/gh_point.h ./include/gh_term.h ./include/gh_thing.h
things/anim.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/anim.o: ./include/gh_console.h ./include/gh_collage.h
things/anim.o: ./include/gh_surface.h ./include/gh_getput_pixel.h
things/anim.o: ./include/gh_math.h ./include/gh_level.h ./include/gh_map.h
things/armour.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/armour.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/armour.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/armour.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/armour.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/armour.o: ./include/gh_io.h ./include/gh_global.h
things/armour.o: ./include/gh_stream.h ./include/gh_templates.h
things/armour.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/armour.o: ./include/gh_string.h ./include/gh_point.h
things/armour.o: ./include/gh_player.h ./include/gh_subsys.h
things/armour.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/armour.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/armour.o: ./include/gh_surface.h ./include/gh_thing.h
things/armour.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/armour.o: ./include/gh_game.h ./include/gh_messages.h
things/armour.o: ./include/gh_level.h ./include/gh_map.h
things/attack.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/attack.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/attack.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/attack.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/attack.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/attack.o: ./include/gh_io.h ./include/gh_global.h
things/attack.o: ./include/gh_stream.h ./include/gh_templates.h
things/attack.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/attack.o: ./include/gh_string.h ./include/gh_point.h
things/attack.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
things/attack.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/attack.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/attack.o: ./include/gh_sdl.h ./include/gh_math.h ./include/gh_game.h
things/attack.o: ./include/gh_messages.h
things/audio.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/audio.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/audio.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/audio.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/audio.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/audio.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/audio.o: ./include/gh_templates.h ./include/gh_size.h
things/audio.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/audio.o: ./include/gh_point.h ./include/gh_game.h
things/audio.o: ./include/gh_messages.h ./include/gh_player.h
things/audio.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/audio.o: ./include/gh_sdl.h ./include/gh_screen.h
things/audio.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/audio.o: ./include/gh_thing.h ./include/gh_model.h
things/audio.o: ./include/gh_thing_ptr.h ./include/gh_level.h
things/audio.o: ./include/gh_map.h ./include/gh_audio.h ./include/gh_math.h
things/bite.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/bite.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/bite.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/bite.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/bite.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/bite.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/bite.o: ./include/gh_templates.h ./include/gh_size.h
things/bite.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/bite.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/bite.o: ./include/gh_thing.h ./include/gh_model.h
things/bite.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/bite.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/bite.o: ./include/gh_messages.h ./include/gh_math.h
things/bite.o: ./include/gh_thing_registration.h ./include/gh_player.h
things/bite.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/bite.o: ./include/gh_surface.h
things/bravery.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/bravery.o: ./include/debugit_instrumented_list.h
things/bravery.o: ./include/gh_version.h ./include/gh_main.h
things/bravery.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/bravery.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/bravery.o: ./include/gh_debug.h ./include/gh_io.h
things/bravery.o: ./include/gh_global.h ./include/gh_stream.h
things/bravery.o: ./include/gh_templates.h ./include/gh_size.h
things/bravery.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/bravery.o: ./include/gh_point.h ./include/gh_thing.h
things/bravery.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/bravery.o: ./include/gh_level.h ./include/gh_map.h
things/can_see_distance.o: ./include/ghack.h ./../build/config.h
things/can_see_distance.o: ./include/debugit.h
things/can_see_distance.o: ./include/debugit_instrumented_list.h
things/can_see_distance.o: ./include/gh_version.h ./include/gh_main.h
things/can_see_distance.o: ./include/gh_sys.h ./include/gh_macro.h
things/can_see_distance.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/can_see_distance.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/can_see_distance.o: ./include/gh_io.h ./include/gh_global.h
things/can_see_distance.o: ./include/gh_stream.h ./include/gh_templates.h
things/can_see_distance.o: ./include/gh_size.h ./include/gh_pixel.h
things/can_see_distance.o: ./io/zip.h ./include/gh_string.h
things/can_see_distance.o: ./include/gh_point.h ./include/gh_level.h
things/can_see_distance.o: ./include/gh_map.h ./include/gh_thing.h
things/can_see_distance.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/can_see_distance.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/can_see_distance.o: ./include/gh_sdl.h ./include/gh_game.h
things/can_see_distance.o: ./include/gh_messages.h
things/chasing.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/chasing.o: ./include/debugit_instrumented_list.h
things/chasing.o: ./include/gh_version.h ./include/gh_main.h
things/chasing.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/chasing.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/chasing.o: ./include/gh_debug.h ./include/gh_io.h
things/chasing.o: ./include/gh_global.h ./include/gh_stream.h
things/chasing.o: ./include/gh_templates.h ./include/gh_size.h
things/chasing.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/chasing.o: ./include/gh_point.h ./include/gh_level.h
things/chasing.o: ./include/gh_map.h ./include/gh_thing.h
things/chasing.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/chasing.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/chasing.o: ./include/gh_sdl.h ./include/gh_math.h
things/cold_proof.o: ./include/ghack.h ./../build/config.h
things/cold_proof.o: ./include/debugit.h
things/cold_proof.o: ./include/debugit_instrumented_list.h
things/cold_proof.o: ./include/gh_version.h ./include/gh_main.h
things/cold_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/cold_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/cold_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/cold_proof.o: ./include/gh_io.h ./include/gh_global.h
things/cold_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/cold_proof.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/cold_proof.o: ./include/gh_string.h ./include/gh_point.h
things/cold_proof.o: ./include/gh_player.h ./include/gh_subsys.h
things/cold_proof.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/cold_proof.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/cold_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/cold_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/cold_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/cold_proof.o: ./include/gh_level.h ./include/gh_map.h
things/cold_proof.o: ./include/gh_math.h
things/collect.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/collect.o: ./include/debugit_instrumented_list.h
things/collect.o: ./include/gh_version.h ./include/gh_main.h
things/collect.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/collect.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/collect.o: ./include/gh_debug.h ./include/gh_io.h
things/collect.o: ./include/gh_global.h ./include/gh_stream.h
things/collect.o: ./include/gh_templates.h ./include/gh_size.h
things/collect.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/collect.o: ./include/gh_point.h ./include/gh_level.h
things/collect.o: ./include/gh_map.h ./include/gh_thing.h
things/collect.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/collect.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/collect.o: ./include/gh_sdl.h ./include/gh_game.h
things/collect.o: ./include/gh_messages.h ./include/gh_player.h
things/collect.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/collect.o: ./include/gh_surface.h ./include/gh_room.h
things/collision.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/collision.o: ./include/debugit_instrumented_list.h
things/collision.o: ./include/gh_version.h ./include/gh_main.h
things/collision.o: ./include/gh_sys.h ./include/gh_macro.h
things/collision.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/collision.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/collision.o: ./include/gh_io.h ./include/gh_global.h
things/collision.o: ./include/gh_stream.h ./include/gh_templates.h
things/collision.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/collision.o: ./include/gh_string.h ./include/gh_point.h
things/collision.o: ./include/gh_thing.h ./include/gh_model.h
things/collision.o: ./include/gh_thing_ptr.h ./include/gh_term.h
things/dead.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/dead.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/dead.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/dead.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/dead.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/dead.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/dead.o: ./include/gh_templates.h ./include/gh_size.h
things/dead.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/dead.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/dead.o: ./include/gh_thing.h ./include/gh_model.h
things/dead.o: ./include/gh_thing_ptr.h ./include/gh_game.h
things/dead.o: ./include/gh_messages.h
things/debug.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/debug.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/debug.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/debug.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/debug.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/debug.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/debug.o: ./include/gh_templates.h ./include/gh_size.h
things/debug.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/debug.o: ./include/gh_point.h ./include/gh_thing.h
things/debug.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/default.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/default.o: ./include/debugit_instrumented_list.h
things/default.o: ./include/gh_version.h ./include/gh_main.h
things/default.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/default.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/default.o: ./include/gh_debug.h ./include/gh_io.h
things/default.o: ./include/gh_global.h ./include/gh_stream.h
things/default.o: ./include/gh_templates.h ./include/gh_size.h
things/default.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/default.o: ./include/gh_point.h ./include/gh_thing.h
things/default.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/default.o: ./include/gh_thing_registration.h
things/dex.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/dex.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/dex.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/dex.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/dex.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
things/dex.o: ./include/gh_global.h ./include/gh_stream.h
things/dex.o: ./include/gh_templates.h ./include/gh_size.h
things/dex.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/dex.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/dex.o: ./include/gh_thing.h ./include/gh_model.h
things/dex.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/dex.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/drop.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/drop.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/drop.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/drop.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/drop.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/drop.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/drop.o: ./include/gh_templates.h ./include/gh_size.h
things/drop.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/drop.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/drop.o: ./include/gh_thing.h ./include/gh_model.h
things/drop.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/drop.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/drop.o: ./include/gh_messages.h ./include/gh_player.h
things/drop.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/drop.o: ./include/gh_surface.h ./include/gh_room.h
things/exit.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/exit.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/exit.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/exit.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/exit.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/exit.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/exit.o: ./include/gh_templates.h ./include/gh_size.h
things/exit.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/exit.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/exit.o: ./include/gh_thing.h ./include/gh_model.h
things/exit.o: ./include/gh_thing_ptr.h ./include/gh_player.h
things/exit.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/exit.o: ./include/gh_sdl.h ./include/gh_screen.h
things/exit.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/exit.o: ./include/gh_room.h ./include/gh_game.h
things/exit.o: ./include/gh_messages.h ./include/gh_math.h
things/find.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/find.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/find.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/find.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/find.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/find.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/find.o: ./include/gh_templates.h ./include/gh_size.h
things/find.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/find.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/find.o: ./include/gh_thing.h ./include/gh_model.h
things/find.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/find.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/find.o: ./include/gh_messages.h
things/fire_proof.o: ./include/ghack.h ./../build/config.h
things/fire_proof.o: ./include/debugit.h
things/fire_proof.o: ./include/debugit_instrumented_list.h
things/fire_proof.o: ./include/gh_version.h ./include/gh_main.h
things/fire_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/fire_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/fire_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/fire_proof.o: ./include/gh_io.h ./include/gh_global.h
things/fire_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/fire_proof.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/fire_proof.o: ./include/gh_string.h ./include/gh_point.h
things/fire_proof.o: ./include/gh_player.h ./include/gh_subsys.h
things/fire_proof.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/fire_proof.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/fire_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/fire_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/fire_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/fire_proof.o: ./include/gh_level.h ./include/gh_map.h
things/fire_proof.o: ./include/gh_math.h
things/followers.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/followers.o: ./include/debugit_instrumented_list.h
things/followers.o: ./include/gh_version.h ./include/gh_main.h
things/followers.o: ./include/gh_sys.h ./include/gh_macro.h
things/followers.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/followers.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/followers.o: ./include/gh_io.h ./include/gh_global.h
things/followers.o: ./include/gh_stream.h ./include/gh_templates.h
things/followers.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/followers.o: ./include/gh_string.h ./include/gh_point.h
things/followers.o: ./include/gh_game.h ./include/gh_messages.h
things/followers.o: ./include/gh_level.h ./include/gh_map.h
things/followers.o: ./include/gh_thing.h ./include/gh_model.h
things/followers.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/followers.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/followers.o: ./include/gh_math.h
things/gfx.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/gfx.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/gfx.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/gfx.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/gfx.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
things/gfx.o: ./include/gh_global.h ./include/gh_stream.h
things/gfx.o: ./include/gh_templates.h ./include/gh_size.h
things/gfx.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/gfx.o: ./include/gh_point.h ./include/gh_term.h ./include/gh_thing.h
things/gfx.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/gfx.o: ./include/gh_console.h ./include/gh_collage.h
things/gfx.o: ./include/gh_surface.h ./include/gh_getput_pixel.h
things/gfx.o: ./include/gh_math.h ./include/gh_figlet.h ./include/gh_audio.h
things/gfx.o: ./include/gh_music.h
things/hit.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/hit.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/hit.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/hit.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/hit.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
things/hit.o: ./include/gh_global.h ./include/gh_stream.h
things/hit.o: ./include/gh_templates.h ./include/gh_size.h
things/hit.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/hit.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/hit.o: ./include/gh_thing.h ./include/gh_model.h
things/hit.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/hit.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/hit.o: ./include/gh_messages.h ./include/gh_player.h
things/hit.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/hit.o: ./include/gh_surface.h ./include/gh_math.h
things/insult.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/insult.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/insult.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/insult.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/insult.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/insult.o: ./include/gh_io.h ./include/gh_global.h
things/insult.o: ./include/gh_stream.h ./include/gh_templates.h
things/insult.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/insult.o: ./include/gh_string.h ./include/gh_point.h
things/insult.o: ./include/gh_thing.h ./include/gh_model.h
things/insult.o: ./include/gh_thing_ptr.h ./include/gh_level.h
things/insult.o: ./include/gh_map.h ./include/gh_game.h
things/insult.o: ./include/gh_messages.h
things/invisible.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/invisible.o: ./include/debugit_instrumented_list.h
things/invisible.o: ./include/gh_version.h ./include/gh_main.h
things/invisible.o: ./include/gh_sys.h ./include/gh_macro.h
things/invisible.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/invisible.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/invisible.o: ./include/gh_io.h ./include/gh_global.h
things/invisible.o: ./include/gh_stream.h ./include/gh_templates.h
things/invisible.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/invisible.o: ./include/gh_string.h ./include/gh_point.h
things/invisible.o: ./include/gh_level.h ./include/gh_map.h
things/invisible.o: ./include/gh_thing.h ./include/gh_model.h
things/invisible.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/invisible.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/item.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/item.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/item.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/item.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/item.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/item.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/item.o: ./include/gh_templates.h ./include/gh_size.h
things/item.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/item.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/item.o: ./include/gh_thing.h ./include/gh_model.h
things/item.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/item.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/item.o: ./include/gh_messages.h ./include/gh_player.h
things/item.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/item.o: ./include/gh_surface.h
things/lifespan.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/lifespan.o: ./include/debugit_instrumented_list.h
things/lifespan.o: ./include/gh_version.h ./include/gh_main.h
things/lifespan.o: ./include/gh_sys.h ./include/gh_macro.h
things/lifespan.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/lifespan.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/lifespan.o: ./include/gh_io.h ./include/gh_global.h
things/lifespan.o: ./include/gh_stream.h ./include/gh_templates.h
things/lifespan.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/lifespan.o: ./include/gh_string.h ./include/gh_point.h
things/lifespan.o: ./include/gh_level.h ./include/gh_map.h
things/lifespan.o: ./include/gh_thing.h ./include/gh_model.h
things/lifespan.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/lifespan.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/light_strength.o: ./include/ghack.h ./../build/config.h
things/light_strength.o: ./include/debugit.h
things/light_strength.o: ./include/debugit_instrumented_list.h
things/light_strength.o: ./include/gh_version.h ./include/gh_main.h
things/light_strength.o: ./include/gh_sys.h ./include/gh_macro.h
things/light_strength.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/light_strength.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/light_strength.o: ./include/gh_io.h ./include/gh_global.h
things/light_strength.o: ./include/gh_stream.h ./include/gh_templates.h
things/light_strength.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/light_strength.o: ./include/gh_string.h ./include/gh_point.h
things/light_strength.o: ./include/gh_level.h ./include/gh_map.h
things/light_strength.o: ./include/gh_thing.h ./include/gh_model.h
things/light_strength.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/light_strength.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/light_strength.o: ./include/gh_game.h ./include/gh_messages.h
things/log.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/log.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/log.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/log.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/log.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
things/log.o: ./include/gh_global.h ./include/gh_stream.h
things/log.o: ./include/gh_templates.h ./include/gh_size.h
things/log.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/log.o: ./include/gh_point.h ./include/gh_sdl.h ./include/gh_thing.h
things/log.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/meet.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/meet.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/meet.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/meet.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/meet.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/meet.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/meet.o: ./include/gh_templates.h ./include/gh_size.h
things/meet.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/meet.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/meet.o: ./include/gh_thing.h ./include/gh_model.h
things/meet.o: ./include/gh_thing_ptr.h ./include/gh_player.h
things/meet.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/meet.o: ./include/gh_sdl.h ./include/gh_screen.h
things/meet.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/meet.o: ./include/gh_room.h ./include/gh_game.h
things/meet.o: ./include/gh_messages.h ./include/gh_math.h
things/metabolism.o: ./include/ghack.h ./../build/config.h
things/metabolism.o: ./include/debugit.h
things/metabolism.o: ./include/debugit_instrumented_list.h
things/metabolism.o: ./include/gh_version.h ./include/gh_main.h
things/metabolism.o: ./include/gh_sys.h ./include/gh_macro.h
things/metabolism.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/metabolism.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/metabolism.o: ./include/gh_io.h ./include/gh_global.h
things/metabolism.o: ./include/gh_stream.h ./include/gh_templates.h
things/metabolism.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/metabolism.o: ./include/gh_string.h ./include/gh_point.h
things/metabolism.o: ./include/gh_level.h ./include/gh_map.h
things/metabolism.o: ./include/gh_thing.h ./include/gh_model.h
things/metabolism.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/metabolism.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/metabolism.o: ./include/gh_game.h ./include/gh_messages.h
things/monst.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/monst.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/monst.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/monst.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/monst.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/monst.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/monst.o: ./include/gh_templates.h ./include/gh_size.h
things/monst.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/monst.o: ./include/gh_point.h ./include/gh_game.h
things/monst.o: ./include/gh_messages.h ./include/gh_level.h
things/monst.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
things/monst.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/monst.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/monst.o: ./include/gh_math.h
things/move.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/move.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/move.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/move.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/move.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/move.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/move.o: ./include/gh_templates.h ./include/gh_size.h
things/move.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/move.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/move.o: ./include/gh_thing.h ./include/gh_model.h
things/move.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/move.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/move.o: ./include/gh_messages.h ./include/gh_room.h
things/move.o: ./include/gh_math.h
things/open.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/open.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/open.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/open.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/open.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/open.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/open.o: ./include/gh_templates.h ./include/gh_size.h
things/open.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/open.o: ./include/gh_point.h ./include/gh_thing.h ./include/gh_model.h
things/open.o: ./include/gh_thing_ptr.h ./include/gh_thing_registration.h
things/open.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_game.h
things/open.o: ./include/gh_messages.h
things/parse.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/parse.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/parse.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/parse.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/parse.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/parse.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/parse.o: ./include/gh_templates.h ./include/gh_size.h
things/parse.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/parse.o: ./include/gh_point.h ./include/gh_term.h ./include/gh_thing.h
things/parse.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/parse.o: ./include/gh_console.h ./include/gh_collage.h
things/parse.o: ./include/gh_surface.h ./include/gh_getput_pixel.h
things/parse.o: ./include/gh_math.h
things/poison_proof.o: ./include/ghack.h ./../build/config.h
things/poison_proof.o: ./include/debugit.h
things/poison_proof.o: ./include/debugit_instrumented_list.h
things/poison_proof.o: ./include/gh_version.h ./include/gh_main.h
things/poison_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/poison_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/poison_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/poison_proof.o: ./include/gh_io.h ./include/gh_global.h
things/poison_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/poison_proof.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/poison_proof.o: ./include/gh_string.h ./include/gh_point.h
things/poison_proof.o: ./include/gh_player.h ./include/gh_subsys.h
things/poison_proof.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/poison_proof.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/poison_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/poison_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/poison_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/poison_proof.o: ./include/gh_level.h ./include/gh_map.h
things/poison_proof.o: ./include/gh_math.h
things/poisoned.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/poisoned.o: ./include/debugit_instrumented_list.h
things/poisoned.o: ./include/gh_version.h ./include/gh_main.h
things/poisoned.o: ./include/gh_sys.h ./include/gh_macro.h
things/poisoned.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/poisoned.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/poisoned.o: ./include/gh_io.h ./include/gh_global.h
things/poisoned.o: ./include/gh_stream.h ./include/gh_templates.h
things/poisoned.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/poisoned.o: ./include/gh_string.h ./include/gh_point.h
things/poisoned.o: ./include/gh_level.h ./include/gh_map.h
things/poisoned.o: ./include/gh_thing.h ./include/gh_model.h
things/poisoned.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/poisoned.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/polymorph.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/polymorph.o: ./include/debugit_instrumented_list.h
things/polymorph.o: ./include/gh_version.h ./include/gh_main.h
things/polymorph.o: ./include/gh_sys.h ./include/gh_macro.h
things/polymorph.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/polymorph.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/polymorph.o: ./include/gh_io.h ./include/gh_global.h
things/polymorph.o: ./include/gh_stream.h ./include/gh_templates.h
things/polymorph.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/polymorph.o: ./include/gh_string.h ./include/gh_point.h
things/polymorph.o: ./include/gh_level.h ./include/gh_map.h
things/polymorph.o: ./include/gh_thing.h ./include/gh_model.h
things/polymorph.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/polymorph.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/polymorph.o: ./include/gh_math.h ./include/gh_game.h
things/polymorph.o: ./include/gh_messages.h
things/possess.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/possess.o: ./include/debugit_instrumented_list.h
things/possess.o: ./include/gh_version.h ./include/gh_main.h
things/possess.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/possess.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/possess.o: ./include/gh_debug.h ./include/gh_io.h
things/possess.o: ./include/gh_global.h ./include/gh_stream.h
things/possess.o: ./include/gh_templates.h ./include/gh_size.h
things/possess.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/possess.o: ./include/gh_point.h ./include/gh_thing.h
things/possess.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/possess.o: ./include/gh_game.h ./include/gh_messages.h
things/possess.o: ./include/gh_level.h ./include/gh_map.h
things/ptr.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/ptr.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/ptr.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/ptr.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/ptr.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
things/ptr.o: ./include/gh_global.h ./include/gh_stream.h
things/ptr.o: ./include/gh_templates.h ./include/gh_size.h
things/ptr.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/ptr.o: ./include/gh_point.h ./include/gh_sdl.h ./include/gh_thing.h
things/ptr.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/rage.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/rage.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/rage.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/rage.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/rage.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/rage.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/rage.o: ./include/gh_templates.h ./include/gh_size.h
things/rage.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/rage.o: ./include/gh_point.h ./include/gh_game.h
things/rage.o: ./include/gh_messages.h ./include/gh_level.h
things/rage.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
things/rage.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/rage.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_math.h
things/ref.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/ref.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/ref.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/ref.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/ref.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
things/ref.o: ./include/gh_global.h ./include/gh_stream.h
things/ref.o: ./include/gh_templates.h ./include/gh_size.h
things/ref.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/ref.o: ./include/gh_point.h ./include/gh_thing.h ./include/gh_model.h
things/ref.o: ./include/gh_thing_ptr.h ./include/gh_level.h
things/ref.o: ./include/gh_map.h
things/registration.o: ./include/ghack.h ./../build/config.h
things/registration.o: ./include/debugit.h
things/registration.o: ./include/debugit_instrumented_list.h
things/registration.o: ./include/gh_version.h ./include/gh_main.h
things/registration.o: ./include/gh_sys.h ./include/gh_macro.h
things/registration.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/registration.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/registration.o: ./include/gh_io.h ./include/gh_global.h
things/registration.o: ./include/gh_stream.h ./include/gh_templates.h
things/registration.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/registration.o: ./include/gh_string.h ./include/gh_point.h
things/registration.o: ./include/gh_thing_registration.h
things/room.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/room.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/room.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/room.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/room.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/room.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/room.o: ./include/gh_templates.h ./include/gh_size.h
things/room.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/room.o: ./include/gh_point.h ./include/gh_game.h
things/room.o: ./include/gh_messages.h ./include/gh_level.h
things/room.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
things/room.o: ./include/gh_thing_ptr.h ./include/gh_room.h
things/scared.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/scared.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/scared.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/scared.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/scared.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/scared.o: ./include/gh_io.h ./include/gh_global.h
things/scared.o: ./include/gh_stream.h ./include/gh_templates.h
things/scared.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/scared.o: ./include/gh_string.h ./include/gh_point.h
things/scared.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
things/scared.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/scared.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/scared.o: ./include/gh_sdl.h
things/shielded.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/shielded.o: ./include/debugit_instrumented_list.h
things/shielded.o: ./include/gh_version.h ./include/gh_main.h
things/shielded.o: ./include/gh_sys.h ./include/gh_macro.h
things/shielded.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/shielded.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/shielded.o: ./include/gh_io.h ./include/gh_global.h
things/shielded.o: ./include/gh_stream.h ./include/gh_templates.h
things/shielded.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/shielded.o: ./include/gh_string.h ./include/gh_point.h
things/shielded.o: ./include/gh_level.h ./include/gh_map.h
things/shielded.o: ./include/gh_thing.h ./include/gh_model.h
things/shielded.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/shielded.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/shop.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/shop.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/shop.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/shop.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/shop.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/shop.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/shop.o: ./include/gh_templates.h ./include/gh_size.h
things/shop.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/shop.o: ./include/gh_point.h ./include/gh_game.h
things/shop.o: ./include/gh_messages.h ./include/gh_level.h
things/shop.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
things/shop.o: ./include/gh_thing_ptr.h ./include/gh_room.h
things/shop.o: ./include/gh_math.h ./include/gh_player.h
things/shop.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/shop.o: ./include/gh_sdl.h ./include/gh_screen.h
things/shop.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/sleeping.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/sleeping.o: ./include/debugit_instrumented_list.h
things/sleeping.o: ./include/gh_version.h ./include/gh_main.h
things/sleeping.o: ./include/gh_sys.h ./include/gh_macro.h
things/sleeping.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/sleeping.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/sleeping.o: ./include/gh_io.h ./include/gh_global.h
things/sleeping.o: ./include/gh_stream.h ./include/gh_templates.h
things/sleeping.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/sleeping.o: ./include/gh_string.h ./include/gh_point.h
things/sleeping.o: ./include/gh_level.h ./include/gh_map.h
things/sleeping.o: ./include/gh_thing.h ./include/gh_model.h
things/sleeping.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/sleeping.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/spawn.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/spawn.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/spawn.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/spawn.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/spawn.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/spawn.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/spawn.o: ./include/gh_templates.h ./include/gh_size.h
things/spawn.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/spawn.o: ./include/gh_point.h ./include/gh_game.h
things/spawn.o: ./include/gh_messages.h ./include/gh_level.h
things/spawn.o: ./include/gh_map.h ./include/gh_thing.h ./include/gh_model.h
things/spawn.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/spawn.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/spawn.o: ./include/gh_math.h
things/spell.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/spell.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/spell.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/spell.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/spell.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/spell.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/spell.o: ./include/gh_templates.h ./include/gh_size.h
things/spell.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/spell.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/spell.o: ./include/gh_thing.h ./include/gh_model.h
things/spell.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/spell.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/spell.o: ./include/gh_game.h ./include/gh_messages.h
things/spell.o: ./include/gh_player.h ./include/gh_screen.h
things/spell.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/spell.o: ./include/gh_math.h
things/stamina.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/stamina.o: ./include/debugit_instrumented_list.h
things/stamina.o: ./include/gh_version.h ./include/gh_main.h
things/stamina.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
things/stamina.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
things/stamina.o: ./include/gh_debug.h ./include/gh_io.h
things/stamina.o: ./include/gh_global.h ./include/gh_stream.h
things/stamina.o: ./include/gh_templates.h ./include/gh_size.h
things/stamina.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/stamina.o: ./include/gh_point.h ./include/gh_level.h
things/stamina.o: ./include/gh_map.h ./include/gh_thing.h
things/stamina.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/stamina.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/stamina.o: ./include/gh_sdl.h ./include/gh_game.h
things/stamina.o: ./include/gh_messages.h
things/stone.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/stone.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/stone.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/stone.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/stone.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/stone.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/stone.o: ./include/gh_templates.h ./include/gh_size.h
things/stone.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/stone.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/stone.o: ./include/gh_thing.h ./include/gh_model.h
things/stone.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/stone.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/stone_proof.o: ./include/ghack.h ./../build/config.h
things/stone_proof.o: ./include/debugit.h
things/stone_proof.o: ./include/debugit_instrumented_list.h
things/stone_proof.o: ./include/gh_version.h ./include/gh_main.h
things/stone_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/stone_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/stone_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/stone_proof.o: ./include/gh_io.h ./include/gh_global.h
things/stone_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/stone_proof.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/stone_proof.o: ./include/gh_string.h ./include/gh_point.h
things/stone_proof.o: ./include/gh_player.h ./include/gh_subsys.h
things/stone_proof.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/stone_proof.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/stone_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/stone_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/stone_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/stone_proof.o: ./include/gh_level.h ./include/gh_map.h
things/stone_proof.o: ./include/gh_math.h
things/strength.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/strength.o: ./include/debugit_instrumented_list.h
things/strength.o: ./include/gh_version.h ./include/gh_main.h
things/strength.o: ./include/gh_sys.h ./include/gh_macro.h
things/strength.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/strength.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/strength.o: ./include/gh_io.h ./include/gh_global.h
things/strength.o: ./include/gh_stream.h ./include/gh_templates.h
things/strength.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/strength.o: ./include/gh_string.h ./include/gh_point.h
things/strength.o: ./include/gh_level.h ./include/gh_map.h
things/strength.o: ./include/gh_thing.h ./include/gh_model.h
things/strength.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/strength.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/suffocation_proof.o: ./include/ghack.h ./../build/config.h
things/suffocation_proof.o: ./include/debugit.h
things/suffocation_proof.o: ./include/debugit_instrumented_list.h
things/suffocation_proof.o: ./include/gh_version.h ./include/gh_main.h
things/suffocation_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/suffocation_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/suffocation_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/suffocation_proof.o: ./include/gh_io.h ./include/gh_global.h
things/suffocation_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/suffocation_proof.o: ./include/gh_size.h ./include/gh_pixel.h
things/suffocation_proof.o: ./io/zip.h ./include/gh_string.h
things/suffocation_proof.o: ./include/gh_point.h ./include/gh_player.h
things/suffocation_proof.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/suffocation_proof.o: ./include/gh_sdl.h ./include/gh_screen.h
things/suffocation_proof.o: ./include/gh_getput_pixel.h
things/suffocation_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/suffocation_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/suffocation_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/suffocation_proof.o: ./include/gh_level.h ./include/gh_map.h
things/suffocation_proof.o: ./include/gh_math.h
things/summon.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/summon.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/summon.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/summon.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/summon.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/summon.o: ./include/gh_io.h ./include/gh_global.h
things/summon.o: ./include/gh_stream.h ./include/gh_templates.h
things/summon.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/summon.o: ./include/gh_string.h ./include/gh_point.h
things/summon.o: ./include/gh_game.h ./include/gh_messages.h
things/summon.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
things/summon.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/summon.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/summon.o: ./include/gh_sdl.h ./include/gh_math.h
things/teleport.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/teleport.o: ./include/debugit_instrumented_list.h
things/teleport.o: ./include/gh_version.h ./include/gh_main.h
things/teleport.o: ./include/gh_sys.h ./include/gh_macro.h
things/teleport.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/teleport.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/teleport.o: ./include/gh_io.h ./include/gh_global.h
things/teleport.o: ./include/gh_stream.h ./include/gh_templates.h
things/teleport.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/teleport.o: ./include/gh_string.h ./include/gh_point.h
things/teleport.o: ./include/gh_game.h ./include/gh_messages.h
things/teleport.o: ./include/gh_level.h ./include/gh_map.h
things/teleport.o: ./include/gh_thing.h ./include/gh_model.h
things/teleport.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/teleport.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/teleport.o: ./include/gh_math.h
things/teleport_proof.o: ./include/ghack.h ./../build/config.h
things/teleport_proof.o: ./include/debugit.h
things/teleport_proof.o: ./include/debugit_instrumented_list.h
things/teleport_proof.o: ./include/gh_version.h ./include/gh_main.h
things/teleport_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/teleport_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/teleport_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/teleport_proof.o: ./include/gh_io.h ./include/gh_global.h
things/teleport_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/teleport_proof.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/teleport_proof.o: ./include/gh_string.h ./include/gh_point.h
things/teleport_proof.o: ./include/gh_player.h ./include/gh_subsys.h
things/teleport_proof.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/teleport_proof.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/teleport_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/teleport_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/teleport_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/teleport_proof.o: ./include/gh_level.h ./include/gh_map.h
things/teleport_proof.o: ./include/gh_math.h
things/thing.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/thing.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/thing.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/thing.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/thing.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/thing.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/thing.o: ./include/gh_templates.h ./include/gh_size.h
things/thing.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/thing.o: ./include/gh_point.h ./include/hacklib.h ./include/gh_thing.h
things/thing.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/thing.o: ./include/gh_unique_things.h ./include/gh_surface.h
things/thing.o: ./include/gh_getput_pixel.h ./include/gh_thing_registration.h
things/thing.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_room.h
things/thing.o: ./include/gh_collage.h ./include/gh_math.h ./include/gh_sdl.h
things/thing.o: ./include/gh_player.h ./include/gh_subsys.h
things/thing.o: ./include/gh_ui_event.h ./include/gh_screen.h
things/throwing.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/throwing.o: ./include/debugit_instrumented_list.h
things/throwing.o: ./include/gh_version.h ./include/gh_main.h
things/throwing.o: ./include/gh_sys.h ./include/gh_macro.h
things/throwing.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/throwing.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/throwing.o: ./include/gh_io.h ./include/gh_global.h
things/throwing.o: ./include/gh_stream.h ./include/gh_templates.h
things/throwing.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/throwing.o: ./include/gh_string.h ./include/gh_point.h
things/throwing.o: ./include/gh_level.h ./include/gh_map.h
things/throwing.o: ./include/gh_thing.h ./include/gh_model.h
things/throwing.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/throwing.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/throwing.o: ./include/gh_math.h
things/tick.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/tick.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/tick.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/tick.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/tick.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/tick.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/tick.o: ./include/gh_templates.h ./include/gh_size.h
things/tick.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/tick.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/tick.o: ./include/gh_thing.h ./include/gh_model.h
things/tick.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/tick.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/tick.o: ./include/gh_messages.h ./include/gh_thing_registration.h
things/tick.o: ./include/gh_player.h ./include/gh_screen.h
things/tick.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/tick.o: ./include/gh_math.h
things/transmog.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/transmog.o: ./include/debugit_instrumented_list.h
things/transmog.o: ./include/gh_version.h ./include/gh_main.h
things/transmog.o: ./include/gh_sys.h ./include/gh_macro.h
things/transmog.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/transmog.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/transmog.o: ./include/gh_io.h ./include/gh_global.h
things/transmog.o: ./include/gh_stream.h ./include/gh_templates.h
things/transmog.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/transmog.o: ./include/gh_string.h ./include/gh_point.h
things/transmog.o: ./include/gh_game.h ./include/gh_messages.h
things/transmog.o: ./include/gh_level.h ./include/gh_map.h
things/transmog.o: ./include/gh_thing.h ./include/gh_model.h
things/transmog.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/transmog.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/transmog.o: ./include/gh_math.h
things/unique_things.o: ./include/ghack.h ./../build/config.h
things/unique_things.o: ./include/debugit.h
things/unique_things.o: ./include/debugit_instrumented_list.h
things/unique_things.o: ./include/gh_version.h ./include/gh_main.h
things/unique_things.o: ./include/gh_sys.h ./include/gh_macro.h
things/unique_things.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/unique_things.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/unique_things.o: ./include/gh_io.h ./include/gh_global.h
things/unique_things.o: ./include/gh_stream.h ./include/gh_templates.h
things/unique_things.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/unique_things.o: ./include/gh_string.h ./include/gh_point.h
things/unique_things.o: ./include/gh_thing.h ./include/gh_model.h
things/unique_things.o: ./include/gh_thing_ptr.h ./include/gh_math.h
things/unique_things.o: ./include/gh_unique_things.h
things/use.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/use.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/use.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/use.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/use.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
things/use.o: ./include/gh_global.h ./include/gh_stream.h
things/use.o: ./include/gh_templates.h ./include/gh_size.h
things/use.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/use.o: ./include/gh_point.h ./include/gh_level.h ./include/gh_map.h
things/use.o: ./include/gh_thing.h ./include/gh_model.h
things/use.o: ./include/gh_thing_ptr.h ./include/gh_subsys.h
things/use.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_game.h
things/use.o: ./include/gh_messages.h ./include/gh_player.h
things/use.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/use.o: ./include/gh_surface.h ./include/gh_math.h
things/util.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/util.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/util.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/util.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/util.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/util.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/util.o: ./include/gh_templates.h ./include/gh_size.h
things/util.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/util.o: ./include/gh_point.h ./include/gh_thing.h ./include/gh_model.h
things/util.o: ./include/gh_thing_ptr.h ./include/gh_level.h
things/util.o: ./include/gh_map.h ./include/gh_game.h ./include/gh_messages.h
things/util.o: ./include/gh_player.h ./include/gh_subsys.h
things/util.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/util.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/util.o: ./include/gh_surface.h ./include/gh_math.h
things/util.o: ./include/gh_thing_registration.h
things/vision.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/vision.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/vision.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/vision.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/vision.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/vision.o: ./include/gh_io.h ./include/gh_global.h
things/vision.o: ./include/gh_stream.h ./include/gh_templates.h
things/vision.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/vision.o: ./include/gh_string.h ./include/gh_point.h
things/vision.o: ./include/gh_thing.h ./include/gh_model.h
things/vision.o: ./include/gh_thing_ptr.h ./include/gh_math.h
things/vision.o: ./include/gh_level.h ./include/gh_map.h
things/water_proof.o: ./include/ghack.h ./../build/config.h
things/water_proof.o: ./include/debugit.h
things/water_proof.o: ./include/debugit_instrumented_list.h
things/water_proof.o: ./include/gh_version.h ./include/gh_main.h
things/water_proof.o: ./include/gh_sys.h ./include/gh_macro.h
things/water_proof.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/water_proof.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/water_proof.o: ./include/gh_io.h ./include/gh_global.h
things/water_proof.o: ./include/gh_stream.h ./include/gh_templates.h
things/water_proof.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/water_proof.o: ./include/gh_string.h ./include/gh_point.h
things/water_proof.o: ./include/gh_player.h ./include/gh_subsys.h
things/water_proof.o: ./include/gh_ui_event.h ./include/gh_sdl.h
things/water_proof.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/water_proof.o: ./include/gh_surface.h ./include/gh_thing.h
things/water_proof.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/water_proof.o: ./include/gh_game.h ./include/gh_messages.h
things/water_proof.o: ./include/gh_level.h ./include/gh_map.h
things/water_proof.o: ./include/gh_math.h
things/weapon.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/weapon.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/weapon.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/weapon.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/weapon.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/weapon.o: ./include/gh_io.h ./include/gh_global.h
things/weapon.o: ./include/gh_stream.h ./include/gh_templates.h
things/weapon.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
things/weapon.o: ./include/gh_string.h ./include/gh_point.h
things/weapon.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
things/weapon.o: ./include/gh_model.h ./include/gh_thing_ptr.h
things/weapon.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/weapon.o: ./include/gh_sdl.h ./include/gh_game.h
things/weapon.o: ./include/gh_messages.h ./include/gh_player.h
things/weapon.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
things/weapon.o: ./include/gh_surface.h
things/wear.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
things/wear.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
things/wear.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
things/wear.o: ./include/gh_hash.h ./include/gh_mathdef.h
things/wear.o: ./include/gh_backtrace.h ./include/gh_debug.h
things/wear.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
things/wear.o: ./include/gh_templates.h ./include/gh_size.h
things/wear.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
things/wear.o: ./include/gh_point.h ./include/gh_player.h
things/wear.o: ./include/gh_subsys.h ./include/gh_ui_event.h
things/wear.o: ./include/gh_sdl.h ./include/gh_screen.h
things/wear.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
things/wear.o: ./include/gh_thing.h ./include/gh_model.h
things/wear.o: ./include/gh_thing_ptr.h
ui/config.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/config.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
ui/config.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
ui/config.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/config.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
ui/config.o: ./include/gh_global.h ./include/gh_stream.h
ui/config.o: ./include/gh_templates.h ./include/gh_size.h
ui/config.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/config.o: ./include/gh_point.h ./include/gh_term.h ./include/gh_ui_event.h
ui/config.o: ./include/gh_figlet.h ./include/gh_level.h ./include/gh_map.h
ui/config.o: ./include/gh_thing.h ./include/gh_model.h
ui/config.o: ./include/gh_thing_ptr.h ./include/gh_player.h
ui/config.o: ./include/gh_subsys.h ./include/gh_sdl.h ./include/gh_screen.h
ui/config.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/config.o: ./include/gh_game.h ./include/gh_messages.h ./include/gh_music.h
ui/config.o: ./include/gh_audio.h
ui/help_txt.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/help_txt.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
ui/help_txt.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
ui/help_txt.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/help_txt.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/help_txt.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
ui/help_txt.o: ./include/gh_templates.h ./include/gh_size.h
ui/help_txt.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/help_txt.o: ./include/gh_point.h ./include/gh_subsys.h
ui/help_txt.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_term.h
ui/help_txt.o: ./include/gh_figlet.h ./include/gh_level.h ./include/gh_map.h
ui/help_txt.o: ./include/gh_thing.h ./include/gh_model.h
ui/help_txt.o: ./include/gh_thing_ptr.h ./include/gh_player.h
ui/help_txt.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
ui/help_txt.o: ./include/gh_surface.h ./include/gh_game.h
ui/help_txt.o: ./include/gh_messages.h ./include/gh_sdl_event.h
ui/help_txt.o: ./include/gh_audio.h
ui/hiscore.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/hiscore.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
ui/hiscore.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
ui/hiscore.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/hiscore.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
ui/hiscore.o: ./include/gh_global.h ./include/gh_stream.h
ui/hiscore.o: ./include/gh_templates.h ./include/gh_size.h
ui/hiscore.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/hiscore.o: ./include/gh_point.h ./include/gh_subsys.h
ui/hiscore.o: ./include/gh_ui_event.h ./include/gh_sdl.h ./include/gh_term.h
ui/hiscore.o: ./include/gh_figlet.h ./include/gh_level.h ./include/gh_map.h
ui/hiscore.o: ./include/gh_thing.h ./include/gh_model.h
ui/hiscore.o: ./include/gh_thing_ptr.h ./include/gh_player.h
ui/hiscore.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
ui/hiscore.o: ./include/gh_surface.h ./include/gh_game.h
ui/hiscore.o: ./include/gh_messages.h ./include/gh_hiscore.h
ui/hiscore.o: ./include/gh_sdl_event.h ./include/gh_audio.h
ui/intro.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/intro.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
ui/intro.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
ui/intro.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/intro.o: ./include/gh_backtrace.h ./include/gh_debug.h ./include/gh_io.h
ui/intro.o: ./include/gh_global.h ./include/gh_stream.h
ui/intro.o: ./include/gh_templates.h ./include/gh_size.h ./include/gh_pixel.h
ui/intro.o: ./io/zip.h ./include/gh_string.h ./include/gh_point.h
ui/intro.o: ./include/gh_term.h ./include/gh_ui_event.h
ui/intro.o: ./include/gh_ui_hiscore.h ./include/gh_subsys.h
ui/intro.o: ./include/gh_sdl.h ./include/gh_game.h ./include/gh_messages.h
ui/intro.o: ./include/gh_figlet.h ./include/gh_screen.h
ui/intro.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/intro.o: ./include/gh_audio.h ./include/gh_music.h ./include/hacklib.h
ui/level_chooser.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/level_chooser.o: ./include/debugit_instrumented_list.h
ui/level_chooser.o: ./include/gh_version.h ./include/gh_main.h
ui/level_chooser.o: ./include/gh_sys.h ./include/gh_macro.h
ui/level_chooser.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/level_chooser.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/level_chooser.o: ./include/gh_io.h ./include/gh_global.h
ui/level_chooser.o: ./include/gh_stream.h ./include/gh_templates.h
ui/level_chooser.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
ui/level_chooser.o: ./include/gh_string.h ./include/gh_point.h
ui/level_chooser.o: ./include/gh_term.h ./include/gh_ui_event.h
ui/level_chooser.o: ./include/gh_figlet.h ./include/gh_level.h
ui/level_chooser.o: ./include/gh_map.h ./include/gh_thing.h
ui/level_chooser.o: ./include/gh_model.h ./include/gh_thing_ptr.h
ui/level_chooser.o: ./include/gh_player.h ./include/gh_subsys.h
ui/level_chooser.o: ./include/gh_sdl.h ./include/gh_screen.h
ui/level_chooser.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/level_chooser.o: ./include/gh_game.h ./include/gh_messages.h
ui/level_chooser.o: ./include/gh_audio.h
ui/level_end.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/level_end.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
ui/level_end.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
ui/level_end.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/level_end.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/level_end.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
ui/level_end.o: ./include/gh_templates.h ./include/gh_size.h
ui/level_end.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/level_end.o: ./include/gh_point.h ./include/gh_term.h
ui/level_end.o: ./include/gh_ui_event.h ./include/gh_figlet.h
ui/level_end.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
ui/level_end.o: ./include/gh_model.h ./include/gh_thing_ptr.h
ui/level_end.o: ./include/gh_player.h ./include/gh_subsys.h
ui/level_end.o: ./include/gh_sdl.h ./include/gh_screen.h
ui/level_end.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/level_end.o: ./include/gh_game.h ./include/gh_messages.h
ui/level_start.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/level_start.o: ./include/debugit_instrumented_list.h
ui/level_start.o: ./include/gh_version.h ./include/gh_main.h
ui/level_start.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
ui/level_start.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
ui/level_start.o: ./include/gh_debug.h ./include/gh_io.h
ui/level_start.o: ./include/gh_global.h ./include/gh_stream.h
ui/level_start.o: ./include/gh_templates.h ./include/gh_size.h
ui/level_start.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/level_start.o: ./include/gh_point.h ./include/gh_subsys.h
ui/level_start.o: ./include/gh_ui_event.h ./include/gh_sdl.h
ui/level_start.o: ./include/gh_term.h ./include/gh_level.h ./include/gh_map.h
ui/level_start.o: ./include/gh_thing.h ./include/gh_model.h
ui/level_start.o: ./include/gh_thing_ptr.h ./include/gh_player.h
ui/level_start.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
ui/level_start.o: ./include/gh_surface.h ./include/gh_game.h
ui/level_start.o: ./include/gh_messages.h ./include/gh_figlet.h
ui/level_start.o: ./include/gh_audio.h
ui/level_type_chooser.o: ./include/ghack.h ./../build/config.h
ui/level_type_chooser.o: ./include/debugit.h
ui/level_type_chooser.o: ./include/debugit_instrumented_list.h
ui/level_type_chooser.o: ./include/gh_version.h ./include/gh_main.h
ui/level_type_chooser.o: ./include/gh_sys.h ./include/gh_macro.h
ui/level_type_chooser.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/level_type_chooser.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/level_type_chooser.o: ./include/gh_io.h ./include/gh_global.h
ui/level_type_chooser.o: ./include/gh_stream.h ./include/gh_templates.h
ui/level_type_chooser.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
ui/level_type_chooser.o: ./include/gh_string.h ./include/gh_point.h
ui/level_type_chooser.o: ./include/gh_term.h ./include/gh_ui_event.h
ui/level_type_chooser.o: ./include/gh_figlet.h ./include/gh_player.h
ui/level_type_chooser.o: ./include/gh_subsys.h ./include/gh_sdl.h
ui/level_type_chooser.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
ui/level_type_chooser.o: ./include/gh_surface.h ./include/gh_game.h
ui/level_type_chooser.o: ./include/gh_messages.h ./include/gh_audio.h
ui/level_type_chooser.o: ./include/gh_thing.h ./include/gh_model.h
ui/level_type_chooser.o: ./include/gh_thing_ptr.h
ui/messages.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/messages.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
ui/messages.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
ui/messages.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/messages.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/messages.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
ui/messages.o: ./include/gh_templates.h ./include/gh_size.h
ui/messages.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/messages.o: ./include/gh_point.h ./include/gh_game.h
ui/messages.o: ./include/gh_messages.h ./include/gh_term.h
ui/messages.o: ./include/gh_subsys.h ./include/gh_ui_event.h
ui/messages.o: ./include/gh_sdl.h ./include/gh_player.h ./include/gh_screen.h
ui/messages.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/player_auto_save.o: ./include/ghack.h ./../build/config.h
ui/player_auto_save.o: ./include/debugit.h
ui/player_auto_save.o: ./include/debugit_instrumented_list.h
ui/player_auto_save.o: ./include/gh_version.h ./include/gh_main.h
ui/player_auto_save.o: ./include/gh_sys.h ./include/gh_macro.h
ui/player_auto_save.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/player_auto_save.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/player_auto_save.o: ./include/gh_io.h ./include/gh_global.h
ui/player_auto_save.o: ./include/gh_stream.h ./include/gh_templates.h
ui/player_auto_save.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
ui/player_auto_save.o: ./include/gh_string.h ./include/gh_point.h
ui/player_auto_save.o: ./include/gh_term.h ./include/gh_ui_event.h
ui/player_auto_save.o: ./include/gh_figlet.h ./include/gh_player.h
ui/player_auto_save.o: ./include/gh_subsys.h ./include/gh_sdl.h
ui/player_auto_save.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
ui/player_auto_save.o: ./include/gh_surface.h ./include/gh_game.h
ui/player_auto_save.o: ./include/gh_messages.h ./include/gh_audio.h
ui/player_chooser.o: ./include/ghack.h ./../build/config.h
ui/player_chooser.o: ./include/debugit.h
ui/player_chooser.o: ./include/debugit_instrumented_list.h
ui/player_chooser.o: ./include/gh_version.h ./include/gh_main.h
ui/player_chooser.o: ./include/gh_sys.h ./include/gh_macro.h
ui/player_chooser.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/player_chooser.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/player_chooser.o: ./include/gh_io.h ./include/gh_global.h
ui/player_chooser.o: ./include/gh_stream.h ./include/gh_templates.h
ui/player_chooser.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
ui/player_chooser.o: ./include/gh_string.h ./include/gh_point.h
ui/player_chooser.o: ./include/gh_term.h ./include/gh_ui_event.h
ui/player_chooser.o: ./include/gh_figlet.h ./include/gh_player.h
ui/player_chooser.o: ./include/gh_subsys.h ./include/gh_sdl.h
ui/player_chooser.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
ui/player_chooser.o: ./include/gh_surface.h ./include/gh_game.h
ui/player_chooser.o: ./include/gh_messages.h ./include/gh_audio.h
ui/player_class_chooser.o: ./include/ghack.h ./../build/config.h
ui/player_class_chooser.o: ./include/debugit.h
ui/player_class_chooser.o: ./include/debugit_instrumented_list.h
ui/player_class_chooser.o: ./include/gh_version.h ./include/gh_main.h
ui/player_class_chooser.o: ./include/gh_sys.h ./include/gh_macro.h
ui/player_class_chooser.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/player_class_chooser.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/player_class_chooser.o: ./include/gh_io.h ./include/gh_global.h
ui/player_class_chooser.o: ./include/gh_stream.h ./include/gh_templates.h
ui/player_class_chooser.o: ./include/gh_size.h ./include/gh_pixel.h
ui/player_class_chooser.o: ./io/zip.h ./include/gh_string.h
ui/player_class_chooser.o: ./include/gh_point.h ./include/gh_term.h
ui/player_class_chooser.o: ./include/gh_ui_event.h ./include/gh_figlet.h
ui/player_class_chooser.o: ./include/gh_player.h ./include/gh_subsys.h
ui/player_class_chooser.o: ./include/gh_sdl.h ./include/gh_screen.h
ui/player_class_chooser.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/player_class_chooser.o: ./include/gh_game.h ./include/gh_messages.h
ui/player_class_chooser.o: ./include/gh_audio.h ./include/gh_thing.h
ui/player_class_chooser.o: ./include/gh_model.h ./include/gh_thing_ptr.h
ui/player_croaked_it.o: ./include/ghack.h ./../build/config.h
ui/player_croaked_it.o: ./include/debugit.h
ui/player_croaked_it.o: ./include/debugit_instrumented_list.h
ui/player_croaked_it.o: ./include/gh_version.h ./include/gh_main.h
ui/player_croaked_it.o: ./include/gh_sys.h ./include/gh_macro.h
ui/player_croaked_it.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/player_croaked_it.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/player_croaked_it.o: ./include/gh_io.h ./include/gh_global.h
ui/player_croaked_it.o: ./include/gh_stream.h ./include/gh_templates.h
ui/player_croaked_it.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
ui/player_croaked_it.o: ./include/gh_string.h ./include/gh_point.h
ui/player_croaked_it.o: ./include/gh_term.h ./include/gh_ui_event.h
ui/player_croaked_it.o: ./include/gh_figlet.h ./include/gh_thing.h
ui/player_croaked_it.o: ./include/gh_model.h ./include/gh_thing_ptr.h
ui/player_croaked_it.o: ./include/gh_player.h ./include/gh_subsys.h
ui/player_croaked_it.o: ./include/gh_sdl.h ./include/gh_screen.h
ui/player_croaked_it.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/player_croaked_it.o: ./include/gh_game.h ./include/gh_messages.h
ui/player_dead.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/player_dead.o: ./include/debugit_instrumented_list.h
ui/player_dead.o: ./include/gh_version.h ./include/gh_main.h
ui/player_dead.o: ./include/gh_sys.h ./include/gh_macro.h ./include/gh_hash.h
ui/player_dead.o: ./include/gh_mathdef.h ./include/gh_backtrace.h
ui/player_dead.o: ./include/gh_debug.h ./include/gh_io.h
ui/player_dead.o: ./include/gh_global.h ./include/gh_stream.h
ui/player_dead.o: ./include/gh_templates.h ./include/gh_size.h
ui/player_dead.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/player_dead.o: ./include/gh_point.h ./include/gh_subsys.h
ui/player_dead.o: ./include/gh_ui_event.h ./include/gh_sdl.h
ui/player_dead.o: ./include/gh_term.h ./include/gh_level.h ./include/gh_map.h
ui/player_dead.o: ./include/gh_thing.h ./include/gh_model.h
ui/player_dead.o: ./include/gh_thing_ptr.h ./include/gh_player.h
ui/player_dead.o: ./include/gh_screen.h ./include/gh_getput_pixel.h
ui/player_dead.o: ./include/gh_surface.h ./include/gh_game.h
ui/player_dead.o: ./include/gh_messages.h ./include/gh_figlet.h
ui/player_powerup_chooser.o: ./include/ghack.h ./../build/config.h
ui/player_powerup_chooser.o: ./include/debugit.h
ui/player_powerup_chooser.o: ./include/debugit_instrumented_list.h
ui/player_powerup_chooser.o: ./include/gh_version.h ./include/gh_main.h
ui/player_powerup_chooser.o: ./include/gh_sys.h ./include/gh_macro.h
ui/player_powerup_chooser.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/player_powerup_chooser.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/player_powerup_chooser.o: ./include/gh_io.h ./include/gh_global.h
ui/player_powerup_chooser.o: ./include/gh_stream.h ./include/gh_templates.h
ui/player_powerup_chooser.o: ./include/gh_size.h ./include/gh_pixel.h
ui/player_powerup_chooser.o: ./io/zip.h ./include/gh_string.h
ui/player_powerup_chooser.o: ./include/gh_point.h ./include/gh_term.h
ui/player_powerup_chooser.o: ./include/gh_ui_event.h ./include/gh_figlet.h
ui/player_powerup_chooser.o: ./include/gh_player.h ./include/gh_subsys.h
ui/player_powerup_chooser.o: ./include/gh_sdl.h ./include/gh_screen.h
ui/player_powerup_chooser.o: ./include/gh_getput_pixel.h
ui/player_powerup_chooser.o: ./include/gh_surface.h ./include/gh_game.h
ui/player_powerup_chooser.o: ./include/gh_messages.h ./include/gh_audio.h
ui/player_powerup_chooser.o: ./include/gh_thing.h ./include/gh_model.h
ui/player_powerup_chooser.o: ./include/gh_thing_ptr.h
ui/read_small_text.o: ./include/ghack.h ./../build/config.h
ui/read_small_text.o: ./include/debugit.h
ui/read_small_text.o: ./include/debugit_instrumented_list.h
ui/read_small_text.o: ./include/gh_version.h ./include/gh_main.h
ui/read_small_text.o: ./include/gh_sys.h ./include/gh_macro.h
ui/read_small_text.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/read_small_text.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/read_small_text.o: ./include/gh_io.h ./include/gh_global.h
ui/read_small_text.o: ./include/gh_stream.h ./include/gh_templates.h
ui/read_small_text.o: ./include/gh_size.h ./include/gh_pixel.h ./io/zip.h
ui/read_small_text.o: ./include/gh_string.h ./include/gh_point.h
ui/read_small_text.o: ./include/gh_subsys.h ./include/gh_ui_event.h
ui/read_small_text.o: ./include/gh_sdl.h ./include/gh_term.h
ui/read_small_text.o: ./include/gh_figlet.h ./include/gh_level.h
ui/read_small_text.o: ./include/gh_map.h ./include/gh_thing.h
ui/read_small_text.o: ./include/gh_model.h ./include/gh_thing_ptr.h
ui/read_small_text.o: ./include/gh_player.h ./include/gh_screen.h
ui/read_small_text.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/read_small_text.o: ./include/gh_game.h ./include/gh_messages.h
ui/read_small_text.o: ./include/gh_sdl_event.h ./include/gh_audio.h
ui/read_text.o: ./include/ghack.h ./../build/config.h ./include/debugit.h
ui/read_text.o: ./include/debugit_instrumented_list.h ./include/gh_version.h
ui/read_text.o: ./include/gh_main.h ./include/gh_sys.h ./include/gh_macro.h
ui/read_text.o: ./include/gh_hash.h ./include/gh_mathdef.h
ui/read_text.o: ./include/gh_backtrace.h ./include/gh_debug.h
ui/read_text.o: ./include/gh_io.h ./include/gh_global.h ./include/gh_stream.h
ui/read_text.o: ./include/gh_templates.h ./include/gh_size.h
ui/read_text.o: ./include/gh_pixel.h ./io/zip.h ./include/gh_string.h
ui/read_text.o: ./include/gh_point.h ./include/gh_subsys.h
ui/read_text.o: ./include/gh_ui_event.h ./include/gh_sdl.h
ui/read_text.o: ./include/gh_term.h ./include/gh_figlet.h
ui/read_text.o: ./include/gh_level.h ./include/gh_map.h ./include/gh_thing.h
ui/read_text.o: ./include/gh_model.h ./include/gh_thing_ptr.h
ui/read_text.o: ./include/gh_player.h ./include/gh_screen.h
ui/read_text.o: ./include/gh_getput_pixel.h ./include/gh_surface.h
ui/read_text.o: ./include/gh_game.h ./include/gh_messages.h
ui/read_text.o: ./include/gh_sdl_event.h ./include/gh_audio.h
resources/things/room/exit.o: ./include/ghack.h ./../build/config.h
resources/things/room/exit.o: ./include/debugit.h
resources/things/room/exit.o: ./include/debugit_instrumented_list.h
resources/things/room/exit.o: ./include/gh_version.h ./include/gh_main.h
resources/things/room/exit.o: ./include/gh_sys.h ./include/gh_macro.h
resources/things/room/exit.o: ./include/gh_hash.h ./include/gh_mathdef.h
resources/things/room/exit.o: ./include/gh_backtrace.h ./include/gh_debug.h
resources/things/room/exit.o: ./include/gh_io.h ./include/gh_global.h
resources/things/room/exit.o: ./include/gh_stream.h ./include/gh_templates.h
resources/things/room/exit.o: ./include/gh_size.h ./include/gh_pixel.h
resources/things/room/exit.o: ./io/zip.h ./include/gh_string.h
resources/things/room/exit.o: ./include/gh_point.h ./include/gh_thing.h
resources/things/room/exit.o: ./include/gh_model.h ./include/gh_thing_ptr.h
resources/things/room/exit.o: ./include/gh_thing_registration.h
resources/things/room/doors/broken_door.o: ./include/ghack.h
resources/things/room/doors/broken_door.o: ./../build/config.h
resources/things/room/doors/broken_door.o: ./include/debugit.h
resources/things/room/doors/broken_door.o: ./include/debugit_instrumented_list.h
resources/things/room/doors/broken_door.o: ./include/gh_version.h
resources/things/room/doors/broken_door.o: ./include/gh_main.h
resources/things/room/doors/broken_door.o: ./include/gh_sys.h
resources/things/room/doors/broken_door.o: ./include/gh_macro.h
resources/things/room/doors/broken_door.o: ./include/gh_hash.h
resources/things/room/doors/broken_door.o: ./include/gh_mathdef.h
resources/things/room/doors/broken_door.o: ./include/gh_backtrace.h
resources/things/room/doors/broken_door.o: ./include/gh_debug.h
resources/things/room/doors/broken_door.o: ./include/gh_io.h
resources/things/room/doors/broken_door.o: ./include/gh_global.h
resources/things/room/doors/broken_door.o: ./include/gh_stream.h
resources/things/room/doors/broken_door.o: ./include/gh_templates.h
resources/things/room/doors/broken_door.o: ./include/gh_size.h
resources/things/room/doors/broken_door.o: ./include/gh_pixel.h ./io/zip.h
resources/things/room/doors/broken_door.o: ./include/gh_string.h
resources/things/room/doors/broken_door.o: ./include/gh_point.h
resources/things/room/doors/broken_door.o: ./include/gh_thing_registration.h
resources/things/room/doors/broken_door.o: ./include/gh_level.h
resources/things/room/doors/broken_door.o: ./include/gh_map.h
resources/things/room/doors/broken_door.o: ./include/gh_thing.h
resources/things/room/doors/broken_door.o: ./include/gh_model.h
resources/things/room/doors/broken_door.o: ./include/gh_thing_ptr.h
resources/things/room/doors/broken_door.o: ./include/gh_game.h
resources/things/room/doors/broken_door.o: ./include/gh_messages.h
resources/things/room/doors/cell_door.o: ./include/ghack.h
resources/things/room/doors/cell_door.o: ./../build/config.h
resources/things/room/doors/cell_door.o: ./include/debugit.h
resources/things/room/doors/cell_door.o: ./include/debugit_instrumented_list.h
resources/things/room/doors/cell_door.o: ./include/gh_version.h
resources/things/room/doors/cell_door.o: ./include/gh_main.h
resources/things/room/doors/cell_door.o: ./include/gh_sys.h
resources/things/room/doors/cell_door.o: ./include/gh_macro.h
resources/things/room/doors/cell_door.o: ./include/gh_hash.h
resources/things/room/doors/cell_door.o: ./include/gh_mathdef.h
resources/things/room/doors/cell_door.o: ./include/gh_backtrace.h
resources/things/room/doors/cell_door.o: ./include/gh_debug.h
resources/things/room/doors/cell_door.o: ./include/gh_io.h
resources/things/room/doors/cell_door.o: ./include/gh_global.h
resources/things/room/doors/cell_door.o: ./include/gh_stream.h
resources/things/room/doors/cell_door.o: ./include/gh_templates.h
resources/things/room/doors/cell_door.o: ./include/gh_size.h
resources/things/room/doors/cell_door.o: ./include/gh_pixel.h ./io/zip.h
resources/things/room/doors/cell_door.o: ./include/gh_string.h
resources/things/room/doors/cell_door.o: ./include/gh_point.h
resources/things/room/doors/cell_door.o: ./include/gh_thing_registration.h
resources/things/room/doors/cell_door.o: ./include/gh_level.h
resources/things/room/doors/cell_door.o: ./include/gh_map.h
resources/things/room/doors/cell_door.o: ./include/gh_thing.h
resources/things/room/doors/cell_door.o: ./include/gh_model.h
resources/things/room/doors/cell_door.o: ./include/gh_thing_ptr.h
resources/things/room/doors/cell_door.o: ./include/gh_game.h
resources/things/room/doors/cell_door.o: ./include/gh_messages.h
resources/things/room/doors/cell_door.o: ./include/gh_audio.h
resources/things/room/doors/door.o: ./include/ghack.h ./../build/config.h
resources/things/room/doors/door.o: ./include/debugit.h
resources/things/room/doors/door.o: ./include/debugit_instrumented_list.h
resources/things/room/doors/door.o: ./include/gh_version.h
resources/things/room/doors/door.o: ./include/gh_main.h ./include/gh_sys.h
resources/things/room/doors/door.o: ./include/gh_macro.h ./include/gh_hash.h
resources/things/room/doors/door.o: ./include/gh_mathdef.h
resources/things/room/doors/door.o: ./include/gh_backtrace.h
resources/things/room/doors/door.o: ./include/gh_debug.h ./include/gh_io.h
resources/things/room/doors/door.o: ./include/gh_global.h
resources/things/room/doors/door.o: ./include/gh_stream.h
resources/things/room/doors/door.o: ./include/gh_templates.h
resources/things/room/doors/door.o: ./include/gh_size.h ./include/gh_pixel.h
resources/things/room/doors/door.o: ./io/zip.h ./include/gh_string.h
resources/things/room/doors/door.o: ./include/gh_point.h
resources/things/room/doors/door.o: ./include/gh_thing_registration.h
resources/things/room/doors/door.o: ./include/gh_level.h ./include/gh_map.h
resources/things/room/doors/door.o: ./include/gh_thing.h ./include/gh_model.h
resources/things/room/doors/door.o: ./include/gh_thing_ptr.h
resources/things/room/doors/door.o: ./include/gh_game.h
resources/things/room/doors/door.o: ./include/gh_messages.h
resources/things/room/doors/door.o: ./include/gh_audio.h
resources/things/room/doors/electro_door.o: ./include/ghack.h
resources/things/room/doors/electro_door.o: ./../build/config.h
resources/things/room/doors/electro_door.o: ./include/debugit.h
resources/things/room/doors/electro_door.o: ./include/debugit_instrumented_list.h
resources/things/room/doors/electro_door.o: ./include/gh_version.h
resources/things/room/doors/electro_door.o: ./include/gh_main.h
resources/things/room/doors/electro_door.o: ./include/gh_sys.h
resources/things/room/doors/electro_door.o: ./include/gh_macro.h
resources/things/room/doors/electro_door.o: ./include/gh_hash.h
resources/things/room/doors/electro_door.o: ./include/gh_mathdef.h
resources/things/room/doors/electro_door.o: ./include/gh_backtrace.h
resources/things/room/doors/electro_door.o: ./include/gh_debug.h
resources/things/room/doors/electro_door.o: ./include/gh_io.h
resources/things/room/doors/electro_door.o: ./include/gh_global.h
resources/things/room/doors/electro_door.o: ./include/gh_stream.h
resources/things/room/doors/electro_door.o: ./include/gh_templates.h
resources/things/room/doors/electro_door.o: ./include/gh_size.h
resources/things/room/doors/electro_door.o: ./include/gh_pixel.h ./io/zip.h
resources/things/room/doors/electro_door.o: ./include/gh_string.h
resources/things/room/doors/electro_door.o: ./include/gh_point.h
resources/things/room/doors/electro_door.o: ./include/gh_thing_registration.h
resources/things/room/doors/electro_door.o: ./include/gh_level.h
resources/things/room/doors/electro_door.o: ./include/gh_map.h
resources/things/room/doors/electro_door.o: ./include/gh_thing.h
resources/things/room/doors/electro_door.o: ./include/gh_model.h
resources/things/room/doors/electro_door.o: ./include/gh_thing_ptr.h
resources/things/room/doors/electro_door.o: ./include/gh_game.h
resources/things/room/doors/electro_door.o: ./include/gh_messages.h
resources/things/room/doors/fancy.o: ./include/ghack.h ./../build/config.h
resources/things/room/doors/fancy.o: ./include/debugit.h
resources/things/room/doors/fancy.o: ./include/debugit_instrumented_list.h
resources/things/room/doors/fancy.o: ./include/gh_version.h
resources/things/room/doors/fancy.o: ./include/gh_main.h ./include/gh_sys.h
resources/things/room/doors/fancy.o: ./include/gh_macro.h ./include/gh_hash.h
resources/things/room/doors/fancy.o: ./include/gh_mathdef.h
resources/things/room/doors/fancy.o: ./include/gh_backtrace.h
resources/things/room/doors/fancy.o: ./include/gh_debug.h ./include/gh_io.h
resources/things/room/doors/fancy.o: ./include/gh_global.h
resources/things/room/doors/fancy.o: ./include/gh_stream.h
resources/things/room/doors/fancy.o: ./include/gh_templates.h
resources/things/room/doors/fancy.o: ./include/gh_size.h ./include/gh_pixel.h
resources/things/room/doors/fancy.o: ./io/zip.h ./include/gh_string.h
resources/things/room/doors/fancy.o: ./include/gh_point.h
resources/things/room/doors/fancy.o: ./include/gh_thing_registration.h
resources/things/room/doors/fancy.o: ./include/gh_level.h ./include/gh_map.h
resources/things/room/doors/fancy.o: ./include/gh_thing.h
resources/things/room/doors/fancy.o: ./include/gh_model.h
resources/things/room/doors/fancy.o: ./include/gh_thing_ptr.h
resources/things/room/doors/fancy.o: ./include/gh_game.h
resources/things/room/doors/fancy.o: ./include/gh_messages.h
resources/things/room/doors/fancy.o: ./include/gh_audio.h
resources/things/room/doors/portcullis.o: ./include/ghack.h
resources/things/room/doors/portcullis.o: ./../build/config.h
resources/things/room/doors/portcullis.o: ./include/debugit.h
resources/things/room/doors/portcullis.o: ./include/debugit_instrumented_list.h
resources/things/room/doors/portcullis.o: ./include/gh_version.h
resources/things/room/doors/portcullis.o: ./include/gh_main.h
resources/things/room/doors/portcullis.o: ./include/gh_sys.h
resources/things/room/doors/portcullis.o: ./include/gh_macro.h
resources/things/room/doors/portcullis.o: ./include/gh_hash.h
resources/things/room/doors/portcullis.o: ./include/gh_mathdef.h
resources/things/room/doors/portcullis.o: ./include/gh_backtrace.h
resources/things/room/doors/portcullis.o: ./include/gh_debug.h
resources/things/room/doors/portcullis.o: ./include/gh_io.h
resources/things/room/doors/portcullis.o: ./include/gh_global.h
resources/things/room/doors/portcullis.o: ./include/gh_stream.h
resources/things/room/doors/portcullis.o: ./include/gh_templates.h
resources/things/room/doors/portcullis.o: ./include/gh_size.h
resources/things/room/doors/portcullis.o: ./include/gh_pixel.h ./io/zip.h
resources/things/room/doors/portcullis.o: ./include/gh_string.h
resources/things/room/doors/portcullis.o: ./include/gh_point.h
resources/things/room/doors/portcullis.o: ./include/gh_thing_registration.h
resources/things/room/doors/portcullis.o: ./include/gh_level.h
resources/things/room/doors/portcullis.o: ./include/gh_map.h
resources/things/room/doors/portcullis.o: ./include/gh_thing.h
resources/things/room/doors/portcullis.o: ./include/gh_model.h
resources/things/room/doors/portcullis.o: ./include/gh_thing_ptr.h
resources/things/room/doors/portcullis.o: ./include/gh_game.h
resources/things/room/doors/portcullis.o: ./include/gh_messages.h
resources/things/room/doors/portcullis.o: ./include/gh_audio.h
