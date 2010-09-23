LSActiveDesktop
===============

LSActiveDesktop lets you display a web page on your desktop. It exposes a
LiteStep object to JavaScripts running on the web page that lets you script
your entire theme using javascript.


Installing and loading the module
---------------------------------

Classic Setup:

    Put LSActiveDesktop.dll in a directory of your choosing, e.g.
    C:\Program Files\Litestep\Modules\. Open your step.rc and add a LoadModule
    line pointing to the dll, for example:

    LoadModule "C:\Program Files\Litestep\Modules\LSActiveDesktop.dll"

NetLoadModule (OTS2) Setup:

    Add the following line to your step.rc/theme.rc/personal.rc/whatever:

    *NetLoadModule LSActiveDesktop-0.4

Then save the file you just edited and recycle LiteStep.


.rc settings and commands
-------------------------

LSActiveDesktopWebWindows windowname1 [windowname2] [windowname3] [...]
    Create one or more LSActiveDesktop web windows. The names you give to your
    windows will be used when you configure them and when you want to send bang
    commands to them. You should replace <WebWindowName> in the following
    settings with the name you specified with this command.

<WebWindowName>X COORDINATE
    Sets the horizontal position of the web window.

<WebWindowName>Y COORDINATE
    Sets the vertical position of the web window.

<WebWindowName>Width INT
    Sets the width of the web window, in pixels.

<WebWindowName>Height INT
    Sets the height of the web window, in pixels.

<WebWindowName>URL STRING
    Sets the initial web page to display. Can be a local file or a page on the
    internet.
    
    Example: LSADMenuURL $ThemeDir$lsactivedesktop\menu.html
    Example: LSADMenuURL http://www.mydomain.com/menu.php

<WebWindowName>HideScrollbars BOOL
    If this command is present it hides the scrollbars of the web window even
    if the web page can't fully be displayed.


!bang commands
--------------

!LSActiveDesktopNavigate <WebWindowName> STRING
    Navigates to the url specified by STRING.

!LSActiveDesktopRunJSFunction <WebWindowName> STRING
    Runs the JavaScript function specified in STRING.
    
    Example: !LSActiveDesktopRunJSFunction LSADMenu collapseMenu()
    Example: !LSActiveDesktopRunJSFunction LSADMenu alert("this is a test")

!LSActiveDesktopBack <WebWindowName>
    Go back in the browsing history (like the back button in your web browser)

!LSActiveDesktopForward <WebWindowName>
    Go forward in the browsing history (like the forward button in your web
    browser)

!LSActiveDesktopRefresh <WebWindowName>
    Refresh the web page (like the refresh button in your web browser)

!LSActiveDesktopRefreshCache <WebWindowName>
    Refresh the web page bypassing the cache (like pressing ctrl+f5 in your
    web browser)


lsexecute hyptertext reference
------------------------------

Web pages shown in a LSActiveDesktop web window can use the special "lsexecute"
hypertext reference (href) on anchors (<a ...>). Use it to execute LiteStep
commands, including launching files.

Examples:

    <a href="lsexecute://'$txteditor$'">Launch the default text editor</a>
    <a href="lsexecute://!recycle">Recycle LiteStep</a>
    <a href="lsexecute://'$browser$' http://tlundberg.com">Go to tlundberg.com</a>


LiteStep JavaScript object
--------------------------

JavaScripts included on web pages shown in LSActiveDesktop web windows can
access a special object called "LiteStep".

The following methods are available:

LiteStep.execute(command)
    Executes a LiteStep command, including launching files.
    
    Examples:
        LiteStep.execute("'$txteditor'");
        LiteStep.execute("!recycle");
        LiteStep.execute("'$browser$' http://tlundberg.com");

LiteStep.writefile(filename, mode, text)
    Writes "text" to "filename", using mode "mode".
    "mode" should be "overwrite" or "append"
    
    Example:
        LiteStep.writefile("C:\\lsadtest.txt", "overwrite", "This is a test");

LiteStep.readfile(filename)
    Reads all the text from "filename" and returns it.
    
    Example:
        var textInFile = LiteStep.readfile("C:\\lsadtest.txt");

LiteStep.getevar(evarName)
    Returns the value of LiteStep environment variable "evarName".
    
    Example:
        var themeDir = LiteStep.getevar('themedir');

LiteStep.setevar(evarName, value)
    Sets the evar "evarName" to the value "value".
    
    Example:
        LiteStep.setevar("MyEvar", "MyValue");


Version history
---------------

Tobbe 2010-09-24 LSActiveDesktop 0.4
    Added a 'LiteStep' object that's accessible from JavaScript code that lets
    you execute LS commands, read/write files and get/set LiteStep's evars.
    First public release.

Tobbe 2010-09-07 LSActiveDesktop 0.3
    Added bang command to call JavaScript function embedded in the current
    web page.
    Added normal "web browser" bang commands, like Back, Forward and Refresh.

Tobbe 2010-09-06 LSActiveDesktop 0.2
    Support for lsexecute:// links

Tobbe 2010-09-02 LSActiveDesktop 0.1
    Local and remote web pages can be shown on the desktop.
    Bang command to navigate to user specified web pages.


Random notes
------------

Based on/ripped from:
http://www.wischik.com/lu/programmer/webform.html

TODO:
* Go through the codeproject comments for "Embed an HTML control in your own window using plain C" and see if any of the bugreports/improvements apply to me too.
    * Same for the comments here: http://www.codeguru.com/Cpp/I-N/ieprogram/article.php/c4379
    * This one I have already fixed, but I would like to be able to verify that my fix works. http://groups.google.com/group/microsoft.public.inetsdk.programming.webbrowser_ctl/browse_frm/thread/a1230f6e0e307cc4/984edb7c33fddaf2?lnk=gst&q=UI_GetExternal#984edb7c33fddaf2
* Test key presses like Tab, Enter etc. Also test in google to make sure the suggest box drops down.