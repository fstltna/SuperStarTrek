#include "sst.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void attakreport(void) {
	if (future[FCDBAS] < 1e30) {
		proutn("\033[31mStarbase in \033[32m");
		cramlc(1, batx, baty);
		prout(" \033[31mis currently under attack.");
		proutn("It can hold out until Stardate \033[32m");
		cramf(future[FCDBAS], 0,1);
		prout("\033[37m.");
	}
	if (isatb == 1) {
		proutn("\033[31mStarbase in \033[32m");
		cramlc(1, d.isx, d.isy);
		prout(" \033[31mis under Super-commander attack.");
		proutn("It can hold out until Stardate \033[32m");
		cramf(future[FSCDBAS], 0, 1);
		prout("\033[37m.");
	}
}
	

void report(int f) {
	char *s1,*s2,*s3;

	chew();
	s1 = (thawed?"thawed ":"");
	switch (length) {
		case 1: s2="short"; break;
		case 2: s2="medium"; break;
		case 4: s2="long"; break;
		default: s2="unknown length"; break;
	}
	switch (skill) {
		case SNOVICE: s3="novice"; break;
		case SFAIR: s3="fair"; break;
		case SGOOD: s3="good"; break;
		case SEXPERT: s3="expert"; break;
		case SEMERITUS: s3="emeritus"; break;
		default: s3="skilled"; break;
	}
	printf("\nYou %s playing a %s%s %s game.\n",
		   alldone? "were": "are now", s1, s2, s3);
	if (skill>SGOOD && thawed && !alldone) prout("No plaque is allowed.");
	if (tourn) printf("This is tournament game \033[32m%d\033[37m.\n", tourn);
	if (f) printf("Your secret password is \"\033[31m%s\033[37m\"\n",passwd);
	printf("\033[32m%d\033[37m of \033[32m%d\033[37m Klingon ships have been destroyed",
		   d.killk+d.killc+d.nsckill, inkling);
	if (d.killc) printf(", including \033[32m%d\033[37m Commander%s.\n", d.killc, d.killc==1?"":"s");
	else if (d.killk+d.nsckill > 0) prout(", but no Commanders.");
	else prout(".");
	if (skill > SFAIR) printf("The Super Commander has %sbeen destroyed.\n",
						  d.nscrem?"not ":"");
	if (d.rembase != inbase) {
		proutn("There ");
		if (inbase-d.rembase==1) proutn("has been 1 base");
		else {
			proutn("have been ");
			crami(inbase-d.rembase, 1);
			proutn(" bases");
		}
		proutn(" destroyed, ");
		crami(d.rembase, 1);
		prout(" remaining.");
	}
	else printf("There are \033[32m%d\033[37m bases.\n", inbase);
	if (REPORTS || iseenit) {
		/* Don't report this if not seen and
			either the radio is dead or not at base! */
		attakreport();
		iseenit = 1;
	}
	if (casual) printf("\033[32m%d\033[37m casualt%s suffered so far.\n",
					   casual, casual==1? "y" : "ies");
#ifdef CAPTURE
    if (brigcapacity != brigfree) printf("\033[32m%d\033[37m Klingon%s in brig.\n",
    							brigcapacity-brigfree, brigcapacity-brigfree>1 ? "s" : "");
    if (kcaptured > 0) printf("\033[32m%d\033[37m captured Klingon%s turned in to Star Fleet.\n", 
                               kcaptured, kcaptured>1 ? "s" : "");
#endif
	if (nhelp) printf("There were \033[32m%d\033[37m call%s for help.\n",
					  nhelp, nhelp==1 ? "" : "s");
	if (ship == IHE) {
		proutn("You have \033[32m");
		if (nprobes) crami(nprobes,1);
		else proutn("no");
		proutn("\033[37m deep space probe");
		if (nprobes!=1) proutn("s");
		prout(".");
	}
	if (REPORTS && future[FDSPROB] != 1e30) {
		if (isarmed) 
			proutn("An armed deep space probe is in");
		else
			proutn("A deep space probe is in");
		cramlc(1, probecx, probecy);
		prout(".");
	}
	if (icrystl) {
		if (cryprob <= .05)
			prout("Dilithium crystals aboard ship...not yet used.");
		else {
			int i=0;
			double ai = 0.05;
			while (cryprob > ai) {
				ai *= 2.0;
				i++;
			}
			printf("Dilithium crystals have been used \033[32m%d\033[37m time%s.\n",
				   i, i==1? "" : "s");
		}
	}
	skip(1);
}
	
void lrscan(void) {
	int x, y;
	chew();
	if (damage[DLRSENS] != 0.0) {
		/* Now allow base's sensors if docked */
		if (condit != IHDOCKED) {
			prout("\033[31mLONG-RANGE SENSORS DAMAGED\033[37m.");
			return;
		}
		skip(1);
		proutn("Starbase's long-range scan for");
	}
	else {
		skip(1);
		proutn("Long-range scan for");
	}
	cramlc(1, quadx, quady);
	skip(1);
	if (coordfixed)
	for (y = quady+1; y >= quady-1; y--) {
		for (x = quadx-1; x <= quadx+1; x++) {
			if (x == 0 || x > 8 || y == 0 || y > 8)
				printf("   -1");
			else {
				printf("%5d", d.galaxy[x][y]);
				// If radio works, mark star chart so
				// it will show current information.
				// Otherwise mark with current
				// value which is fixed. 
				starch[x][y] = damage[DRADIO] > 0 ? d.galaxy[x][y]+1000 :1;
			}
		}
		putchar('\n');
	}
	else
	for (x = quadx-1; x <= quadx+1; x++) {
		for (y = quady-1; y <= quady+1; y++) {
			if (x == 0 || x > 8 || y == 0 || y > 8)
				printf("   -1");
			else {
				printf("%5d", d.galaxy[x][y]);
				// If radio works, mark star chart so
				// it will show current information.
				// Otherwise mark with current
				// value which is fixed. 
				starch[x][y] = damage[DRADIO] > 0 ? d.galaxy[x][y]+1000 :1;
			}
		}
		putchar('\n');
	}

}

void dreprt(void) {
	int jdam = FALSE, i;
	chew();

	for (i = 1; i <= ndevice; i++) {
		if (damage[i] > 0.0) {
			if (!jdam) {
				skip(1);
				prout("DEVICE            -REPAIR TIMES-");
				prout("                IN FLIGHT   DOCKED");
				jdam = TRUE;
			}
			printf("  %16s ", device[i]);
			cramf(damage[i]+0.05, 8, 2);
			proutn("  ");
			cramf(docfac*damage[i]+0.005, 8, 2);
			skip(1);
		}
	}
	if (!jdam) prout("\033[32mAll devices functional\033[37m.");
}

void chart(int nn) {
	int i,j;

	chew();
	skip(1);
	if (stdamtim != 1e30 && stdamtim != d.date && condit == IHDOCKED) {
		prout("\033[34mSpock\033[37m-  \"I revised the Star Chart from the");
		prout("  starbase's records.\"");
		skip(1);
	}
	if (nn == 0) prout("\033[35mSTAR CHART FOR THE KNOWN GALAXY\033[37m");
	if (stdamtim != 1e30) {
		if (condit == IHDOCKED) {
			/* We are docked, so restore chart from base information -- these values won't update! */
			stdamtim = d.date;
			for (i=1; i <= 8 ; i++)
				for (j=1; j <= 8; j++)
					if (starch[i][j] == 1) starch[i][j] = d.galaxy[i][j]+1000;
		}
		else {
			proutn("(Last surveillance update \033[32m");
			cramf(d.date-stdamtim, 0, 1);
			prout("\033[37m stardates ago.)");
		}
	}
	if (nn ==0) skip(1);

	prout("      1    2    3    4    5    6    7    8");
	prout("    ----------------------------------------");
	if (nn==0) prout("  -");
	if (coordfixed)
	for (j = 8; j >= 1; j--) {
		printf("%d -", j);
		for (i = 1; i <= 8; i++) {
			if (starch[i][j] < 0) // We know only about the bases
				printf("  .1.");
			else if (starch[i][j] == 0) // Unknown
				printf("  ...");
			else if (starch[i][j] > 999) // Memorized value
				printf("%5d", starch[i][j]-1000);
			else
				printf("%5d", d.galaxy[i][j]); // What is actually there (happens when value is 1)
		}
		prout("  -");
	}
	else
	for (i = 1; i <= 8; i++) {
		printf("%d -", i);
		for (j = 1; j <= 8; j++) {
			if (starch[i][j] < 0) // We know only about the bases
				printf("  .1.");
			else if (starch[i][j] == 0) // Unknown
				printf("  ...");
			else if (starch[i][j] > 999) // Memorized value
				printf("%5d", starch[i][j]-1000);
			else
				printf("%5d", d.galaxy[i][j]); // What is actually there (happens when value is 1)
		}
		prout("  -");
	}
	if (nn == 0) {
		skip(1);
		crmshp();
		proutn(" is currently in");
		cramlc(1, quadx, quady);
		skip(1);
	}
}
		
		
void srscan(int l) {
	static char requests[][3] =
		{"","da","co","po","ls","wa","en","to","sh","kl","ti"};
	char *cp;
	int leftside=TRUE, rightside=TRUE, i, j, jj, k=0, nn=FALSE;
	int goodScan=TRUE;
	switch (l) {
		case 1: // SRSCAN
			if (damage[DSRSENS] != 0) {
				/* Allow base's sensors if docked */
				if (condit != IHDOCKED) {
					prout("\033[31mSHORT-RANGE SENSORS DAMAGED\033[37m");
					goodScan=FALSE;
				}
				else
					prout("[Using starbase's sensors]");
			}
			if (goodScan)
				starch[quadx][quady] = damage[DRADIO]>0.0 ?
									   d.galaxy[quadx][quady]+1000:1;
			scan();
			if (isit("chart")) nn = TRUE;
			if (isit("no")) rightside = FALSE;
			chew();
			prout("\n    1 2 3 4 5 6 7 8 9 10");
			break;
		case 2: // REQUEST
			while (scan() == IHEOL)
				printf("Information desired? ");
			chew();
			for (k = 1; k <= 10; k++)
				if (strncmp(citem,requests[k],min(2,strlen(citem)))==0)
					break;
			if (k > 10) {
				prout("\033[33mUNRECOGNIZED REQUEST\033[37m. \033[32mLegal requests are\033[37m:\n"
					 "  date, condition, position, lsupport, warpfactor,\n"
					 "  energy, torpedoes, shields, klingons, time.");
				return;
			}
			// no "break"
		case 3: // STATUS
			chew();
			leftside = FALSE;
			skip(1);
	}
	for (i = 1; i <= 10; i++) {
		int jj = (k!=0 ? k : i);
		if (leftside) {
			if (coordfixed) {
				printf("%2d  ", 11-i);
				for (j = 1; j <= 10; j++) {
					if (goodScan || (abs((11-i)-secty)<= 1 && abs(j-sectx) <= 1))
						printf("%c ",quad[j][11-i]);
					else
						printf("- ");
				}
			} else {
				printf("%2d  ", i);
				for (j = 1; j <= 10; j++) {
					if (goodScan || (abs(i-sectx)<= 1 && abs(j-secty) <= 1))
						printf("%c ",quad[i][j]);
					else
						printf("- ");
				}
			}
		}
		if (rightside) {
			switch (jj) {
				case 1:
					printf(" \033[1mStardate\033[0m      %.1f", d.date);
					break;
				case 2:
					if (condit != IHDOCKED) newcnd();
					switch (condit) {
						case IHRED: cp = "\033[31mRED\033[37m"; break;
						case IHGREEN: cp = "\033[32mGREEN\033[37m"; break;
						case IHYELLOW: cp = "\033[33mYELLOW\033[37m"; break;
						case IHDOCKED: cp = "DOCKED"; break;
					}
					printf(" \033[1mCondition\033[0m     %s", cp);
#ifdef CLOAKING
				    if (iscloaked) printf(", \033[34mCLOAKED\033[37m");
#endif
					break;
				case 3:
					printf(" \033[1mPosition\033[0m     ");
					cramlc(0, quadx, quady);
					putchar(',');
					cramlc(0, sectx, secty);
					break;
				case 4:
					printf(" \033[1mLife Support\033[0m  ");
					if (damage[DLIFSUP] != 0.0) {
						if (condit == IHDOCKED)
							printf("\033[31mDAMAGED\033[37m, supported by starbase");
						else
							printf("\033[31mDAMAGED\033[37m, reserves=%4.2f", lsupres);
					}
					else
						printf("\033[32mACTIVE\033[37m");
					break;
				case 5:
					printf(" \033[1mWarp Factor\033[0m   %.1f", warpfac);
					break;
				case 6:
					printf(" \033[1mEnergy\033[0m        %.2f", energy);
					break;
				case 7:
					printf(" \033[1mTorpedoes\033[0m     %d", torps);
					break;
				case 8:
					printf(" \033[1mShields\033[0m       ");
					if (damage[DSHIELD] != 0)
						printf("\033[31mDAMAGED\033[37m,");
					else if (shldup)
						printf("\033[32mUP\033[37m,");
					else
						printf("\033[34mDOWN\033[37m,");
					printf(" %d%% %.1f units",
						   (int)((100.0*shield)/inshld + 0.5), shield);
					break;
				case 9:
					printf(" \033[1mKlingons Left\033[0m %d", d.remkl);
					break;
				case 10:
					printf(" \033[1mTime Left\033[0m     %.2f", d.remtime);
					break;
			}
					
		}
		skip(1);
		if (k!=0) return;
	}
	if (nn) chart(1);
}
			
			
void eta(void) {
	int key, ix1, ix2, iy1, iy2, prompt=FALSE;
	int wfl;
	double ttime, twarp, tpower;
	if (damage[DCOMPTR] != 0.0) {
		prout("\033[31mCOMPUTER DAMAGED, USE A POCKET CALCULATOR\033[37m.");
		skip(1);
		return;
	}
	if (scan() != IHREAL) {
		prompt = TRUE;
		chew();
		proutn("Destination quadrant and/or sector? ");
		if (scan()!=IHREAL) {
			huh();
			return;
		}
	}
	iy1 = aaitem +0.5;
	if (scan() != IHREAL) {
		huh();
		return;
	}
	ix1 = aaitem + 0.5;
	if (scan() == IHREAL) {
		iy2 = aaitem + 0.5;
		if (scan() != IHREAL) {
			huh();
			return;
		}
		ix2 = aaitem + 0.5;
	}
	else {	// same quadrant
		ix2 = ix1;
		iy2 = iy1;
		ix1 = quady;	// ya got me why x and y are reversed!
		iy1 = quadx;
	}

	if (ix1 > 8 || ix1 < 1 || iy1 > 8 || iy1 < 1 ||
		ix2 > 10 || ix2 < 1 || iy2 > 10 || iy2 < 1) {
		huh();
		return;
	}
	dist = sqrt(square(iy1-quadx+0.1*(iy2-sectx))+
				square(ix1-quady+0.1*(ix2-secty)));
	wfl = FALSE;

	if (prompt) prout("Answer \"no\" if you don't know the value:");
	while (TRUE) {
		chew();
		proutn("Time or arrival date? ");
		if (scan()==IHREAL) {
			ttime = aaitem;
			if (ttime > d.date) ttime -= d.date; // Actually a star date
			if (ttime <= 1e-10 ||
				(twarp=(floor(sqrt((10.0*dist)/ttime)*10.0)+1.0)/10.0) > 10) {
				prout("\033[33mWe'll never make it, sir\033[37m.");
				chew();
				return;
			}
			if (twarp < 1.0) twarp = 1.0;
			break;
		}
		chew();
		proutn("Warp factor? ");
		if (scan()== IHREAL) {
			wfl = TRUE;
			twarp = aaitem;
			if (twarp<1.0 || twarp > 10.0) {
				huh();
				return;
			}
			break;
		}
		prout("\033[35mCaptain, certainly you can give me one of these\033[37m.");
	}
	while (TRUE) {
		chew();
		ttime = (10.0*dist)/square(twarp);
		tpower = dist*twarp*twarp*twarp*(shldup+1);
		if (tpower >= energy) { // Suggestion from Ethan Staffin -- give amount needed
			prout("\033[31mInsufficient energy, sir: we would need \033[32m");
			cramf(tpower, 1, 1);
			proutn (" \033[31munits\033[37m.");
			if (shldup==0 || tpower > energy*2.0) {
				if (!wfl) return;
				proutn("New warp factor to try? ");
				if (scan() == IHREAL) {
					wfl = TRUE;
					twarp = aaitem;
					if (twarp<1.0 || twarp > 10.0) {
						huh();
						return;
					}
					continue;
				}
				else {
					chew();
					skip(1);
					return;
				}
			}
			prout("But if you lower your shields,");
			proutn("remaining");
			tpower /= 2;
		}
		else
			proutn("Remaining");
		proutn(" energy will be ");
		cramf(energy-tpower, 1, 1);
		prout(".");
		if (wfl) {
			proutn("\033[33mAnd we will arrive at stardate \033[32m");
			cramf(d.date+ttime, 1, 1);
			prout("\033[37m.");
		}
		else if (twarp==1.0)
			prout("\033[32mAny warp speed is adequate\033[37m.");
		else {
			proutn("\033[33mMinimum warp needed is \033[32m");
			cramf(twarp, 1, 2);
			skip(1);
			proutn("\033[33mand we will arrive at stardate \033[32m");
			cramf(d.date+ttime, 1, 2);
			prout("\033[37m.");
		}
		if (d.remtime < ttime)
			prout("\033[31mUnfortunately, the Federation will be destroyed by then\033[37m.");
		if (twarp > 6.0)
			prout("\033[33mYou'll be taking risks at that speed, Captain\033[37m");
		if ((isatb==1 && d.isy == ix1 && d.isx == iy1 &&
			 future[FSCDBAS]< ttime+d.date)||
			(future[FCDBAS]<ttime+d.date && baty==ix1 && batx == iy1))
			prout("\033[31mThe starbase there will be destroyed by then\033[37m.");
		proutn("New warp factor to try? ");
		if (scan() == IHREAL) {
			wfl = TRUE;
			twarp = aaitem;
			if (twarp<1.0 || twarp > 10.0) {
				huh();
				return;
			}
		}
		else {
			chew();
			skip(1);
			return;
		}
	}
			
}
