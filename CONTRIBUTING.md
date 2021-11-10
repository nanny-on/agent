# Contributing to Nanny-On Project

Thank you for your consideration of contributing to Nanny-On Project.

How to contribute to Nanny-On Project
=====================================

Development is done on GitHub in the [nanny-on/agent] repository.

  [nanny-on/agent]: <https://github.com/nanny-on/agent>

To request new features or report bugs, please open an issue on GitHub

To submit a patch, please open a pull request on GitHub.  If you are thinking
of making a large contribution, open an issue for it before starting work,
to get comments from the community.  Someone may be already working on
the same thing or there may be reasons why that feature isn't implemented.

Coding Style
============

We know that coding sytle is very personal. However, an open source project is likely to have a variety of members and contributors.
To make sure effective communication between them, we hopefully recommend to comply with [Linux kernel coding style].

[Linux kernel coding style]: <https://www.kernel.org/doc/Documentation/process/coding-style.rst>

For Testing and Development
===========================

Although testing and development could in theory also be done using
the source tarballs, having a local copy of the git repository with
the entire project history gives you much more insight into the
code base.

The official Nanny-On Git Repository is located at [github.com/nanny-on/agent].
 
 [github.com/nanny-on/agent]: <https://github.com/nanny-on/agent>

A local copy of the Git Repository can be obtained by cloning it from
the original Nanny-On repository using

    $ git clone git://github.com/nanny-on/agent.git

If you intend to contribute to Nanny-On project, either to fix bugs or contribute
new features, you need to fork the Nanny-On repository nanny-on/agent on
GitHub and clone your public fork instead.

How to Build
============

Bofore Building
---------------

You may need some pre-installed development tools related to compile Nanny-On agent.
First of all, you have to install C/C++ compilers and "make" on your ubuntu system.

    $ sudo apt-get update
    $ sudo apt-get install build-essential

For the next step, you should install SQLite as a DBMS tool. SQLite is simple, but powerful embedded SQL database engine. Unlike other database systems, SQLite does not need to run on a server, and all information is stored into a single file.

    $ sudo apt-get install sqlite3 libsqlite3-dev

The last step is to install UNIXODBC tool. UNIXODBC is an open specification for providing application developers with a predictable API with which to access Data Sources.

    $ sudo apt-get install unixodbc unixodbc-dev

Building Nanny-On agent
-----------------------

As we mentioned before, we strongly recommend to test and develop with a local copy of the git repository. Therefore you have to install 'git' on your ubuntu system.

    $ sudo apt-get install git

After installing git, you can clone Nanny-On agent source codes into your local system.
    
    $ git clone git://github.com/nanny-on/agent.git

Now, you can see directory "agent" that is the local copy of the git repository and just type "make". That's all!

    $ cd agent
    $ make

If you have any questions or issues, please post the issues on GitHub in the [nanny-on/agent/issues] repository.

  [nanny-on/agent/issues]: <https://github.com/nanny-on/agent/issues>

