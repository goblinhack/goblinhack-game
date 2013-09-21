goblinhack
==========

goblinhack - a 2d smooth scrolling ascii dungeon romp 

<pre>
  ____       _     _ _         _   _            _    
 / ___| ___ | |__ | (_)_ __   | | | | __ _  ___| | __
| |  _ / _ \| '_ \| | | '_ \  | |_| |/ _` |/ __| |/ /
| |_| | (_) | |_) | | | | | | |  _  | (_| | (__|   < 
 \____|\___/|_.__/|_|_|_| |_| |_| |_|\__,_|\___|_|\_\

</pre>

Welcome to GoblinHack 1.19. This version adds:                       
                                                                                
  Ineresting changes, monsters, rule changes and stuff:                         
                                                                                
    Fire/cold/... proofing now percentage based                                 
    Differentiate between proofing negative damage monsters, like fire          
    Fire proofing and other proofings were not fully working                    
    2.5D walls for some levels                                                  
    Auto rage now at 1/3 HP                                                     
    Power-up descriptions clarified                                             
    Insulting monsters                                                          
    Fractal landscapes                                                          
    Deep and Dwarve Lords                                                       
    Enter vs exit on level 1                                                    
    Trolls have fewer fate points                                               
    Return exits take longer to appear                                          
    Self teleport screen shake lessened                                         
    No shield protection on teleport attacks                                    
    Named things have fewer extra fate points                                   
    No bravery for killing moving walls or lava pits                            
    No negative damage attacks from lava when fire retardant                    
    Power up to increase you base health.                                       
    Using monsters led to an odd message...                                     
    Suffocation attacks.                                                        
    Summon monster takes into account uniqueness of monster.                    
    Cursed spell of lightness was not working.                                  
    Added cursed spell of zippiness.                                            
    Tutorial level                                                              
    Monsters will go slower, you too, when heavily injured                      
    Fate points only recover half of your health each time used                 
    and this decreases for subsequent uses.                                     
    Base health powerups, helps with fate point usage.                          
                                                                                
  Boring changes, but usually essential:                                        
                                                                                
    Screen dump option was not saving filenames with .gz that led               
    to folks thinking it was corrupt.                                           
    Compiz full screen mode fix, xnuer works                                    
    Configuration menu                                                          
    Quit confirm option. Q for hard quit                                        
    Start menu shortcut fixed                                                   
    make install fixed                                                          
    Binary updater will only retry randomly not every time                      
    Hiscore table format tidy up                                                
                                                                                
Press space/cursor keys to navigate this help. Escape/q to quit.                
                                                                                
<pre>
__        ___           _     _       _ _  ___ 
\ \      / / |__   __ _| |_  (_)___  (_) ||__ \
 \ \ /\ / /| '_ \ / _` | __| | / __| | | __|/ /
  \ V  V / | | | | (_| | |_  | \__ \ | | |_|_| 
   \_/\_/  |_| |_|\__,_|\__| |_|___/ |_|\__(_) 

</pre>
       
Goblin Hack is an ASCII, SDL + opengl based game. It opts for terrible          
graphics and no sound to give a nice simple game that should appeal to          
kids and adults alike.                                                          
                                                                                
Think NetHack, but faster and with fewer keys; and worse graphics ;)            
                                                                                

<pre>
 _   _                 _                    _     _ 
| | | | _____      __ | |_ ___     __ _  __| | __| |
| |_| |/ _ \ \ /\ / / | __/ _ \   / _` |/ _` |/ _` |
|  _  | (_) \ V  V /  | || (_) | | (_| | (_| | (_| |
|_| |_|\___/ \_/\_/    \__\___/   \__,_|\__,_|\__,_|
                                                    
     _          __  __ ___ 
 ___| |_ _   _ / _|/ _|__ \
/ __| __| | | | |_| |_  / /
\__ \ |_| |_| |  _|  _||_| 
|___/\__|\__,_|_| |_|  (_) 

                           
</pre>

You want to add more monsters/treasure/etc...? It's best to look at examples.   
e.g. look at                                                                    
                                                                                
        src/resources/things/monst/ghost/1                                      
                                                                                
        "sneaky ghost"                                                          
        render_depth=3                                                          
        does_stuff                                                              
        score=1                                                                 
        strength=1                                                              
        stamina=100                                                             
        intelligence=1                                                          
        weight=1                                                                
        health=50                                                               
        loves_night                                                             
        loves_full_moon                                                         
        loves_midnight                                                          
        loves_halloween                                                         
        karma=-12                                                               
        bite_damage=1                                                           
        bite_delay_in_tenths=0                                                  
        bite_chance=50                                                          
        metabolism=70                                                           
        can_see_distance=7                                                      
        is_monst                                                                
        loves_void                                                              
        only_in_dungeons                                                        
        is_explorer                                                             
        is_movable                                                              
        is_destroyable                                                          
        is_undead                                                               
        fate=1                                                                  
        attacks=[ "hero" ]                                                      
        attack_chance=[ 90 ]                                                    
        appearing_chance=[ 10 20 30 40 50 40 30 20 ]                            
        explosion_type="explosion/harmless"                                     
        look_delay_in_tenths=5                                                  
        is_transparent                                                          
        opacity=80                                                              
                                                                                
this is the definition file that gives the monst its attributes. There          
are many attributes. For a full list, look at src/include/gh_thing.h.           
Most things in there can be specified. Now look at the 'graphics' file:         
                                                                                
        src/resources/thing/monst/ghost/1.map                                   
                                                                                
and this is the ascii file. It's arranged in sections. First section            
is the graphics, next the foreground colors and then background. Anim           
frames follow sideways.                                                         
                                                                                
        ####################################################                    
        all normal                                                              
        ####################################################                    
        +-----+-----+                                                           
        | .-. | .-. |                                                           
        || oo|||OO ||                                                           
        ||   |||   ||                                                           
        ||   |||   ||                                                           
        |'^^ '|' ^^'|                                                           
        # foreground                                                            
        #  -Black w-white r-red    g-blue y-yellow   b-blue   p-pink   c-cyan   
        # _-gray  W-snow  o-orange G-lime  Y-ltyellow B-ltblue P-purple C-ltcyan
        # D - dark blue O-ltorange                                              
        # 0-9 dk-ltgray   R-random e-darkblue                                   
        +-----+-----+                                                           
        |ppppp|ppppp|                                                           
        |p wwp|pww p|                                                           
        |p   p|p   p|                                                           
        |p   p|p   p|                                                           
        |pww p|p wwp|                                                           
        # background                                                            
        +-----+-----+                                                           
        |     |     |                                                           
        |     |     |                                                           
        |     |     |                                                           
        |     |     |                                                           
        |     |     |                                                           
        # can bite mask; the areas that get bitten or bite                      
        +-----+-----+                                                           
        |     |     |                                                           
        | 111 | 111 |                                                           
        | 111 | 111 |                                                           
        | 111 | 111 |                                                           
        |     |     |                                                           
        # obstacle mask; the areas we use in collisions with wall-like things   
        +-----+-----+                                                           
        |     |     |                                                           
        | 111 | 111 |                                                           
        | 111 | 111 |                                                           
        | 111 | 111 |                                                           
        |     |     |                                                           
        # can touch mask; where we allow other things to interact with us       
        +-----+-----+                                                           
        |     |     |                                                           
        | 111 | 111 |                                                           
        | 111 | 111 |                                                           
        | 111 | 111 |                                                           
        |     |     |                                                           
        +-----+-----+                                                           
                                                                                
After the background section is the bite mask. Any non ' ' char means           
this bit bites in collision detections. Next up is for bumping into             
walls and the last section is how we interact with requests like 'open'.        
                                                                                
Of particular note is the map file in resources/levels/map.                     
If you add a new monster you need to also add it in here also and map           
it to a character - these charaters are often referred to by spells,            
e.g. hit all 'g's, where g maps to goblin/1/2...                                
                                                                                
Other files of note, resources/levels/which to determine what type of level     
you see at any given level.                                                     
                                                                                
The source code lives in src/ and graphics and config files all live in         
src/resources.                                                                  
                                                                                
This file is src/resources/txt/help.txt. Please add to it 8).                   
                                                                                
<pre>

 ____                        
| __ ) _   _  __ _  __ _ ___ 
|  _ \| | | |/ _` |/ _` / __|
| |_) | |_| | (_| | (_| \__ \
|____/ \__,_|\__, |\__, |___/
             |___/ |___/     

</pre>

amulet of unchanging needs to block polymorph
auto rage not working
angels not responding to very low karma
                                                                                
<pre>

  ____            _             _   
 / ___|___  _ __ | |_ __ _  ___| |_ 
| |   / _ \| '_ \| __/ _` |/ __| __|
| |__| (_) | | | | || (_| | (__| |_ 
 \____\___/|_| |_|\__\__,_|\___|\__|


</pre>

mail me at goblinhack@gmail.com                                                 
                                                                                
tx                                                                              
                                                                                
Neil McGIll                                                                     
                                                                                

<pre>
 ___    _                
|_ _|__| | ___  __ _ ___ 
 | |/ _` |/ _ \/ _` / __|
 | | (_| |  __/ (_| \__ \
|___\__,_|\___|\__,_|___/
                         
</pre>

blindness attacks - but what kind of monster?       
slow motion spell, like slow monster, but for all
emergency teleport
wand recharge
traps/click to explode/fire arrows                                              
kobolds?                                                                        
Too easy to gain complete invulnerability - how about absorption x% even -ve    
bravery menu too long - page left/right?                                        
spell dmage depending on karma e.g. holy sword vs vorpal                        
spell books or spell combos                                                     
permanently learn a spell at cost to health/stamina                             
spells that recharge?                                                           
rock tiles when destryed should leave a dust tile perhaps?                      
cleaners should not eat lava                                                    
cleaners should pick up corpses                                                 
crash on returning to hell after bonus                                          
no teleport in/out of shops esp for imps                                        
no monst in shops                                                               
throwing grenades correlate with strenght                                       
pick up grenade; die                                                            
necromancer too weak; need staff weapon                                         
items outside shops; don't mark as thief                                        
instead of thief - slayer?                                                      
save/load system                                                                
food restore stamina                                                            
rotten meat in dungeon light red chicke leg ? "@>~t"                            
crossbows can't be destroyed                                                    
ghosts bravery 0 - more for hoards?                                             
monst info shows dead ghosts                                                    
on use of fate point by monster back off                                        
increase return exit time                                                       
shotgun spray fire                                                              
shop walls tougher?                                                             
spells -> scrolls?                                                              
repair weapons in bravery menu?                                                 
recharging a wand vs gun should be different price                              
karma needs working on, too easy to go bad                                      
bravery item to avoid cursed items                                              
how to recharge individual weapons?                                             
.debs packages                                                                  
hall of fame for survivors                                                      
make named things unique                                                        
lightning rod from eyes                                                         
improve weapon / wand power +1 +2 for those with lots of bravery points         
frost bow arrows look odd in diagonals                                          
hot keys start with 0 - not ~                                                   
manual to describe ctrl/alt hot keys 0-9                                        
skill -> dex                                                                    
Your blow damages appears if you destroy something                              
Poisoning bites armour!?                                                        
unpause any key annoying                                                        
look into fusion icon for compiz                                                
cursed grrenades stay cursed                                                    
screenshot in non ppm                                                           
grenades shown with some kind of gun                                            
amulet of fools - drain brain                                                   
pause - move to pause/break key use 'p' to pickup                               
forest fires!                                                                   
make any monster zombie potential                                               
need blindness attack                                                           
spells to cast lava or acid pools?                                              
wish for spells/items based on karma?                                           
add death worm                                                                  
undercity with zombies?                                                         
pulsing things                                                                  
night vision                                                                    
sell things in shops                                                            
if evil, teleport trap to hell?                                                 
fountains?                                                                      
cursed ring of regeneration                                                     
perma food? one eat per level?                                                  
giant throws rocks                                                              
juggernaut                                                                      
make pumpkins into pumpkin pie!                                                 
good/bad leprechauns - steal your goodies                                       
goose golden eggs                                                               
snowman                                                                         
werewolves                                                                      
poisonous frog                                                                  
intelligent vampires                                                            
carnivorous trees                                                               
chinese dragon                                                                  
docs on powerups?                                                               
polymorph potions                                                               
gremlins                                                                        
credit card                                                                     
O_O meep                                                                        
moon and gemstone water                                                         
teleport when near death                                                        
regeneration to original points                                                 
leprechauns and random teleport                                                 
moles                                                                           
warlock                                                                         
lava should act like lava                                                       
potion to destroy moving and all walls!                                         
vampire mice?                                                                   
trees that sleep until you shoot them - but give you apples                     
vampire turns into mist to go through doors...                                  
goblins shout at each other? argue?                                             
a cat or monster in a cage                                                      
magic lepricauhn; follow him and get hit treasure pot                           
giant worms they make holes in the walls                                        
snail slime                                                                     
baby monster &                                                                  
spell, ha-Bracha dab'ra                                                         
named characters                                                                
spellbooks and rooms (The Library of Forgotten Books)                           
                                                                                
   @  |D - archers?                                                             
 `( )~+                                                                         
   ^                                                                            
  , .                                                                           
                                                                                
   @  \  - archers?                                                             
 `( )~}                                                                         
   ^  /                                                                         
  , .                                                                           
                                                                                
auto open of doors                                                              

<pre>
              ____ _                            _                               
             / ___| |__   __ _ _ __   __ _  ___| |    ___   __ _                
            | |   | '_ \ / _` | '_ \ / _` |/ _ \ |   / _ \ / _` |               
            | |___| | | | (_| | | | | (_| |  __/ |__| (_) | (_| |               
             \____|_| |_|\__,_|_| |_|\__, |\___|_____\___/ \__, |               
                                     |___/                 |___/                

</pre>
                                                                                
Tue Mar 24 14:43:01 EDT 2009                                                    
----------------------------                                                    
                                                                                
Some monsters have treasure and weapon hoards                                   
Minotaur class                                                                  
Civil ward soldier class (for Ben)                                              
Musket (for the above!)                                                         
New monsters, squarm, smurlinger, morhock, lich, galump                         
Warhammers                                                                      
Removed dex requirement on swords and axes                                      
Less of a skill bonus for multi fate monsters                                   
Made wand intelligence reqs all the same at 50                                  
Generator of doom crash on aliens (thanks MaxZ)                                 
Item drop crash fix (thanks Eli)                                                
Windows install would fail if a path was given with no /                        
                                                                                
Thu Jan  8 17:49:06 EST 2009                                                    
----------------------------                                                    
                                                                                
More trolls and goblin types                                                    
Shop bugfixes                                                                   
Installer fixes                                                                 
No teleport for shopkeeper                                                      
Excalibur easily damaged fixed                                                  
Broad sword and other swords downgraded to they get damaged by stone            
Yellow ghosts could not move                                                    
Org generators not attacking                                                    
New orc class                                                                   
More troll types                                                                
More goblin types                                                               
Fewer imps                                                                      
Fewer spawning goblins                                                          
Faster centipedes                                                               
Moving walls more dangerous                                                     
Amulets and armour not being listed on itinery                                  
Edge of level impentrable                                                       
Sleeping mines                                                                  
                                                                                
Sun Dec 21 16:38:20 EST 2008                                                    
----------------------------                                                    
                                                                                
New features: music by Pascal Provost,                                          
              Level shake                                                       
              Nice floor tiles                                                  
              Monster AI decision making                                        
              Flash when hit                                                    
              Bones files                                                       
              Smoother scrolling                                                
              Auto updater,                                                     
              Bit rock installer                                                
              Shortcut keys                                                     
              Pausing and saving reduces bravery points                         
              followers can follow you through exits                            
              Secret walls                                                      
              Amulets                                                           
              Named monsters                                                    
                                                                                
New spells: summon monster, monster walls, living wall, rock wall,              
            instafood, living shield, guardian angel/demon, exit wall,          
            snake charmer, summon undeaad, poison, sleeping, paralysis and      
            slow gas/spells, invisibility, raise dead, polymorph                
                                                                                
New monsters: sandman and poison mushrooms, stomper, jumping monsters           
              robot roach, more snakes, spiders, fire lord, jumper monster,     
              millipedes, golems, snake and centipede rooms, more dragons       
              with treasure hoards, gold beetles, poison zombies, more          
              puddings, clusters of lava, carnivorous mist, living ice,         
              black ice, dezaugl (greater demon), leeches, grass snakes,        
              death knight, orc generator, hydra, mind flayer, medusa           
                                                                                
New abilities: monsters with sleep attacks                                      
               valkyrie and barbarian raging and auto raging                    
               solider with bazooka, guns, laser and grenades                   
               necromancer and warlocks                                         
               soldiers, seargant and saboteur                                  
               teleport once per level power up                                 
               teleport permanently power up                                    
               tireless swing power up                                          
               tireless power up                                                
               rock mining possible                                             
               dexterity requirements and powerups                              
               intelligence needed for spells                                   
               evil things can shop now but items cost more                     
               regenerating monsters and powers                                 
                                                                                
New levels: caves, hell, forest, the void, bonus levels, end game               
                                                                                
New weapons: ice and fire axes, poison blades, vorpal weapons                   
             Holy, evil and enchanted weapons, flaming arrows,                  
             silver daggers and gun (quad damage on undead),                    
             suit of armour (jgs), stun grenades, poison grenades,              
             rocket launcher, mines, dynamites, shotgun, deathray               
                                                                                
Bugs fixed:                                                                     
              general speed                                                     
              spawning slowdowns                                                
              armor more effective                                              
              endless shop bugs                                                 
              spells have no weight                                             
              featherlight spell fixed                                          
              fixed uncurse spell                                               
              shopkeepers and possessed monsters shots no longer hurt           
              you if you are in the way of their target.                        
                                                                                
version 1.15 Sat Feb  2 00:13:31 GMT 2008                                       
-----------------------------------------                                       
                                                                                
dig wand                                                                        
poison arrows                                                                   
teletrap                                                                        
vampire knights                                                                 
rock and lava outside the level                                                 
poison zombies                                                                  
vampires bite fate points                                                       
bravery points bug fixes                                                        
prefer non cursed/paid for items when using                                     
made hard monsters less frequent                                                
too much treasre                                                                
patricia's pantry fixed                                                         
                                                                                
version 1.13 Wed Jan 23 01:45:40 GMT 2008                                       
-----------------------------------------                                       
                                                                                
bravery points                                                                  
power ups                                                                       
black widow, goliath and wandering spiders with poison                          
shields on teleport and fate point and level start                              
crash recovery (auto save)                                                      
thieiving is more punishing                                                     
trapdoor spiders                                                                
bat caves                                                                       
source code rename zx->gh                                                       
bouncing explosions                                                             
added character classes, sorceror, barbarian, warrior etc...                    
pet vampires                                                                    
elves and dwarves with armour and variable items                                
sound effects                                                                   
spinning swords                                                                 
characters have initial backpacks of items now                                  
intelligence, stamina, fatigue effects                                          
rearraged main screen for clarity                                               
demon and angels tougher to kill                                                
dragons a bit easier                                                            
more monsters                                                                   
lightning and lasers changed to be inert, not cold/hot effect                   
fate points more generous, gets you back to 20 hp                               
shops more likely                                                               
threaded main loop                                                              
                                                                                
version 1.11 aug 27 2007                                                        
------------------------                                                        
                                                                                
animated dragons                                                                
gnasher                                                                         
multi layer ascii                                                               
jelly molds                                                                     
midnight fix                                                                    
key press fires and starts fix                                                  
teleport sleeping fix                                                           
blobs                                                                           
black dragons                                                                   
amethyst dragons                                                                
fate points                                                                     
fractal lakes                                                                   
more armour types - acid/fire/water/coldproof                                   
more orcs and goblin types                                                      
bite armour bug                                                                 
                                                                                
version 1.1 Sun Apr  8 09:52:35 EDT 2007                                        
-----------                                                                     
                                                                                
weightless spell for items                                                      
monster spying                                                                  
invisible man                                                                   
too many keys bugfix                                                            
exit warp and exploding exits                                                   
crown                                                                           
hungry treasure chest                                                           
                                                                                
version 1.0 Thu Mar 22 22:54:07 EDT 2007                                        
-----------                                                                     
                                                                                
lightning gun                                                                   
added karma counter back in                                                     
huge speed improvement for exploding potions                                    
exploding potions and some monsters                                             
teleport bug fix                                                                
spells, remove curse, slow/fast, fireball, iceball,                             
lightning starburst, warp, blind, eagle vision                                  
                                                                                
version 0.99 Sun Feb 25 18:59:21 EST 2007                                       
------------                                                                    
                                                                                
milk                                                                            
more dangerous potions                                                          
armour has weight now                                                           
cursed armour added                                                             
chocolate/ice/liquid/dragon armour                                              
broken/cell doors; can't see through solid doors now!                           
big levels play faster now                                                      
dimensional rifts (might need additional tweaking)                              
killer rat                                                                      
little mouse                                                                    
glorks                                                                          
killer tar                                                                      
lasers and mini lasers                                                          
quit and you get in the high score table - not in nethack                       
killing shopkeeper and stealing allowed now                                     
bonans armoury has armour!                                                      
angels now get annoyed                                                          
rust monster eats treasure                                                      
                                                                                
version 0.97 Sat Jan 20 10:16:50 EST 2007                                       
-----------                                                                     
                                                                                
spirites, imps, treasure stealing, white witches, moving walls,                 
firebugs and nests, chocolate slime molds, sherbet slime,                       
fallen fairies, faster gfx, armouries, food shops, shop bugs fixed,             
save/load bugs fixed, corpses                                                   
                                                                                
version 0.94                                                                    
-----------                                                                     
                                                                                
full moon/night/friday 13th/midnight nethack ripoff                             
                                                                                
version 0.93                                                                    
-----------                                                                     
                                                                                
gcc 4 fixes                                                                     
                                                                                
version 0.92 Sun Dec 31 01:15:07 GMT 2006                                       
-----------                                                                     
                                                                                
karma                                                                           
haloball and lavaball                                                           
angels and demons                                                               
random flicker was too fast                                                     
double damage for fire/ice loving hating creatures                              
load not working for visited rooms                                              
64 bit fixes                                                                    
                                                                                
version 0.9 Wed Dec 20 22:45:21 EST 2006                                        
-----------                                                                     
                                                                                
save/load                                                                       
                                                                                
version 0.8 Mon Nov 27 00:29:11 GMT 2006                                        
-----------                                                                     
                                                                                
Shops added. Also, multiple weapons can be carried now.                         
Police also added for those tempted to steal from shopkeepers...                
                                                                                
version 0.7 Mon Nov 20 17:43:35 CET 2006                                        
-----------                                                                     
                                                                                
Why buy a PS3 when Goblin Hack! is free 8) Thanksgiving                         
release does not add turkeys, but treasure and monster                          
rooms, graveyards, dragon's lairs, electro doors and more.                      
                                                                                
version 0.6 Sun Oct 22 21:01:40 EST 2006                                        
-----------                                                                     
                                                                                
Halloween edition. Magic potions. spider webs, more ghosts                      
pumpkins, goodly and evil knights, stoat packs, killer bees,                    
maggots! centipedes, black mamba, creepy things, more treasure,                 
more bugs! skeleton keys, ported to MacOS                                       
                                                                                
compiles on MacOS (thanks to "Robert Serocki" <robert@network.ucsd.edu>)        
BUG FIXED: can't shoot doors                                                    
                                                                                
version 0.5 Sat Sep 23 23:21:50 EST 2006                                        
-----------                                                                     
                                                                                
BUG FIXED: no more exits next to doors                                          
BUG FIXED: can shoot through open doors                                         
FEATURE: pause button and help screen                                           
                                                                                
version 0.4 sep 17 2006                                                         
-----------                                                                     
                                                                                
Worlds first ASCII snakes; anacondas and copperheads.                           
Yet more monsters; crack of doom, flying witch, creepy                          
button eyed thing and more treasure added.                                      
                                                                                
giant anacondas! and copperhead ascii snakes                                    
crack of doom monster                                                           
button eyed monster                                                             
added cackling flying witch                                                     
better giant bat graphics                                                       
make depend added                                                               
more treasure - piles of dollar bills                                           
BUG: light flicker fixed                                                        
BUG: gcc 4.1.1 fixes                                                            
BUG: lava pits can now sneak up under the hero                                  
BUG: stdout forever increasing in size bug fix                                  
                                                                                
version 0.3                                                                     
-----------                                                                     
                                                                                
windows installer wasn't working!                                               
mail goblinhack@gmail.com for problems                                          
vampire bats that change into vampires and back again; really hard to beat      
coins and some monsters now appear in clusters/groups                           
numeric keypad can be used for moving                                           
weapons no longer fire through walls! 8)                                        
compiles on freebsd (thanks to "Dmitry Marakasov" <amdmi3@mail.ru>)             
added make install                                                              
                                                                                
version 0.2 aug 2006                                                            
---------------------                                                           
                                                                                
installer for windows                                                           
triple shot fireball, iceball and plasmaball                                    
more monsters, vampires, ogres, huge beholder with starburst fireball           
more treasure                                                                   
improved lighting                                                               
gcc 4.0 support                                                                 
bug fixes for shot damage                                                       
                                                                                

<pre>
 _                     _   ____  _          __  __ 
| |    ___  __ _  __ _| | / ___|| |_ _   _ / _|/ _|
| |   / _ \/ _` |/ _` | | \___ \| __| | | | |_| |_ 
| |__|  __/ (_| | (_| | |  ___) | |_| |_| |  _|  _|
|_____\___|\__, |\__,_|_| |____/ \__|\__,_|_| |_|  
           |___/                                   

</pre>

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

