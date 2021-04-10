# BPS Documentation Contributors Guide

### **Important: Make sure you have cloned the BPS repository before following these instructions.**

The BPS Documentation can be viewed [here](bps.rtfd.io). The documentation is built using [sphinx](https://docs.readthedocs.io/en/stable/intro/getting-started-with-sphinx.html). These instructions assume that you have already cloned the BPS repository. I would recommend working in Linux, Windows Subsystem for Linux (WSL), or Windows since I have successfully built the docs in these environments. I have not tried this on a Mac.

## Tools setup instructions (Linux/WSL)

1. Open up a terminal.
2. Make sure Python and pip are installed. If they are not, you can install them by running `sudo apt install python3` and `sudo apt install pip3`.
3. Install all of the packages we are using for our docs by navigating to the `BPS/Docs` folder and running `pip3 install -r requirements.txt`.
4. To make sure everything is installed properly, run `make html` in the `BPS/Docs` folder. This will build the docs based on the code on your machine. You should get a message that ends in "The HTML pages are in build/html."

### Common Issues (Linux/WSL)
- If you need to add sphinx to your path, you can do it by:
    - Open your .bashrc file. You can do this by running `vim $HOME/.bashrc` or `nano $HOME/.bashrc` for example.
    - Scroll to the bottom and add the directory containing `sphinx-build`, `sphinx-quickstart`, etc to your path by adding
      the following line: `export PATH="$PATH:<your path>"`
    - For example, if the path you want to add is `$HOME/.local/bin` (I think this is the default on Linux/WSL), write
      `export PATH="$PATH:$HOME/.local/bin`
    - Save and exit the file
    - Launch a new instance of your shell (run `bash`)

## Tools setup instructions (Windows)

1. Open up command prompt (cmd.exe).
2. Make sure Python and pip are installed (you can do this by running `python --version`). If they are not, you can install them [here](https://www.python.org/downloads/). Note that pip is installed with Python.
3. Install all of the packages we are using for our docs by [navigating](http://www.cs.columbia.edu/~sedwards/classes/2015/1102-fall/Command%20Prompt%20Cheatsheet.pdf) to the `BPS/Docs` folder and running `pip3 install -r requirements.txt`.
4. To make sure everything is installed properly, run `make html` in the `BPS/Docs` folder. This will build the docs based on the code on your machine. You should get a message that ends in "The HTML pages are in build/html."

### Common Issues (Windows)
- If you get a message similar to the following:
```
WARNING: The scripts sphinx-apidoc.exe, sphinx-autogen.exe, sphinx-build.exe and sphinx-quickstart.exe are installed in 'C:\Users\clark\AppData\Local\Packages\PythonSoftwareFoundation.Python.3.9_qbz5n2kfra8p0\LocalCache\local-packages\Python39\Scripts' which is not on PATH.
  Consider adding this directory to PATH or, if you prefer to suppress this warning, use --no-warn-script-location.
```
you can fix this by running the following command:
```
set PATH=%PATH%;C:\Users\clark\AppData\Local\Packages\PythonSoftwareFoundation.Python.3.9_qbz5n2kfra8p0\LocalCache\local-packages\Python39\Scripts
```
Note that your path will be slightly different than mine.

## Editing the docs
5. To open the built docs, navigate to `BPS/Docs/build/html` and open the file called `index.html` in a web browser (This can be done in your terminal or in a file explorer).
6. To edit the content of the docs, edit the `.rst` files in `BPS/Docs/source` with a text editor of choice. If you do not have one, I recommend [VSCode](https://code.visualstudio.com/download). The documentation is written in [markdown](https://www.markdownguide.org/basic-syntax/).
7. If you want to see your changes, run `make html` in the `BPS/Docs` folder and it will rebuild the docs.


For any questions, post in the BPS Slack channel or contact the BPS lead (Clark Poon)
