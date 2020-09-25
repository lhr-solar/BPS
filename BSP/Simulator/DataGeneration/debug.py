"""
Useful functions.
"""

DEBUG = False

def log(s, end="\n"):
    if DEBUG:
        print(s, end=end)
