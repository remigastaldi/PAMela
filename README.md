# PAMela

PAM was created by Sun Microsystems in 1995 as an authentication framework.  
It then was improved and widely spread among Unix-like systems.  

The goal of this project was to write a fully working PAM-module in C that opens one or several  
user’s encrypted containers.  

• Each user have his own container in his home.  
• When a user logs on, it's open his container (decrypt). When the user logs
out, his container is closed.  

## Usage:
### Install module:
```bash
sudo make install
```
### Uninstall module:
```bash
sudo make uninstall
```
### Check if module is present:
```bash
sudo make check
```
