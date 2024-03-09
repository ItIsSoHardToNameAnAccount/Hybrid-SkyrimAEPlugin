# Hybrid-SkyrimAEPlugin
Skyrim SKSE plugin allow player become both werewolf and vampire.
# Overview
- This mod is made to improve the experience of survival mode, also give you the reason to become a dark creature.
- In orignal game, there is no specific reason to become either vampire or werewolf, the mighty dragonborn is just like a god and fear of nothing.
- I like the idea of survival mode, but to be honest, survival mode sucks. I play game for fun, not for suffer.
# What this mod does
- Werewolf
  * As a werewolf, you won't be exauhted, and you gain a level based carry weight improve.
- Vamipre
  * As a vampire, hungry is no longer a problem. especially for a vampire lord.
  * Cold also have no effect to an undead.
  * You gain extra speed and jump height in the night.
- Hybrid
  * Have all the advantages and ignore the weeks.
  * Cancel werewolf beast form anytime you want by press sneak key.
- Human
  * Last but not least, when you level-up over 80, fast travel will be unlocked.
# How to be a hybrid
- First of all, you need to become a werewolf.
- Then you go to Hakon and let him to turn you to a vampire lord.
- Once you become a vampire lord you will lost the beast blood but gain a wolf soul.
- Equip the Hircine's Cursed Ring to revice Hircine's curse.
- Wait until the Cursed Ring turn you into werewolf beast form, and you are a hybrid.
# Knowing Issues
- Player's Race set to "PlayerRace"
  * This is an original bug, at least not caused by this mod. When game set player's race to related vampire race, player's race become unrecognized as a "PlayerRace", then game can not get player's race through Game.GetPlayer().GetRace(). Back to mortal race seems don't have this problem.
# Requirement
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
- Official Survival Mode
- [powerofthree's Tweaks](https://www.nexusmods.com/skyrimspecialedition/mods/51073)
  * Soft required, without this, you won't able to jump higher as a vampire.
# Credit
- [SkyrimScripting](https://github.com/mrowrpurr) for her template and video.
- [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)
- [GamerPoets](https://www.youtube.com/watch?v=l3FuJA1UGvQ) for his vedio to help me setup creation kit enviroment.
- [Nukem](https://www.nexusmods.com/skyrimspecialedition/mods/20061) for the creation kit fix, really a life saver.
