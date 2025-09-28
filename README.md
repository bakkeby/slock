This is a version of slock that is built on [slock-flexipatch](https://github.com/bakkeby/slock-flexipatch) and the main difference between this and other builds of slock is that this one comes with runtime configuration.

That means that slock can be reconfigured without having to recompile it.

After compiling copy the `slock.cfg` file from the project directory to `~/.config/slock/` or the XDG config home equivalent if you are using that.

Refer to [https://tools.suckless.org/slock/](https://tools.suckless.org/slock/) for details on the
slock tool, how to install it and how it works.

---

### Notable additions to slock (beyond patches)

   - runtime configuration
   - exit command (run a command after entering a valid password)
   - failure command (run a command after entering the wrong password too many times)
   - screenshot without imlib2
   - can apply graphical filters on background image, as keyboard feedback and on failure

### Patches incorporated:

   - [alpha](https://github.com/khuedoan/slock)
      - enables transparency for slock
      - intended to be combined with a compositor that can blur the transparent background

   - [auto-timeout](https://tools.suckless.org/slock/patches/auto-timeout/)
      - allows for a command to be executed after a specified time of inactivity

   - [background_image](https://tools.suckless.org/slock/patches/background-image/)
      - sets the lockscreen picture to a background image

   - [blur_pixelated_screen](https://tools.suckless.org/slock/patches/blur-pixelated-screen/)
      - sets the lockscreen picture to a blured or pixelated screenshot

   - [capscolor](https://tools.suckless.org/slock/patches/capscolor/)
      - adds an additional color to indicate the state of Caps Lock

   - [control-clear](https://tools.suckless.org/slock/patches/control-clear/)
      - with this patch slock will no longer change to the failure color if a control key is pressed
        while the buffer is empty
      - this may be useful if, for example, you wake your monitor up by pressing a control key and
        don't want to spoil the detection of failed unlocking attempts

   - [dpms](https://tools.suckless.org/slock/patches/dpms/)
      - interacts with the Display Power Signaling and automatically shuts down the monitor after a
        configurable amount of seconds
      - the monitor will automatically be activated by pressing a key or moving the mouse and the
        password can be entered then

   - [dwmlogo](https://tools.suckless.org/slock/patches/dwmlogo/)
      - draws the dwm logo which changes color based on the state

   - [failure-command](https://tools.suckless.org/slock/patches/failure-command/)
      - allows for a command to be run after a specified number of incorrect attempts

   - [keypress_feedback](https://tools.suckless.org/slock/patches/keypress-feedback/)
      - draws random blocks on the screen to display keypress feedback

   - [mediakeys](https://tools.suckless.org/slock/patches/mediakeys/)
      - allows media keys to be used while the screen is locked, e.g. adjust volume or skip to the
        next song without having to unlock the screen first

   - [pam-auth](https://tools.suckless.org/slock/patches/pam_auth/)
      - replaces shadow support with PAM authentication support

   - [quickcancel](https://tools.suckless.org/slock/patches/quickcancel/)
      - cancel slock by moving the mouse within a certain time-period after slock started
      - this can be useful if you forgot to disable xautolock during an activity that requires no
        input (e.g. reading text, watching video, etc.)

   - [secret-password](https://tools.suckless.org/slock/patches/secret-password/)
      - allows for commands to be executed when the user enters special passwords

   - [terminalkeys](https://tools.suckless.org/slock/patches/terminalkeys/)
      - adds key commands that are commonly used in terminal applications (in particular the login
        prompt)

   - [unlockscreen](https://tools.suckless.org/slock/patches/unlock_screen/)
      - this patch keeps the screen unlocked, but keeps the input locked
      - that is, the screen is not affected by slock, but users will not be able to interact with
        the X session unless they enter the correct password

   - [xresources](https://tools.suckless.org/slock/patches/xresources/)
      - this patch adds the ability to get colors via Xresources
