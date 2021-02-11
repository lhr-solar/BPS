# BPS Documentation Contributors Guide

The BPS Documentation can be viewed [here](bps.rtfd.io). The documentation is built using [sphinx](https://docs.readthedocs.io/en/stable/intro/getting-started-with-sphinx.html). These instructions assume that you have already cloned the BPS repository. I would recommend working in Linux, Windows Subsystem for Linux (WSL), or Windows since I have successfully built the docs in these environments. I have not tried this on Mac.

## Tools setup instructions (Linux/WSL)

1. Open up a terminal.
2. Make sure Python and pip are installed. If they are not, you can install them by running `sudo apt install python3` and `sudo apt install pip3`.
3. Install all of the packages we are using for our docs by navigating to the `BPS/Docs` folder and running `pip3 install -r requirements.txt`.
4. To make sure everything is installed properly, run `make html` in the `BPS/Docs` folder. This will build the docs based on the code on your machine. You should get a message that ends in "The HTML pages are in build/html."

## Tools setup instructions (Windows)

1. Open up command prompt (cmd.exe).
2. Make sure Python and pip are installed (you can do this by running `python --version`). If they are not, you can install them [here](https://www.python.org/downloads/). Note that pip is installed with Python.
3. Install all of the packages we are using for our docs by navigating to the `BPS/Docs` folder and running `pip3 install -r requirements.txt`.
4. To make sure everything is installed properly, run `make html` in the `BPS/Docs` folder. This will build the docs based on the code on your machine. You should get a message that ends in "The HTML pages are in build/html."

## Editing the docs
5. To open the built docs, navigate to `BPS/Docs/build/html` and open the file called `index.html` in a web browser (This can be done in your terminal or in a file explorer).
6. To edit the content of the docs, edit the `.rst` files in `BPS/Docs/source` with a text editor of choice. If you do not have one, I recommend [VSCode](https://code.visualstudio.com/download). The documentation is written in [markdown](https://www.markdownguide.org/basic-syntax/).
7. If you want to see your changes, run `make html` in the `BPS/Docs` folder and it will rebuild the docs.


For any questions, post in the BPS Slack channel or contact the BPS lead (Clark Poon)
