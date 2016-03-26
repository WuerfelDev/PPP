# PPP - PowerPoint Presenter
"PowerPoint Presenter" app &amp; watchapp
<br>

This is a watchapp and an Android app wich lets you do the PC's PowerPoint/LibreOffice/OpenOffice NEXT and BACK funtion.


[→ DOWNLOAD THE APP](https://github.com/WuerfelDev/PPP/blob/master/android-debug.apk?raw=true)
-------------------

<br>
Pro & Contra
------------

<b>PRO</b>
- Supported by every "presentation-device" that supports external USB keyboards and switches the pages with arrow keys
- Easy setup
- self explanatory
- At least all devices running [nethunter](https://github.com/pelya/android-keyboard-gadget#installation) supported

<b>CONTRA</b>
- You need one of [these smartphones](https://github.com/pelya/android-keyboard-gadget#installation) with the specified kernel.




How it works
------------

When first starting the android app, it downloads [`hid-gadget-test`](https://github.com/pelya/android-keyboard-gadget) and <i>installs</i> it like it is shown [here](https://github.com/pelya/android-keyboard-gadget#scripting).
The <b>Pebble watchapp</b> sends an integer (which represents the pressed button) via AppMessage to the Android app. This integer is assigned to a command. This either sends `right` or `left` through USB to the PC.



