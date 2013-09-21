/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: main.cpp 848 2008-04-30 14:38:59Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <signal.h>
#include <unistd.h>
#include <gh_thing.h>
#include <gh_room.h>
#include <gh_zip.h>
#include <gh_surface.h>
#include <hacklib.h>
#include <gh_audio.h>
#include <gh_music.h>
#include <gh_sdl.h>

#if defined __linux__
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#endif

//
// Download the next version of the game. If already downloaded, kick
// off the .exe to do the install.
//
void gh_main_installer (void)
{
#if defined WIN32 || defined __CYGWIN__
    //
    // e.g. save as goblinhack-1.15-windows.exe
    //
    string save_file_as =
                    "goblinhack-" +
                    tostring(GH_MAJOR_VERSION_NUMBER) +
                    "." +
                    tostring(GH_MINOR_VERSION_NUMBER + 1) +
                    "-windows.exe";
    //
    // e.g. download as
    //
    // http://prdownloads.sourceforge.net/sourceforge/goblinhack/goblinhack-1.15-windows.exe
    //
    string download_file_as =
                    "http://prdownloads.sourceforge.net/"
                    "sourceforge/goblinhack/"
                    "goblinhack-" +
                    tostring(GH_MAJOR_VERSION_NUMBER) +
                    "." +
                    tostring(GH_MINOR_VERSION_NUMBER + 1) +
                    "-windows.exe";

    //
    // curl -g -L
    //    --output goblinhack-1.15-windows.exe
    //    --url
    //    http://prdownloads.sourceforge.net/sourceforge/goblinhack/goblinhack-1.15-windows.exe
    //
    const char *curl_args[] = {
        "-g",
        "-L",
        "--output",
        save_file_as.c_str(),
        "--url",
        download_file_as.c_str(),
        0,
    };

    //
    // If the next version doesn't exist, download it now.
    //
    if (!gh_global::file_exists(save_file_as)) {
        //
        // Spawn curl as a child process to do the download in the background.
        //
        // If this fails then we're in trouble as we'll have a partial .exe.
        // Not worked out how to avoid this. Help!
        //
        spawnve(P_NOWAIT, "curl", curl_args, NULL);
    } else {
        //
        // Lets check the file looks to be of a sane size before calling
        // the installer. If it looks dodgy, < 10 Mb then just delete it
        // so we can download the file again.
        //
        if (gh_global::file_size(save_file_as) < 10 * 1000 * 1000) {
            //
            // Is too annoying to keep retrying all the time.
            //
            if ((rand() % 100) < 5) {
                gh_global::file_unlink(save_file_as);
            }

            return;
        }

        //
        // Replace the current process with the .exe to download.
        //
        int ret = MessageBox(NULL,
                             "I've downloaded the next version of Ghack!"
                             "    Install it now?",
                             download_file_as.c_str(), MB_YESNO);

        if (ret == IDYES) {
            const char *no_args[] = {
                0,
            };

            execve(save_file_as.c_str(), no_args, NULL);
            gh_global::file_unlink(save_file_as);
            return;
        }

        //
        // Ok, delete it instead? I'll just download it again anyway! 8)
        //
        ret = MessageBox(NULL,
                         "Should I delete the download file?",
                         download_file_as.c_str(), MB_YESNO);
        if (ret == IDYES) {
            gh_global::file_unlink(save_file_as);
        }
    }
#endif

#if defined __linux__
    pid_t childpid = fork();
    
    if (childpid >= 0) {
        //
        // Fork ok
        //
        if (childpid == 0) {
            //
            // Child
            //
        } else {
            //
            // Parent
            //
            return;
        }
    } else {
        perror("fork");
        exit(0);
    }

    //
    // e.g. save as goblinhack-1.15-linux-installer.bin
    //
    string save_file_as =
                    "goblinhack-" +
                    tostring(GH_MAJOR_VERSION_NUMBER) +
                    "." +
                    tostring(GH_MINOR_VERSION_NUMBER + 1) +
#ifdef __x86_64__
                    "-linux-x64-installer.bin";
#else
                    "-linux-installer.bin";
#endif
    //
    // e.g. download as
    //
    // http://prdownloads.sourceforge.net/sourceforge/goblinhack/goblinhack-1.15-linux-installer.bin
    //
    string download_file_as =
                    "http://prdownloads.sourceforge.net/"
                    "sourceforge/goblinhack/"
                    "goblinhack-" +
                    tostring(GH_MAJOR_VERSION_NUMBER) +
                    "." +
                    tostring(GH_MINOR_VERSION_NUMBER + 1) +
#ifdef __x86_64__
                    "-linux-x64-installer.bin";
#else
                    "-linux-installer.bin";
#endif

    //
    // curl -g -L
    //    --output goblinhack-1.15-linux-installer.bin
    //    --url
    //    http://prdownloads.sourceforge.net/sourceforge/goblinhack/goblinhack-1.15-linux-installer.bin
    //
    char *curl_args[] = {
        (char*)"-g",
        (char*)"-L",
        (char*)"--output",
        (char*)save_file_as.c_str(),
        (char*)"--url",
        (char*)download_file_as.c_str(),
        0,
    };

    //
    // If the next version doesn't exist, download it now.
    //
    if (!gh_global::file_exists(save_file_as)) {
        //
        // Spawn curl as a child process to do the download in the background.
        //
        // If this fails then we're in trouble as we'll have a partial installer.bin.
        // Not worked out how to avoid this. Help!
        //
        execve("/usr/bin/curl", curl_args, NULL);
        perror("execve");   /* execve() only returns on error */
    } else {
        //
        // Lets check the file looks to be of a sane size before calling
        // the installer. If it looks dodgy, < 10 Mb then just delete it
        // so we can download the file again.
        //
        if (gh_global::file_size(save_file_as) < 10 * 1000 * 1000) {
            //
            // Is too annoying to keep retrying all the time.
            //
            if ((rand() % 100) < 5) {
                gh_global::file_unlink(save_file_as);
            }

            exit(0);
        }

        if (system(("/bin/chmod +x " + save_file_as).c_str()) == -1) {
            perror("change permissions failed");
        }

        if (system(("./" + save_file_as).c_str()) == -1) {
            perror("execute installer failed");
        }

        gh_global::file_unlink(save_file_as);
    }

    exit(0);
#endif
}
