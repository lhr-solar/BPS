"""
Useful functions.
"""

DEBUG = True

def log(s, end="\n"):
    if DEBUG:
        print(s, end=end)
