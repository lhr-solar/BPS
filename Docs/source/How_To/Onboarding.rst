**********
Onboarding
**********

Mindset
#######
Don’t be afraid to ask questions. If someone brings up a term that you do not know at a meeting, feel free to ask what it means.
There will probably be other people there who have the same question. The BPS system is probably more complex than any class project you will work on.

Getting Started
###############

First Steps
***********
1. If you are running Linux, that's great. If you are running Windows, `install Windows Subsystem for Linux <https://docs.microsoft.com/en-us/windows/wsl/install-win10>`_. If you are running Mac OS, talk to Clark. (I don't see why our tools couldn't be set up on Mac, but I have never tried.)
2. Ask for access to the Github, Google Drive, Slack, Discord, OneDrive, and Monday.
3. Check the README (scroll down) in the `BPS repository <https://github.com/lhr-solar/BPS>`_ for instructions on how to set up your environment.
4. Check out our starter projects. These are designed to introduce new members to the BPS architecture and C programming.

Help, I'm Stuck!
################
In software development, it is fairly common to come across a problem that you do not know how to solve. When this happens to you (I know it will happen
to you because it happens to me every day), I have found three approaches for coming up with the solution to the problem.

Approach #1: Look it up
***********************
Sometimes the best approach is to consult the documentation, datasheet, reference manual, or your favorite search engine. Of course, this only works
if someone else who has experienced a similar problem has documented their solution. This approach is usually the way to go when your problem is related to:

1. Programming language syntax (How do I do X in C?)
2. Programming language features (eg. ``printf()`` or any other standard library function)
3. The behavior of a commonly used tool (eg. git, gcc, make, VSCode, etc.). For most terminal commands, you can get access to documentation by running the command with ``<command> --help`` or ``man <command>``. ``man`` pages and the ``--help`` command are especially useful because they were written by the people who made the tools.
4. Common error messages (paste them into Google and see if anything comes up)
5. The behavior of a specific part (eg. why does the STM32F413, LTC6811, etc. not behave how I think it should?)
6. Anything else that is documented somewhere.

Approach #2: Ask someone for help
*********************************
When you can't find your problem in some documentation or online, the easiest solution is often to ask someone for help. Generally, the best people to ask
for help are people who have worked on similar parts of the project before. For example, if you need help figuring out how a specific function in the BPS codebase
works, you can check the ``git blame`` and see who wrote that function. They will probably be able to help you understand their code. This approach is generally
the way to go when your problem is related to:

1. Something specific to our vehicle that is not documented anywhere. (Even if it is documented somewhere, if you can't find where it's documented or the documentation is not helpful, please ask someone for help. I will be the first to admit that are docs aren't perfect.)
2. The requirements for the task you are working on. (eg. I don't understand/I forgot what Clark wants me to work on)
3. Something that you don't know how to look up. This tends to happen if you don't know the proper search terms to get google to give you the answer. 
4. Something that comes up in a meeting that you don't understand. (eg. Wtf is a "thread" or a "can"? I thought this was engineering, not arts and crafts!)
5. Anything else that you can't solve by looking it up.

Approach #3: Think really hard
******************************
Sometimes you can't find the answer online, and you can't find someone else who can help you. In my personal experience, this mainly happens during debugging.
In situations like this, you just have to think about the issue and what would need to happen to cause the bug you are facing. Hopefully this will lead to
you coming up with a theory you can test (or at least point raise some questions that you can answer by looking them up or asking someone for help).

Useful Resources
################
    These materials were put together by Alex, a previous BPS member, who found them helpful for learning about the tools used by the BPS team. Clark has made 
    some additions. Alex's complete notes can be found `here <https://docs.google.com/document/d/194_Bps9CNa6AP8NM2QrIr0dryWyDBb6Pbnc9A6n8sl4/view>`__

Git/Github
**********
    Watch `Git and Github for beginners by FreeCodeAcademy <https://www.youtube.com/watch?v=RGOj5yH7evk&t=1045s>`_
    Provides you with a very solid understanding of how git and github works. Way better than any other videos out there 
    No background knowledge on Git/Github required

    `How to add SSH Key <https://www.youtube.com/watch?v=WgZIv5HI44o>`_
    When you first start out you need to add a SSH Key. The video explains how to add

    `What’s Git? <https://www.youtube.com/watch?v=USjZcfj8yxE>`_
    In addition of FreeCodeAcademy video, this video shows a lot of commands and is a good review

    `This pdf <https://education.github.com/git-cheat-sheet-education.pdf>`_ lists a lot of the basic commands that you will need to know when using Git

    Git's docs can be found `here <https://git-scm.com/docs>`_

Ubuntu / VScode
***************
    `What’s a text editor and IDE <https://www.makeuseof.com/tag/text-editors-vs-ides-one-better-programmers/>`_
    If you are confused on what VScode is and why we use it watch this
    
    `Terminal V.S Bash V.S. Command Line V.S. Prompt <https://www.youtube.com/watch?v=hMSByvFHOro>`_
    If you are confused by all these terms. 

    What’s Ubuntu and why we use it:
    Quote from Sensei Clark Poon - “Ubuntu is one of the more commonly used linux distributions. We use Linux because there are a lot of helpful (and free) software development tools that are designed primarily for Linux instead of Windows. Some examples that we use on BPS are: git(version control), gcc (C compiler), make (code building tool)

C Programming
*************
    Some documentation of the C programming language can be found `here <https://en.cppreference.com/w/c>`__
