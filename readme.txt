REXX Utilities - rexxutil replacement for eComStation

This is a replacement rexxutil.dll made from toolkit examples, Open OREXX
source, and by me recreating source.  Part of SysStemSort from regutil 1.26
by Patrick TJ McPhee - see license in sysstem.c.

As of 6 Mar 2008 it works with the included demo program, cfgtool, archive
viewer, and ecsmt; however, more testing should be done.

I have a lot of sysstemsort fixed and I am sure sysini is working but not
how well it is working.

************** NOTE **************

This is a disclaimer for what I have witnessed in the usenet groups
over the last month.

1.I have Warp 3, Warp 4, MCP, and Warp Server Advanced CDs.  I do not have
  any of these installed nor do I ever intend to install any of them again.
  All my systems have eCS (specifically rc4) and this is what I have used to
  test this dll.  If you try it on a system that doesn't have eCS 2.0 then
  you have to except the results.

2.I only use JFS (bootable JFS), not hpfs or hpfs386.

3.I do this for fun and not for money.  You get this for free without any
  expectation that I will do anything more with it.

4.If you think doing this is or was a waste of time then that is great, but
  it is my time to waste and none of your business how I waste it.

5.If you use it and it blows up your system then you have every right to
  demand what you paid for it returned.  Get it?

6.If you can do a better job then have at it, the source is included.

************** End of Rant **************

Compiles with Open Watcom 1.7a and above only.


- Contains the math functions from the orexx rexxutil.dll

  SysPi    SysSqrt   SysExp    SysLog    SysLog10  SysSinh
  SysCosh  SysTanh   SysPower  SysSin    SysCos    SysTan
  SysCotan SysArcSin SysArcCos SysArcTan

- I added the following functions:

  - SysECSVer
    Returns the eCS version from the SYSLEVEL.ECS file.  SysVersion('ECS')
    falls through to SysECSVer and if SYSLEVEL.ECS is not found it falls
    through to SysOS2Ver.

  - SysReplaceObject
    Based on WinReplaceObjectClass as suggested by Rick Walsh.

    Function:  SysReplaceObject

    Syntax:    call SysReplaceObject orgClass , newClass, flag

    Params:    orgClass  - class being replaced
               newClass  - new class name
               flag - TRUE  - Replace the function of orgObject with
                              function of newObject
                      FALSE - Undo the replacement of orgObject  with
                              newObject

    Return:    0 if fail, 1 if succeed.

  - SysUtilVersion
    Returns a version of 2.10 to tell difference from older library.



