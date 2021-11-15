# How to Install the Nanny-On Agent


## Manual Installation

If you have downloaded a `211013-nannyon-2.0.1.1-linux-ubuntu-5.3.0-61.el6.x86_64.tar.gz`
file, use the following steps to install it.


### Required Steps

1. **Move the `.tar.gz` file to the directory where you'd like to install it.** <br>
   Don't use a temporary directory.

2. **untar/uncompress the file.** <br>
   To untar `.tar.gz` file, enter the following command:
   
   ```tar -zxvf 211013-nannyon-2.0.1.1-linux-ubuntu-5.3.0-61.el6.x86_64.tar.gz```
   
   This will create a `media` directory

3. In the `media` directory, execute the `nanny_install.sh` script: <br>

   ```sudo ./nanny_install.sh```

   That will launch an interactive installer, which will emit a series of prompts 
   for you to respond to. 
   
   ```bash
    ######################################
    #                                    #
    #     Nanny-On agent installation    #
    #                                    #
    ######################################
    
    [*] Check install privilege.
    
    [*] Check target os information.
    
    [1] System Check ..... [kernel version][X.X.X-XX-generic] [ok].
    
    [2] System Check ..... [glibc version][glibc X.XX] [ok].
    
    [*] Check previous installed package.
    
    [*] Install sqlite and unixodbc.
    
    Sqlite pkg is installed.
    
    Libsqlite3odbc is installed.
    
    Unixodbc pkg is installed.
   ```
  
  4. To set logon server information, enter the logon server ip and port information: <br>

   ```bash
    [*] Set logon server infomation.

     > Enter ip for logon server. : XXX.XXX.XXX.XXX
     
     > Enter port for logon server (default : 60010). : XXXXX
     
     --------------------------------
     
     Confirm logon server information
     
     XXX.XXX.XXX.XXX:XXXXX
     
     --------------------------------
     
     > Is it correct? [Y/n] y
   ```
  
 5. The final step in the process is to install files and set agent environments. <br>

   ```bash
    [*] Create nanny agent infomation file.

    [*] Install nanny agent files.
     
    [*] Create nanny's database.
     
    Success to create nanny's database.
     
    [*] Setup environment.
     
    [01] Set install root path.
    
    [02] Set agent db information.
    
    [03] Set logon svr information.
     
    [*] Setup init scripts.
    
    [*] Setup init monitor scripts.
    
    [*] Setup init acccntl scripts.
   ```
  
 6. After the process is complete, the installer will print a success message. <br>
  
 
   ```[*] Installation is done.```
    

# Uninstalling 

If you want to uninstall the release, run the `nanny_uninstall.sh` script.

   ```/usr/local/ashin/nanny/inst/nanny_uninstall.sh```

This will revert all changes made by the `install.sh` script.

**NOTE:** The `nanny_uninstall.sh` file isn't created until `nanny_install.sh` is run for the first time. 
You must run `nanny_install.sh` before running `nanny_uninstall.sh`.

----

