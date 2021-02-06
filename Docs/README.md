# BPS Documentation Contributors Guide

The BPS Documentation can be viewed [here](bps.rtfd.io). The documentation is built using [sphinx](https://docs.readthedocs.io/en/stable/intro/getting-started-with-sphinx.html). I would recommend working in Linux or Windows Subsystem for Linux (WSL), since I have successflly built the docs in both environments. I have not tried building in Windows or Mac.

## Tools setup instructions (Linux/WSL)

1. Make sure Python and pip are installed. If they are not, you can install them by running `sudo apt install python3` and `sudo apt install pip3`
2. Install all of the packages we are using for our docs by navigating to the `BPS/Docs` folder and running `pip3 install -r requirements.txt`
3. To make sure everything is installed properly, run `make html` in the `BPS/Docs` folder. This will build the docs based on the code on your machine. You should get a message that ends in "The HTML pages are in build/html."
4. To open the built docs, navigate to `BPS/Docs/build/html` and open the file called `index.html` in a web browser.
5. To edit the content of the docs, edit the `.rst` files in `BPS/Docs/source`

For any questions, post in the BPS Slack channel or contact the BPS lead (Clark Poon)
