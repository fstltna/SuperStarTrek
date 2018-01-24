# Super Star Trek
A version of the classic game reworked to support running as a BBS door. Now displays ANSI color and attribute settings.
Official support sites: [Official Github Repo](https://github.com/fstltna/SuperStarTrek) - [Official Forum](https://synchronetbbs.org/index.php/forum/super-star-trek) 
![Star Trek Logo](https://SynchronetBBS.org/images/StarTrekLogo.jpg) 


Installation Steps
==
1. Extract files from .zip (You already did this, right?)
2. **mkdir -p /sbbs/doors/sst**
3. **cp sst sst.doc SST_Logo.txt /sbbs/doors/sst**

Run scfg and add a new door with these settings:
**startup directory:** sst
**command line:** ./sst
**multiple concurrent users:** yes
**native executable:** yes
**use shell to execute:** yes
**pause after execution:** yes
everything else defaults...

--
Thats it!