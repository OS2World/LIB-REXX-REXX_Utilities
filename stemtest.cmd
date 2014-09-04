/*******************************************************************/
/*                                                                 */
/* REXXUDEM.CMD                                                    */
/*                                                                 */
/* This program calls the various REXX external functions          */
/* provided in the REXXUTIL.C sample function package.             */
/*                                                                 */
/* Each function is called once to illustrate how it is used.      */
/* Some of the functions have multiple options and multiple        */
/* types of output, but not all options are used here.             */
/*                                                                 */
/* Some of the functions in REXXUTIL only work in windowed         */
/* or fullscreen command prompt sessions, not in PMREXX.           */
/* This is because they are doing vio-type operations such as      */
/* placing the cursor at a particular place on the screen.         */
/* So if this program is run in PMREXX, it will start a new        */
/* windowed session and run itself there.                          */
/*                                                                 */
/*******************************************************************/
'@echo off'

/* Detect if we are running under PMREXX.                          */
/* Some of the REXXUTIL functions (such as SysCurPos) only work    */
/* in OS/2 command prompt sessions, not in PMREXX.                 */
If Address() = 'PMREXX' Then  Do
  Say 'This program will now run itself in a windowed '
  Say '  command prompt session'
  'START REXXUDEM.CMD /C /WIN'
  Exit
  End
  /* We now know we are running in a line-mode session.            */

/*
 * Get rid of anything loaded:
 *  1. Check if SysDropFuncs loaded if yes drop it and load my
 *     SysDropFuncs.
 *  2. Call SysDropFuncs to dump all the functions.
 *  3. Load all the functions.
 */
if rxfuncquery("SysDropFuncs") then say "1- SysDropFuncs not loaded"
else do
    say "1 - SysDropFuncs is loaded, unloading..."
    rc=rxfuncdrop("SysDropFuncs")
    if rxfuncquery("SysDropFuncs") then say "2 - SysDropFuncs not loaded"
end
rc = rxfuncadd('SysDropFuncs','REXXUTIL', 'SysDropFuncs')
call SysDropFuncs


/*
 * Now load all functions
 */
say "Load all functons..."
rc = rxfuncadd('SysLoadFuncs','REXXUTIL', 'SysLoadFuncs')
call SysLoadFuncs
say "Done."


names.0  = 10
names.1  = 'Irina'
names.2  = 'Evgeni'
names.3  = 'Shizuka'
names.4  = 'Jeffrey'
names.5  = 'Sasha'
names.6  = 'Evan'
names.7  = 'Stephane'
names.8  = 'BRIAN'
names.9  = 'Carolina'
names.10 = 'Mira'

/* Sort the entire stem in ascending order, ignoring case.
 */
say SysStemSort('names.')
SAY 'X'names.0'Q'
DO i = 1 TO names.0
    SAY names.i
END

say SysStemSort('names.',, 'C')
DO i = 1 TO names.0
    SAY names.i
END
SAY

/* Now sort the entire stem in descending order, using characters 2 through 4
 * of each element as the sort key.
 */

names.0  = 10
names.1  = 'Irina'
names.2  = 'Evgeni'
names.3  = 'Shizuka'
names.4  = 'Jeffrey'
names.5  = 'Sasha'
names.6  = 'Evan'
names.7  = 'Stephane'
names.8  = 'BRIAN'
names.9  = 'Carolina'
names.10 = 'Mira'

CALL SysStemSort 'names.','D',, 3, 7, 2, 4
DO i = 1 TO names.0
    SAY names.i
END

exit

/*
The following is a sample of output from this example:

  Brian
  Carolina
  Evan
  Evgeni
  Irina
  Jeffrey
  Mira
  Sasha
  Shizuka
  Stephane

  Evgeni
  Evan
  Stephane
  Irina
  Brian
  Mira
  Shizuka
  Jeffrey
  Sasha
  Carolina
*/
