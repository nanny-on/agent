# Nanny-On agent for Linux

This repository contains Nanny-On agent source codes. Anyone can contribute to this project.

Welcome to Nanny-On Open Source Project
=======================================

Nanny-On is an application control program taking advantage of adaptive whitelisting technologies.
Generally, application control programs enhance security features and availability of embedded systems such as POS, CD/ATM, ICS(Industrial Control System), etc.
However, we wish to extend the target system to personal and server computers. Nanny-On project is to implement and maintain application control software for Linux kernels.

![image](https://github.com/nanny-on/misc/blob/main/images/usecase.png)

You are always welcomed to join this project.

This project is sponsored by [NIPA] to promote Open Source Software development.  

 [NIPA]: <https://www.nipa.kr/eng/index.do>

Nanny-On: Next Generation Endpoint Security Protection
======================================================

Endpoints are a primary target of cyber attacks. Nanny-On delivers the most complete endpoint security functionality using state-of-the-art technologies from preventing execution of unknown malware to hardening systems.

![image](https://github.com/nanny-on/misc/blob/main/images/nannyon_img.png)

Actually, we have really focused on implementing whitelist-based application control on Linux systems. The reason why we've done it is to overcome the limitations of traditional anti-virus software using signature-based malware detection thechnologies(a.k.a. blacklist-based).

![image](https://github.com/nanny-on/misc/blob/main/images/whitevsblack.png)

Why Whitelist-based Application Control
=======================================

Application whitelisting has been based on zero-trust concept(default-deny). Users or system managers only trust applications which were made and published by well-known developers or software vendors.

![image](https://github.com/nanny-on/misc/blob/main/images/trustmodel.png)

When it comes to prevention from malware infection, application controls will bring a variety of benefits to us.
Firstly, you can protect your endpoints from zero-day attack by unkown malware. This is because application controls eanble your computer systems to execute only approved applications. In other word, excution of unauthorized programs including computer viruses will be blocked or suspended. 
In addition, prvention from running useless processes not only makes resource management efficient, but improves produtivity.

How to contribute to Nanny-On Project
=====================================

We always welcome to contribute our opensource project Nanny-On.
For deatails, see [this].

[this]: <https://github.com/nanny-on/agent/blob/main/CONTRIBUTING.md>

License
=======

Nanny-On for Linux is licensed under the GNU General Public License version 3, which means that
you are free to get and use it for commercial and non-commercial purposes
as long as you fulfill its conditions.

See the [LICENSE](https://github.com/nanny-on/agent/blob/main/LICENSE) file for more details.

Copyright
=========

Copyright (c) 2013-2021 ASHINi Corp.

All rights reserved.
