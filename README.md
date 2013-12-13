glimpse_client
==============

### Build requirements
* Mobile UI requires at least Qt 5.2.0
* Console version may also work with Qt 4.8
* **Linux**: libwnck-dev package
* **Android**: Android SDK and NDK

### Building
1. Download and install the latest Qt version for your platform from http://download.qt-project.org/
2. Clone this repository or fork it first if you're going to contribute
```
git clone git@github.com:YOURUSERNAME/glimpse_client.git &&
git submodule init && 
git submodule update
```
3. Run QtCreator, open client.pro, add your Qt configuration and select the "mobile" project near the "Play" button